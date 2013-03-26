#!/usr/bin/env python
# 
# Copyright 2013 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

import numpy as np
import cmath
from gnuradio import gr
#import pylab as pl

# custom implementation of numpy.corrcoef that works with complex arrays
def complex_corrcoef(x, y=None, rowvar=1, bias=0, ddof=None):
    c = complex_cov(x, y, rowvar, bias, ddof)
    try:
        d = np.diag(c)
    except ValueError: # scalar covariance
        return 1
    return c/np.sqrt(np.multiply.outer(d,d))
    
def complex_cov(m, y=None, rowvar=1, bias=0, ddof=None):
    # Check inputs
    if ddof is not None and ddof != int(ddof):
        raise ValueError("ddof must be integer")
     
    X = np.array(m, ndmin=2)#, dtype=float)
    if X.shape[0] == 1:
        rowvar = 1
    if rowvar:
        axis = 0
        tup = (slice(None),np.newaxis)
    else:
        axis = 1
        tup = (np.newaxis, slice(None))
     
     
    if y is not None:
        y = np.array(y, copy=False, ndmin=2)#, dtype=float)
        X = np.concatenate((X,y), axis)
     
    X -= X.mean(axis=1-axis)[tup]
    if rowvar:
        N = X.shape[1]
    else:
        N = X.shape[0]
     
    if ddof is None:
        if bias == 0:
            ddof = 1
        else:
            ddof = 0
    fact = float(N - ddof)
     
    if not rowvar:
        return (np.dot(X.T, X.conj()) / fact).squeeze()
    else:
        return (np.dot(X, X.T.conj()) / fact).squeeze()
    

 
	
class cp_sync_py(gr.basic_block):
    """
    Perform time synchronization and fractional frequency offset correction by cylic prefix correlation.
    Symbol detection is based on a threshold detection (max. correlation value).
    Output is frequency corrected and starts with the first sample of a symbol.
    """
    
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="cp_sync_py",
            in_sig=[np.complex64],
            out_sig=[np.complex64])
            
        self.FS = 48000
        self.TU_B = 0.02133
        
        self.TG_B = 0.00533
        self.T_S = self.TU_B + self.TG_B
        self.nsamp_ts = int(round(self.FS * self.T_S))
        self.nsamp_tu = int(round(self.FS * self.TU_B))
        self.nsamp_tg = self.nsamp_ts - self.nsamp_tu
        self.tracking_mode = False # switches between acquisition and tracking mode
        self.corr_vec = np.zeros(( self.nsamp_ts, 1), dtype=np.complex64)		
        self.timing_offset = (-1, 0) # (peak index, peak value)
        self.freq_hist_len = 8
        self.frac_freq_offset_hist = []
        self.frac_freq_offset_avg = np.NaN
        self.sync_step_size = 5 # number of samples for which one estimation shall be valid
        self.sync_step_counter = 0
        self.corr_threshold = 0.6
        
    def forecast(self, noutput_items, ninput_items_required):
        # we need more samples in the input buffer than we consume because of the correlation
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = 2*self.nsamp_ts
            
    def find_timing_offset(self, in0):
        # calculate correlation values for sliding window
        for i in range(self.nsamp_ts):
            self.corr_vec[i] = abs(complex_corrcoef(in0[i:self.nsamp_tg-1+i], in0[self.nsamp_tu+i:self.nsamp_ts-1+i])[0][1])
            
        #corr_vec_abs = np.abs(self.corr_vec)
        peak_index = 0;
        peak_val = 0;
        for k in range(self.nsamp_ts):
    		if self.corr_vec[k] > peak_val:
    			peak_val = self.corr_vec[k]
    			peak_index = k		
        
        self.timing_offset = (peak_index, peak_val)
    
    def find_frac_freq_offset(self, in0):
        #calculate average phase difference between the two intervals and determine the fractional frequency offset
        #phase_diff = cmath.phase(complex(self.corr_vec[self.timing_offset[0]].real, self.corr_vec[self.timing_offset[0]].imag))
        phase_diff = 0;
        for i in range(self.nsamp_tg): # e^(jwt + phi_1 - (jwt + phi_2) = e^(phi_2 - phi_1) --> arg(x) = delta_phi
            phase_diff += cmath.phase(in0[self.timing_offset[0]+self.nsamp_tu+i] / in0[self.timing_offset[0]+i]) 
        cur_frac_freq_offset = phase_diff / (self.TU_B) / 2 / cmath.pi / self.nsamp_tg		
        self.find_avg_frac_freq_offset(cur_frac_freq_offset)
    
    def find_avg_frac_freq_offset(self, cur_offset): # moving average of frac_freq_offset
        if len(self.frac_freq_offset_hist) < self.freq_hist_len: # append to history if history is not filled
            self.frac_freq_offset_hist.append(cur_offset)
            
        else: # if history is filled, shift history and drop oldest entry
            for i in range(self.freq_hist_len - 1): 
                self.frac_freq_offset_hist[i] = self.frac_freq_offset_hist[i+1]
            self.frac_freq_offset_hist[self.freq_hist_len-1] = cur_offset
        print "avg:", self.frac_freq_offset_avg
        print "history:", self.frac_freq_offset_hist       
        self.frac_freq_offset_avg= np.mean(self.frac_freq_offset_hist)
        
    def correct_frac_freq_offset(self, in0):
        # multiply input vector with a complex exponential function to compensate fractional frequency offset
        for i in range(self.nsamp_ts): # start correction at the beginning of the detected symbol
            in0[self.timing_offset[0] + i] = in0[self.timing_offset[0] + i] * np.exp(-1j*2*np.pi*self.frac_freq_offset_avg*i/self.FS)
            
        return in0[self.timing_offset[0] : self.timing_offset[0] + self.nsamp_ts]        
    
    def reset_estimates(self):
        self.timing_offset = (-1, 0)
        self.frac_freq_offset_avg = np.NaN
        self.frac_freq_offset_hist = []
        self.tracking_mode = False
        
    def general_work(self, input_items, output_items):
        in0 = input_items[0]        
        out = output_items[0]
        
        # drop, if not enough samples in input buffer
        if len(in0) < 2*self.nsamp_ts:
            #print "not enough samples, skip work()"
            #self.consume_each(len(in0))
            self.sync_step_counter = 0
            self.reset_estimates()
            return 0            
            
        if not(self.tracking_mode): # acquisition mode, try to find a symbol
           # print "acquisition mode"
            self.sync_step_counter = 0
            self.find_timing_offset(in0)        
            self.find_frac_freq_offset(in0)

        elif self.sync_step_counter >= self.sync_step_size: # tracking mode, refresh estimates
            #print "tracking mode, estimation"
            self.sync_step_counter = 0
            self.find_timing_offset(in0)        
            self.find_frac_freq_offset(in0)  

        #else: # tracking mode, but no estimation for this symbol
            #print "tracking mode, no estimation, counter:", self.sync_step_counter
        
        #print "t_off index / corr: ", self.timing_offset[0], "/", float(self.timing_offset[1]), \
        #"; f_off cur / avg: ", float(self.frac_freq_offset_hist[0]), "/", self.frac_freq_offset_avg
        
        self.consume_each(self.nsamp_ts)
        
        # threshold decision, if a signal was detected or not
        if self.timing_offset[1] > self.corr_threshold:
            out[0:self.nsamp_ts] = self.correct_frac_freq_offset(in0)
            self.tracking_mode = True
            self.sync_step_counter += 1
            return self.nsamp_ts # return aligned symbol
        else:
            self.reset_estimates()
            return 0 # no output if no symbol was found
        
             
        
        
		

