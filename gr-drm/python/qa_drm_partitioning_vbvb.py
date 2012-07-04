#!/usr/bin/env python
# 
# Copyright 2012 <+YOU OR YOUR COMPANY+>.
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
#

from gnuradio import gr, gr_unittest
import drm_init
import drm
#import drm_swig

class qa_partitioning_vbvb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.src = gr.vector_source_b((1,2,3,4,5,6,7,8,9), 1, 9)
        self.demux = drm.partitioning_vbvb(9, (2,3,4))
        self.head = gr.head(9,2)
        self.snk1 = gr.vector_sink_b(2)
        self.snk2 = gr.vector_sink_b(3)
        self.snk3 = gr.vector_sink_b(4)
        
        self.tb.connect(self.src, self.head, self.demux)
        self.tb.connect((self.demux,0), self.snk1)
        self.tb.connect((self.demux,1), self.snk2)
        self.tb.connect((self.demux,2), self.snk3)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        res1 = self.snk1.data()
        res2 = self.snk2.data()
        res3 = self.snk3.data()
        # check data
        ref1 = (1,2,1,2)
        ref2 = (3,4,5,3,4,5)
        ref3 = (6,7,8,9,6,7,8,9)
        self.assertTupleEqual(res1, ref1)
        self.assertTupleEqual(res2, ref2)
        self.assertTupleEqual(res3, ref3)
        

if __name__ == '__main__':
    gr_unittest.main ()
