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

class qa_qam_map_bc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        tp = drm.transm_params(1, 3, False, 0, 1, 0, 1, 1, 0, False, 24000, "station label", "text message")
        self.tables = tp.cfg().ptables()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # this models 16-QAM mapping with 2 input streams
        v0 = (0,0,0,1,1,0,1,1)
        v1 = (0,1,1,1,0,0,1,0)
        self.src0 = blocks.vector_source_b(v0, False)
        self.src1 = blocks.vector_source_b(v1, False)
        self.map = drm.qam_map_bc(self.tables.d_QAM16, 4, len(v0)/2, 2)
        self.snk = blocks.vector_sink_c()
        self.tb.connect(self.src0, (self.map, 0))
        self.tb.connect(self.src1, (self.map, 1))
        self.tb.connect(self.map, self.snk)
        self.tb.run ()
        # check data
        res =  self.snk.data()
        c1 = 0.9486832980
        c2 = 0.3162277660
        ref = ( complex(c1, -c2), \
                complex(-c2, -c1), \
                complex(c2, c1), \
                complex(-c1, c2) )
        self.assertComplexTuplesAlmostEqual(ref, res)
    
    def test_002_t (self):
        # this models 4-QAM with 1 input stream
        v0 = (0,0,0,1,1,0,1,1)
        self.src = blocks.vector_source_b(v0, False)
        self.map = drm.qam_map_bc(self.tables.d_QAM4, 2, len(v0)/2, 1)
        self.snk = blocks.vector_sink_c()
        self.tb.connect(self.src, self.map, self.snk)
        self.tb.run()
        # check data
        res = self.snk.data()
        c = 0.7071067811
        ref = ( complex(c, c), complex(c, -c), complex(-c, c), complex(-c, -c) )
        self.assertComplexTuplesAlmostEqual(ref, res)

    def test_003_t (self):
        # this models 64-QAM SM (only some constellation points) with 3 input streams
        v0 = (0, 0, 0, 0, 1, 1, 1, 1)
        v1 = (1, 1, 0, 0, 1, 0, 1, 1)
        v2 = (1, 0, 0, 0, 0, 1, 1, 1)
        self.src0 = blocks.vector_source_b(v0, False)
        self.src1 = blocks.vector_source_b(v1, False)
        self.src2 = blocks.vector_source_b(v2, False)
        self.map = drm.qam_map_bc(self.tables.d_QAM64SM, 6, len(v0)/2, 3)
        self.snk = blocks.vector_sink_c()
        self.tb.connect(self.src0, (self.map, 0))
        self.tb.connect(self.src1, (self.map, 1))
        self.tb.connect(self.src2, (self.map, 2))
        self.tb.connect(self.map, self.snk)
        self.tb.run()
        res = self.snk.data()
        c1 = 0.1543033499
        c2 = 0.4629100498
        c3 = 0.7715167498
        c4 = 1.0801234497
        ref = ( complex(-c3,c2), complex(c4,c4), complex(c1,-c2), complex(-c4,-c4) )
        self.assertComplexTuplesAlmostEqual(ref, res)

if __name__ == '__main__':
    gr_unittest.run(qa_qam_map_bc, "qa_qam_map_bc.xml")
