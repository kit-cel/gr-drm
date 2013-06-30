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
    TODO: correct assignment of the indexes to bit sequences. make lookup tables for I and Q.
    """
    def __init__(self, rx, channel_type):
        gr.basic_block.__init__(self,
            name="qam_demod",
            in_sig=[np.complex64],
            out_sig=[np.uint8])
            
        self.rx = rx
        self.p = rx.p()
        self.channel_type = channel_type # either FAC, SDC or MSC
        
        self.const_size = 0 # number of constellation points
        self.nrows = 0 # length of one side of the constellation diagram
        self.a = 0 # normalization factor
        self.constellation = 0 # constellation diagram
        self.decided_dec = [] # array of decided bits in decimal representation
        self.bits_per_symbol = 0 # log2(M)
        self.msb_factor = 0 # factor to scale MSB index with 
        self.bin_format = '' # needed for dec2bin conversion
        self.bit_assignment = 0 # connection between decimal indexes and actual bit sequences
        self.set_output_multiple(6) # set minimum output buffer size (bits, 6==log2(64))
        
    def set_const_params(self):
        if self.channel_type == "MSC":
            self.const_size = self.rx.msc_const_size()
        elif self.channel_type == "SDC":
            self.const_size = self.rx.sdc_const_size()
        elif self.channel_type == "FAC":
            self.const_size = self.rx.fac_const_size() # FAC always uses 4 QAM
        else:
            print "no DRM channel specified. const_size defaults to 0."
        
        self.bits_per_symbol = int(round(np.log(self.const_size)/np.log(2))) 
        self.msb_factor = int(round(np.log(self.const_size)/np.log(2)/2))
        self.bin_format = '{0:0'+str(self.bits_per_symbol)+'b}'
        
    def make_constellation(self):
        self.nrows = int(round(np.sqrt(self.const_size))) # quadratic -> nrows==ncols
        self.constellation = np.zeros((self.nrows, self.nrows), dtype=np.complex64)    
        
        if self.const_size == 4:
            self.a = 1.0/np.sqrt(2)
        elif self.const_size == 16:
            self.a = 1.0/np.sqrt(10)
        elif self.const_size == 64:
            self.a = 1.0/np.sqrt(42)
        else:
            print "Invalid constellation size! a defaults to 0."
        
        for i in range(self.nrows):
            self.constellation[:,i] += 1j*self.a*((self.nrows-1)-2*i) # imaginary part
            self.constellation[i,:] += self.a*(2*i-(self.nrows-1)) # real part
     
    def make_decision(self, vec_in, vec_len):      
        self.decided_dec = np.zeros((vec_len,), dtype=int)
        for n in range(vec_len):
            i_I, i_Q = self.nrows-1, self.nrows-1
            # decide for I and Q independently    
            for i in range(self.nrows-1): # real part, MSBs             
                if vec_in[n].real < (-(self.nrows-2) + 2*i)*self.a:
#                    print "I:",vec_in[n].real, "<", (-(self.nrows-2) + 2*i)*self.a
                    i_I = i
                    break            
            for i in range(self.nrows-1): # imaginary part, LSBs
                if vec_in[n].imag < (-(self.nrows-2) + 2*i)*self.a:
#                    print "Q:",vec_in[n].imag, "<", (-(self.nrows-2) + 2*i)*self.a 
                    i_Q = i
                    break           
            self.decided_dec[n] += self.msb_factor*i_I + i_Q # decimal index of constellation point
            print vec_in[n], "was decoded to", i_I, i_Q   

    def dec2bin(self):
                out_vec = np.zeros((len(self.decided_dec)*self.bits_per_symbol,), dtype=np.uint8)
                for i in range(len(self.decided_dec)):
                    out_vec[i*self.bits_per_symbol:(i+1)*self.bits_per_symbol] = self.bit_assignment[self.decided_dec[i])]#list(self.bin_format.format(self.decided_dec[i]))
                print out_vec
                return out_vec
                
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    def general_work(self, input_items, output_items):
        in0 = input_items[0]
        out = output_items[0][:]
        
        # init constellation
        if self.const_size == 0: 
            self.set_const_params()
            if self.const_size != 0:
                self.make_constellation()
                self.set_relative_rate(self.bits_per_symbol)
            else:
                print "qam_demod: wait for valid constellation size for", self.channel_type, ". return"
                return 0
        
        sym_in = len(in0)
#        print "len(in0)", len(in0), "len(out)", len(out)
        bits_out = len(out)
        max_sym_proc = min(sym_in, int(float(bits_out)/self.bits_per_symbol))
        
#        if bits_out < self.bits_per_symbol:        
#            print "qam_demod: output buffer shorter than bits_per_symbol!"
        
        self.make_decision(in0, max_sym_proc)
        out[:max_sym_proc*self.bits_per_symbol] = self.dec2bin()
            
        self.consume_each(max_sym_proc)
        #print "consume", max_sym_proc, "symbols and return", max_sym_proc*self.bits_per_symbol, "bits"
        return max_sym_proc*self.bits_per_symbol