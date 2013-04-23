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
import pylab as pl

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
    Symbol detection is based on a threshold decision (max. correlation value).
    Output is frequency corrected and starts with the first sample of a symbol with its cylic prefix removed.
    TODO: maybe divide this block up in timing offset detection -> fractional frequency offset correction -> cylic prefix removal
    """
    
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="cp_sync_py",
            in_sig=[np.complex64],
            out_sig=[np.complex64])
        self.rx = rx
        self.FS = rx.p().FS()
        self.nsamp_ts = self.rx.p().nsamp_Ts()
        self.nsamp_tu = self.rx.p().nsamp_Tu()
        self.nsamp_tg = self.rx.p().nsamp_Tg()
        self.tracking_mode = False # switches between acquisition and tracking mode
        self.coarse_freq_offset = []
        self.corr_vec = [np.zeros(( self.nsamp_ts[0], 1), dtype=np.complex64), np.zeros(( self.nsamp_ts[1], 1), \
            dtype=np.complex64), np.zeros(( self.nsamp_ts[2], 1), dtype=np.complex64), np.zeros(( self.nsamp_ts[3], 1), dtype=np.complex64)]                           
        self.corr_vec_abs = [np.zeros(( self.nsamp_ts[0], 1)), np.zeros(( self.nsamp_ts[1], 1)), \
            np.zeros(( self.nsamp_ts[2], 1)), np.zeros(( self.nsamp_ts[3], 1))]              
        self.timing_offset = (-1, 0) # (peak index, peak value)
        self.timing_backoff = 10 # backoff to prevent late sync
        self.freq_hist_len = 4
        self.freq_hist_ctr = 0
        self.freq_hist_filled = False
        #self.frac_freq_offset_hist = np.zeros((self.freq_hist_len,))
        self.frac_freq_offset_hist = []
        self.frac_freq_offset_avg = np.NaN
        self.sync_step_size = 3 # number of symbols for which one estimation shall be valid
        self.sync_step_counter = 0
        self.corr_threshold = 0.7
        
    def forecast(self, noutput_items, ninput_items_required):
        # we need more samples in the input buffer than we consume because of the correlation
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = 2*max(self.nsamp_ts)
            
    def find_tags(self):
        nread = self.nitems_read(0)
        ninput_items = []
        if self.rx.RM() != self.rx.p().RM_NONE():
            ninput_items = self.nsamp_ts[self.rx.RM()]
        else:
            ninput_items = max(self.nsamp_ts)
            
        tags = self.get_tags_in_range(0, nread, nread+ninput_items, gr.pmt.pmt_string_to_symbol("coarse_freq_offset"))
        
        if len(tags) > 0:
            if len(tags) > 1:
                print "cp_sync_py: got more tags than expected (", len(tags), "). Discarding all but the first."
            if self.coarse_freq_offset == []: # first run
                self.coarse_freq_offset = gr.pmt.pmt_to_uint64(tags[0].value)
                
            if self.coarse_freq_offset != gr.pmt.pmt_to_uint64(tags[0].value): # coarse frequency estimate has changed
                self.coarse_freq_offset = gr.pmt.pmt_to_uint64(tags[0].value)
                self.reset_freq()
            #print gr.pmt.pmt_symbol_to_string(tags[0].key), gr.pmt.pmt_to_uint64(tags[0].value)
       
       
            
    def find_timing_offset(self, in0):
        # calculate correlation values for sliding window for every robustness mode
        
        if self.tracking_mode: # wait until freq_hist is filled to get a more reliable result
            
            if self.sync_step_counter == 0: # calculate timing and frequency offset for current RM
            # NOTE: UPDATING ONLY ONE ROW OF THE CORRELATION MATRIX RESULTS IN DIFFERENT ROWS REFERRING TO DIFFERENT SAMPLES
                for k in range(self.nsamp_ts[self.rx.RM()]):
                    self.corr_vec[self.rx.RM()][k] = complex_corrcoef(in0[k:self.nsamp_tg[self.rx.RM()]-1+k], in0[self.nsamp_tu[self.rx.RM()]+k:self.nsamp_ts[self.rx.RM()]-1+k])[0][1]
                    self.corr_vec_abs[self.rx.RM()][:] = abs(self.corr_vec[self.rx.RM()][:])
                    peak_index = 0
                    peak_val = 0
                    
                for k in range(self.nsamp_ts[self.rx.RM()]):
            		if self.corr_vec_abs[self.rx.RM()][k] > peak_val:
            			peak_val = self.corr_vec_abs[self.rx.RM()][k]
            			peak_index = k  
               
                timing_diff = abs(self.timing_offset[0] - peak_index)
                if peak_val > self.corr_threshold and timing_diff < self.nsamp_tg[self.rx.RM()] / 5:                
                    self.timing_offset = (peak_index, peak_val)
                
                    if (self.timing_offset[0] < 0.1 * self.nsamp_ts[self.rx.RM()]) or (self.timing_offset[0] > self.nsamp_ts[self.rx.RM()] - 0.1 * self.nsamp_ts[self.rx.RM()]):
                        # if the offset is close to zero or nsamp_ts, consume some items to prevent losing whole symbols through wrap-arounds
                        # after that, a resync is needed   
                        print "cp_sync_py: consuming some items to prevent wrap-around"
                        self.consume_each(self.nsamp_ts[self.rx.RM()]/2)
                        self.reset_all()
                        return 0
                
                else:
                    self.reset_all()  
                
            else: # increase counter and wrap if needed
                self.sync_step_counter = (self.sync_step_counter + 1) % self.sync_step_size
                
        else:
            for i in range(4): # modes A, B, C, D
                for k in range(self.nsamp_ts[i]):
                    self.corr_vec[i][k] = complex_corrcoef(in0[k:self.nsamp_tg[i]-1+k], in0[self.nsamp_tu[i]+k:self.nsamp_ts[i]-1+k])[0][1]
                    self.corr_vec_abs[i][:] = abs(self.corr_vec[i][:])
                
            peak_index = [0, 0, 0, 0]
            peak_val = [0, 0, 0, 0]
            for i in range(4): # find maximum in every correlation vector
                for k in range(self.nsamp_ts[i]):
            		if self.corr_vec_abs[i][k] > peak_val[i]:
            			peak_val[i] = self.corr_vec_abs[i][k]
            			peak_index[i] = k
    
            new_rm_index = []
            maxval = max(peak_val)
            for i in range(4):
                if peak_val[i] == maxval: # comparing float here. should be okay because maxval is a simple copy
                    new_rm_index = i
            
            #print "peak_index:", peak_index, "peak_val:", list(peak_val)
            #self.debug_plot()
            
            # decide, if the estimation is reliable enough
            if peak_val[new_rm_index] > self.corr_threshold:
                
                self.timing_offset = (peak_index[new_rm_index], peak_val[new_rm_index])
                
                if (self.timing_offset[0] < 0.1 * self.nsamp_ts[new_rm_index]) or (self.timing_offset[0] > self.nsamp_ts[new_rm_index] - 0.1 * self.nsamp_ts[new_rm_index]):
                    # if the offset is close to zero or nsamp_ts, consume some items to prevent losing whole symbols through wrap-arounds
                    # after that, a resync is needed   
                    print "cp_sync_py: consuming some items to prevent wrap-around"
                    self.consume_each(self.nsamp_ts[new_rm_index]/2)
                    self.reset_all()
                    return 0
                    
                else:
                    self.rx.set_RM(new_rm_index) # make detected RM "official"
                    print "cp_sync_py: enter tracking mode. RM:", self.rx.RM()#, ", correlation value:", float(peak_val[self.rx.RM()])
                    self.tracking_mode = True
                
            else:
                print "cp_sync_py: correlation value too low (", float(peak_val[new_rm_index]), ")"
                self.reset_all()           
            
    def find_frac_freq_offset(self, in0):
        #calculate average phase difference between the two intervals and determine the fractional frequency offset
        phase_diff = cmath.phase(complex(self.corr_vec[self.rx.RM()][self.timing_offset[0]].real, self.corr_vec[self.rx.RM()][self.timing_offset[0]].imag))
        time_diff = float(self.nsamp_tu[self.rx.RM()]) / self.FS
        cur_frac_freq_offset = - phase_diff / (2*cmath.pi * time_diff)
    
        self.find_avg_frac_freq_offset(cur_frac_freq_offset)
    
    def find_avg_frac_freq_offset(self, cur_offset): # moving average of frac_freq_offset     
        self.frac_freq_offset_hist.append(cur_offset)
        cur_list_len = len(self.frac_freq_offset_hist)
        if cur_list_len > self.freq_hist_len:
            self.frac_freq_offset_hist = self.frac_freq_offset_hist[cur_list_len-self.freq_hist_len:]

        self.frac_freq_offset_avg = np.mean(self.frac_freq_offset_hist)
            
    def correct_frac_freq_offset(self, in0):
        # multiply input vector with a complex exponential function to compensate fractional frequency offset
        for i in range(self.nsamp_ts[self.rx.RM()]): # start correction at the beginning of the detected symbol
            in0[self.timing_offset[0] + i] = in0[self.timing_offset[0] + i] * np.exp(-1j*2*np.pi*self.frac_freq_offset_avg*i/self.FS)
            
        return in0[self.timing_offset[0] : self.timing_offset[0] + self.nsamp_ts[self.rx.RM()]]  
        
    def remove_cp(self, in0):
        # remove cylic prefix. to prevent late synchronization, a timing backoff is applied. this
        # results in a constant phase shift.
        symbol_start = self.nsamp_tg[self.rx.RM()] - self.timing_backoff
        return in0[symbol_start : symbol_start + self.nsamp_tu[self.rx.RM()]]
        
    def reset_freq(self):
        print "cp_sync_py: reset frequency estimation because coarse estimate has changed"
        self.frac_freq_offset_hist = []
        self.frac_freq_offset_avg = np.NaN
        self.sync_step_counter = 0
        
    def reset_all(self):
        print "cp_sync_py: reset sync due to overflow or low correlation peak"
        self.timing_offset = (-1, 0)
        self.frac_freq_offset_avg = np.NaN
        self.frac_freq_offset_hist = []
        self.tracking_mode = False
        self.rx.set_RM(self.rx.p().RM_NONE()) # reset RM to invalid
        
    def debug_plot(self):
        pl.subplot(4,1,1)
        pl.plot(self.corr_vec[0][:])
        pl.ylabel("RM A")
        pl.subplot(4,1,2)
        pl.plot(self.corr_vec[1][:])
        pl.ylabel("RM B")
        pl.subplot(4,1,3)
        pl.plot(self.corr_vec[2][:])
        pl.ylabel("RM C")
        pl.subplot(4,1,4)
        pl.plot(self.corr_vec[3][:])
        pl.ylabel("RM D")
        pl.show()
        
    def general_work(self, input_items, output_items):
        in0 = input_items[0]        
        out = output_items[0]
        
        # drop, if not enough samples in input buffer
        if len(in0) < 2*max(self.nsamp_ts):
            print "cp_sync_py: not enough samples, skip work()"
            self.sync_step_counter = 0
            return 0            
        
        self.find_tags()
        self.find_timing_offset(in0) # TODO: make this more modular!
        
        if self.tracking_mode:
            self.find_frac_freq_offset(in0)
            in0[0:self.nsamp_ts[self.rx.RM()]] = self.correct_frac_freq_offset(in0)
            out[0:self.nsamp_tu[self.rx.RM()]] = self.remove_cp(in0)
#            print "cp_sync_py: t_off index / corr: ", self.timing_offset[0], "/", self.timing_offset[1].real, \
#                "; f_off cur / avg: ", self.frac_freq_offset_hist[0], "/", self.frac_freq_offset_avg
            print "cp_sync_py: current / average offset:", self.frac_freq_offset_hist[0], "/", self.frac_freq_offset_avg, "Hz. Timing:", self.timing_offset[0]
            self.consume_each(self.nsamp_ts[self.rx.RM()])
            return self.nsamp_tu[self.rx.RM()]
            
        else:
            self.consume_each(max(self.nsamp_ts))
            return 0
