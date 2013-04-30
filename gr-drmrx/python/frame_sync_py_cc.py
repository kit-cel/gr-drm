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
        self.RM = self.rx.RM()
        self.SO = self.rx.SO()
        self.nfft = self.p.nfft()[self.RM]
        self.time_pil = self.calc_time_pil()
        self.dc_carr = self.nfft/2
        self.k_min = self.p.k_min()[self.RM][self.SO]
    
    def calc_time_pil(self):
        self.time_pil = np.zeros((self.nfft,), dtype=np.complex64)
        for i in self.p.time_pil_pos()[self.RM][:]: 
            self.time_pil[i+self.dc_carr+self.k_min] = np.exp(2j*np.pi*self.p.time_pil_phase()[self.RM][i]/1024)
        
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    def general_work(self, input_items, output_items):
        output_items[0][:] = input_items[0]
        #self.consume_each(0, len(input_items[0]))
        #self.consume_each(len(input_items[0]))
        return len(output_items[0])
