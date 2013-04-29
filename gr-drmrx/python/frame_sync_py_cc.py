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
    docstring for block frame_sync_py_cc
    """
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="frame_sync_py_cc",
            in_sig=[np.complex64],
            out_sig=[np.complex64])

        # rx class and some shortcuts
        self.rx = rx
        self.p = rx.p()
        self.RM = self.p.RM_B()
        self.nfft = self.p.nfft()[self.RM]
        self.time_pil = self.p.time_pil()[self.RM]
    
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = noutput_items

    def general_work(self, input_items, output_items):
        output_items[0][:] = input_items[0]
        #self.consume_each(0, len(input_items[0]))
        #self.consume_each(len(input_items[0]))
        return len(output_items[0])
