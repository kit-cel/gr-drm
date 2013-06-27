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
        self.channel_type = channel_type
        
        self.const_size = 0
        self.constellation = 0
        
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
        nrows = int(round(np.sqrt(self.const_size))) # quadratic -> nrows==ncols
        self.constellation = np.zeros((nrows, nrows), dtype=np.complex64)    
        
        if self.const_size == 4:
            a = np.sqrt(2)
        elif self.const_size == 16:
            a = np.sqrt(10)
        elif self.const_size == 64:
            a = np.sqrt(64)
        else:
            print "Invalid constellation size! a defaults to 0."
            a = 0
        a=1 # debug!
        for i in range(nrows):
            self.constellation[:,i] += 1j*a*((nrows-1)-2*i) # imaginary part
            self.constellation[i,:] += a*(2*i-(nrows-1)) # real part
        
        
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    def general_work(self, input_items, output_items):
        in0 = input_items[0]
        out = output_items[0][:]
        if self.const_size == 0: # init constellation
            self.get_const_size()
            if self.const_size != 0:
                self.make_constellation()
        
        self.consume_each(len(in0))
        return 0
