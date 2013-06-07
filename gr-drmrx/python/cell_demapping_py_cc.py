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

class cell_demapping_py_cc(gr.basic_block):
    """
    Demultiplexing of the logical channels (MSC, SDC, FAC). 
    NOTE: only supports RM B, SO 3
    """
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="cell_demapping_py_cc",
            in_sig=[np.complex64],
            out_sig=[np.complex64, np.complex64, np.complex64])
            
        self.rx = rx
        self.p = rx.p()
        self.SO = self.p.SO_3()
        self.RM = self.p.RM_B()
        self.nfft = self.p.nfft()[self.RM]
        # TODO: find a way to efficiently extract the channel cells
        self.sym_ctr = 0
        self.pos_msc = np.zeros
        

    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = self.nfft

    def general_work(self, input_items, output_items):
        in0 = input_items[0]
        #output_items[0][:] = input_items[0]
        output_items[0][:100] = np.zeros((100,))
        self.produce(0, 100)
        output_items[1][:200] = np.ones((200,))
        self.produce(1, 200)
        output_items[2][:300] = np.ones((300,))+1
        self.produce(2, 300)
        self.consume_each(self.nfft)
        return -2 # WORK_CALLED_PRODUCE
