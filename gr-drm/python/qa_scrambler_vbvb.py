#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
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

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import drm_swig as drm
import numpy as np

class qa_scrambler_bb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.src = blocks.vector_source_b((0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0), 0, 16)
        self.scrambler = drm.scrambler_bb(16)
        self.snk = blocks.vector_sink_b(16)
        self.tb.connect(self.src, self.scrambler, self.snk)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        # check data
        res = self.snk.data()
        ref = (0,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0)
        # check data
        self.assertTupleEqual(res, ref)

if __name__ == '__main__':
    gr_unittest.run(qa_scrambler_bb, "qa_scrambler_bb.xml")
