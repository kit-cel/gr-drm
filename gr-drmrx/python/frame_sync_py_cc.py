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

from gnuradio import gr
import numpy as np
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
        
class frame_sync_py_cc(gr.basic_block):
    """
    Samples are expected to come in fftshifted. Detects the start of a frame by correlating with time pilot symbols.
    NOTE: ONLY RM B SO 3 supported!
    """
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="frame_sync_py_cc",
            in_sig=[np.complex64],
            out_sig=[np.complex64])

        # rx class and some shortcuts
        self.rx = rx
        self.p = rx.p()
        self.RM = self.p.RM_B() # fixed for now
        self.SO = self.p.SO_3() # fixed for now
        self.nfft = self.p.nfft()[self.RM]
        self.dc_carr = self.nfft/2
        #self.k_min = self.p.k_min()[self.RM][self.SO]   
        self.time_pil_pos = self.p.time_pil_pos()[self.RM][:]
        self.time_pil_phase = self.p.time_pil_phase()[self.RM][:]
        self.num_pilots = len(self.time_pil_pos)
        self.time_pil = self.calc_time_pil()                
        self.extracted_pilots = np.zeros((self.num_pilots,), dtype=np.complex64)
        self.corr_threshold = 0.8
        self.frame_detected = False
        
        self.set_output_multiple(self.nfft)
    
    def calc_time_pil(self):
        pilot_symbols = np.zeros((self.num_pilots,), dtype=np.complex64)
        for i in range(self.num_pilots): 
            #print "carrier index (raw/dc_off):", self.time_pil_pos[i], self.time_pil_pos[i]+self.dc_carr, "phase index:", self.time_pil_phase[i]
            pilot_symbols[i] = np.exp(2j*np.pi*self.time_pil_phase[i]/1024)

        return pilot_symbols
     
    def extract_pilots_from_symbol(self, in0):
        for i in range(self.num_pilots):
            self.extracted_pilots[i] = in0[self.dc_carr + self.time_pil_pos[i]]
            
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = self.nfft

    def general_work(self, input_items, output_items):
        in0 = input_items[0]

        if not(self.frame_detected):
            self.extract_pilots_from_symbol(in0)
            self.consume_each(self.nfft)
            if abs(complex_corrcoef(self.extracted_pilots, self.time_pil)[0][1]) > self.corr_threshold:
                print "detected start of frame"
                self.frame_detected = True
                output_items[0][:self.nfft] = in0[:self.nfft]
                return self.nfft
            else:
                print "no start of frame detected"
                return 0
        else:
            output_items[0][:self.nfft] = in0[:self.nfft]
            self.consume_each(self.nfft)
            return self.nfft
