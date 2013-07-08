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
        
        # set bit assignments
        self.bit_assignment = [[] for i in range(self.const_size)] # creates const_size empty lists
        # constellation points are numbered from the top right corner downwards, then proceeding to the left row per row
        if self.const_size == 4:
            for i in range(4):
                self.bit_assignment[i] = list(self.bin_format.format(i))
                
        elif self.const_size == 16: # gray coding in the quadrants
            self.bit_assignment[0] = list(self.bin_format.format(0))
            self.bit_assignment[1] = list(self.bin_format.format(2))
            self.bit_assignment[2] = list(self.bin_format.format(1))
            self.bit_assignment[3] = list(self.bin_format.format(3))
            self.bit_assignment[4] = list(self.bin_format.format(8))
            self.bit_assignment[5] = list(self.bin_format.format(10))
            self.bit_assignment[6] = list(self.bin_format.format(9))
            self.bit_assignment[7] = list(self.bin_format.format(11))
            self.bit_assignment[8] = list(self.bin_format.format(4))
            self.bit_assignment[9] = list(self.bin_format.format(6))
            self.bit_assignment[10] = list(self.bin_format.format(5))
            self.bit_assignment[11] = list(self.bin_format.format(7))
            self.bit_assignment[12] = list(self.bin_format.format(12))
            self.bit_assignment[13] = list(self.bin_format.format(14))
            self.bit_assignment[14] = list(self.bin_format.format(13))
            self.bit_assignment[15] = list(self.bin_format.format(15))
            
        elif self.const_size == 64: # symmetric mapping
            self.bit_assignment[0] = list(self.bin_format.format(0))
            self.bit_assignment[1] = list(self.bin_format.format(4))
            self.bit_assignment[2] = list(self.bin_format.format(2))
            self.bit_assignment[3] = list(self.bin_format.format(6))
            self.bit_assignment[4] = list(self.bin_format.format(1))
            self.bit_assignment[5] = list(self.bin_format.format(5))
            self.bit_assignment[6] = list(self.bin_format.format(3))
            self.bit_assignment[7] = list(self.bin_format.format(7))
            self.bit_assignment[8] = list(self.bin_format.format(32))
            self.bit_assignment[9] = list(self.bin_format.format(36))
            self.bit_assignment[10] = list(self.bin_format.format(34))
            self.bit_assignment[11] = list(self.bin_format.format(38))
            self.bit_assignment[12] = list(self.bin_format.format(33))
            self.bit_assignment[13] = list(self.bin_format.format(37))
            self.bit_assignment[14] = list(self.bin_format.format(35))
            self.bit_assignment[15] = list(self.bin_format.format(39))            
            self.bit_assignment[16] = list(self.bin_format.format(16))
            self.bit_assignment[17] = list(self.bin_format.format(20))
            self.bit_assignment[18] = list(self.bin_format.format(18))
            self.bit_assignment[19] = list(self.bin_format.format(22))
            self.bit_assignment[20] = list(self.bin_format.format(17))
            self.bit_assignment[21] = list(self.bin_format.format(21))
            self.bit_assignment[22] = list(self.bin_format.format(19))
            self.bit_assignment[23] = list(self.bin_format.format(23))
            self.bit_assignment[24] = list(self.bin_format.format(48))
            self.bit_assignment[25] = list(self.bin_format.format(52))
            self.bit_assignment[26] = list(self.bin_format.format(50))
            self.bit_assignment[27] = list(self.bin_format.format(54))
            self.bit_assignment[28] = list(self.bin_format.format(49))
            self.bit_assignment[29] = list(self.bin_format.format(53))
            self.bit_assignment[30] = list(self.bin_format.format(51))
            self.bit_assignment[31] = list(self.bin_format.format(55))
            self.bit_assignment[32] = list(self.bin_format.format(8))
            self.bit_assignment[33] = list(self.bin_format.format(12))
            self.bit_assignment[34] = list(self.bin_format.format(10))
            self.bit_assignment[35] = list(self.bin_format.format(14))
            self.bit_assignment[36] = list(self.bin_format.format(9))
            self.bit_assignment[37] = list(self.bin_format.format(13))
            self.bit_assignment[38] = list(self.bin_format.format(11))
            self.bit_assignment[39] = list(self.bin_format.format(15))
            self.bit_assignment[40] = list(self.bin_format.format(40))
            self.bit_assignment[41] = list(self.bin_format.format(44))
            self.bit_assignment[42] = list(self.bin_format.format(42))
            self.bit_assignment[43] = list(self.bin_format.format(46))
            self.bit_assignment[44] = list(self.bin_format.format(41))
            self.bit_assignment[45] = list(self.bin_format.format(45))
            self.bit_assignment[46] = list(self.bin_format.format(43))
            self.bit_assignment[47] = list(self.bin_format.format(47))
            self.bit_assignment[48] = list(self.bin_format.format(24))
            self.bit_assignment[49] = list(self.bin_format.format(28))
            self.bit_assignment[50] = list(self.bin_format.format(26))
            self.bit_assignment[51] = list(self.bin_format.format(30))
            self.bit_assignment[52] = list(self.bin_format.format(25))
            self.bit_assignment[53] = list(self.bin_format.format(29))
            self.bit_assignment[54] = list(self.bin_format.format(27))
            self.bit_assignment[55] = list(self.bin_format.format(31))
            self.bit_assignment[56] = list(self.bin_format.format(56))
            self.bit_assignment[57] = list(self.bin_format.format(60))
            self.bit_assignment[58] = list(self.bin_format.format(58))
            self.bit_assignment[59] = list(self.bin_format.format(62))
            self.bit_assignment[60] = list(self.bin_format.format(57))
            self.bit_assignment[61] = list(self.bin_format.format(61))
            self.bit_assignment[62] = list(self.bin_format.format(59))
            self.bit_assignment[63] = list(self.bin_format.format(63))
        
        # convert characters to uint8/byte
        for i in range(len(self.bit_assignment)):
            for k in range(len(self.bit_assignment[0])):
                self.bit_assignment[i][k] = np.uint8(self.bit_assignment[i][k])     
            
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
            out_vec[i*self.bits_per_symbol:(i+1)*self.bits_per_symbol] = self.bit_assignment[self.decided_dec[i]]
        print out_vec
        return out_vec
                
    def calc_max_sym_proc(self, in0, out):
        sym_in = len(in0)
        bits_out = len(out)
        return min(sym_in, int(float(bits_out)/self.bits_per_symbol))
        
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
        
        
#        print "len(in0)", len(in0), "len(out)", len(out)
        
        max_sym_proc = self.calc_max_sym_proc(in0, out)
        
#        if bits_out < self.bits_per_symbol:        
#            print "qam_demod: output buffer shorter than bits_per_symbol!"
        
        self.make_decision(in0, max_sym_proc)
        out[:max_sym_proc*self.bits_per_symbol] = self.dec2bin()
            
        self.consume_each(max_sym_proc)
        #print "consume", max_sym_proc, "symbols and return", max_sym_proc*self.bits_per_symbol, "bits"
        return max_sym_proc*self.bits_per_symbol