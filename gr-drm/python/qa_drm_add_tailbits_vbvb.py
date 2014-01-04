#!/usr/bin/env python
# 
# Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
# Author: Felix Wunsch
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

from gnuradio import gr, gr_unittest, blocks
#import drm
import drm_swig as drm

class qa_add_tailbits_vbvb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.src = blocks.vector_source_b((1,1,0,1), True, 4)
        self.head = blocks.head(4,3)
        self.add_tailbits = drm.add_tailbits_vbvb(4,2)
        self.snk = blocks.vector_sink_b(6)
        
        self.tb.connect(self.src, self.head, self.add_tailbits, self.snk)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        
        # check data
        self.assertTupleEqual(self.snk.data(), (1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,1,0,0))


if __name__ == '__main__':
    gr_unittest.main ()
