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
import pylab as pl
from gnuradio import gr

class detect_SO_py_cc(gr.basic_block):
    """
    Detect SO by looking for band edges. 
    TODO: implement reset mechanism
    """
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="detect_SO_py_cc",
            in_sig=[np.complex64],
            out_sig=[np.complex64])
        
        self.rx = rx
        self.p = rx.p()
        self.RM = self.p.RM_B()
        self.nfft = self.p.nfft()[self.RM]
        self.estimated_SO = self.p.SO_NONE()
        self.SO_detected = False
        
    def forecast(self, noutput_items, ninput_items_required):
        # we need more samples in the input buffer than we consume because of the correlation
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = self.nfft
            
    def detect_SO(self, in0):
        in0_mag = self.moving_average(abs(in0[:])) # do this to avoid locking on fading gaps
        dc_carr = self.nfft/2
        delta_f = self.p.FS() / self.nfft
        # start at the dc carrier and progress in both directions until the magnitude is considerably lower
        k_low = dc_carr + 10 # RM A starts above DC
        k_high = dc_carr
        
        #debug plot
        #pl.plot(in0_mag)
        #pl.title("averaged magnitude of input FFT")     
        #pl.show()
        
        ctr = 0
        while k_low == dc_carr+10 and ctr < self.nfft/2+20: # in RM A, the lowest carrier is above DC
            ctr+=1
            if in0_mag[dc_carr+20-ctr] / in0_mag[dc_carr+20-ctr-10] > 5: # if ratio between bins becomes substantial, an edge is found
                k_low = dc_carr+20-ctr-5 # actual edge is assumed to be in the middle between the compared bins
                ctr = 0
        while k_high == dc_carr and ctr < self.nfft/2:
            ctr+=1
            if in0_mag[dc_carr+ctr] / in0_mag[dc_carr+ctr+10] > 5: # find upper edge
                k_high = dc_carr + ctr + 5
        
        est_bandwidth = (k_high - k_low) * delta_f
        #print "detect_SO_py_cc: est_bandwidth:", est_bandwidth, ". upper/lower bound:", k_high, k_low
        self.estimated_SO = self.estimate_SO(est_bandwidth)
        print "detect_SO_py_cc: detected SO", self.estimated_SO
                   
    def moving_average(self, vec): # short MA (2 samples window, look ahead)
        window_len = 5
        vec_avg = np.zeros((len(vec),))
        np.concatenate((vec, vec[0:window_len])) # cyclic continuation for MA filtering
        for i in range(len(vec_avg)):
            vec_avg[i] = sum(vec[i:i+window_len]) / window_len
            
        return vec_avg
        
    def estimate_SO(self, est_bw): 
        if est_bw >= 4250 and est_bw < 46:
            return self.p.SO_0()
        elif est_bw >= 4600 and est_bw < 5200:
            return self.p.SO_1()
        elif est_bw >= 8500 and est_bw < 9100:
            return self.p.SO_2()
        elif est_bw >= 9100 and est_bw < 10100:
            return self.p.SO_3()
        elif est_bw >= 17100 and est_bw < 18100:
            return self.p.SO_4()
        elif est_bw >= 19100 and est_bw < 20100:
            return self.p.SO_5()
        else:
            print "detect_SO_py_cc: No SO detected!"
            return self.p.SO_NONE()
    
    def general_work(self, input_items, output_items):
        in0 = input_items[0]       
        out = output_items[0]
        
        if self.SO_detected:
            in0 = in0[:self.nfft]
            out[:self.nfft] = in0
            self.consume_each(self.nfft)
            return self.nfft
        else:
            in0 = in0[:self.nfft]
            self.detect_SO(in0[:])
            self.rx.set_SO(self.estimated_SO)
            
            if self.estimated_SO != self.p.SO_NONE():
                self.SO_detected = True # this is never set back to false!
                out[:self.nfft] = in0
                self.consume_each(self.nfft)
                return self.nfft
            else:
                self.consume_each(self.nfft)
                return 0

