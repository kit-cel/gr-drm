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
from gnuradio import gr

class qam_demod(gr.basic_block):
    """
    docstring for block qam_demod
    """
    def __init__(self, rx, channel_type):
        gr.basic_block.__init__(self,
            name="qam_demod",
            in_sig=[np.complex64],
            out_sig=[bool])
            
        self.rx = rx
        self.p = rx.p()
        self.channel_type = channel_type # either FAC, SDC or MSC
        
        self.const_size = 0 # number of constellation points
        self.nrows = 0 # length of one side of the constellation diagram
        self.a = 0 # normalization factor
        self.constellation = 0 # constellation diagram
        
    def get_const_size(self):
        if self.channel_type == "MSC":
            self.const_size = self.rx.msc_const_size()
        elif self.channel_type == "SDC":
            self.const_size = self.rx.sdc_const_size()
        elif self.channel_type == "FAC":
            self.const_size = self.rx.fac_const_size() # FAC always uses 4 QAM
        else:
            print "no DRM channel specified. const_size defaults to 0."
            
    def make_constellation(self):
        self.nrows = int(round(np.sqrt(self.const_size))) # quadratic -> nrows==ncols
        self.constellation = np.zeros((self.nrows, self.nrows), dtype=np.complex64)    
        
        if self.const_size == 4:
            self.a = 1.0/np.sqrt(2)
        elif self.const_size == 16:
            self.a = 1.0/np.sqrt(10)
        elif self.const_size == 64:
            self.a = 1.0/np.sqrt(64)
        else:
            print "Invalid constellation size! a defaults to 0."
        
        for i in range(self.nrows):
            self.constellation[:,i] += 1j*self.a*((self.nrows-1)-2*i) # imaginary part
            self.constellation[i,:] += self.a*(2*i-(self.nrows-1)) # real part
     
    def make_decision(self, sample):      
        # decide for I and Q independently
        s = (sample.real, sample.imag)
        i_I = 0 # inphase index (to be interpreted in base 2 -> 3 == 11)
        i_Q = 0 # quadrature phase index
        if self.const_size == 4: # 4 QAM
            # real part
            if s[0] < 0:
                i_I = 1
            # imaginary part
            if s[1] < 0:
                i_Q = 1
        else: # FIXME! doesn't work so far...
            for i in range(self.nrows-1):
                # real part
                if s[0] > ((self.nrows-2) + 2*i)*self.a:
                    i_I = i
                # imaginary part
                if s[1] > ((self.nrows-2) + 2*i)*self.a:
                    i_Q = i

        print sample, "was decoded to", i_I, i_Q   
        return 2*i_I + i_Q
            
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    def general_work(self, input_items, output_items):
        in0 = input_items[0]
        out = output_items[0][:]
        print "i/o lens:", len(in0), len(out)
        
        # init constellation
        if self.const_size == 0: 
            self.get_const_size()
            if self.const_size != 0:
                self.make_constellation()
        
        for i in range(min(len(in0), len(out))):
            out[i] = self.make_decision(in0[i])
            
        self.consume_each(min(len(in0), len(out)))
        return min(len(in0), len(out))