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
#import drm_swig as drm
import drm
import drm_init

class qa_generate_fac_vb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.tp = drm_init.transm_params()
        self.src = drm.generate_fac_vb(self.tp)
        self.head = gr.head(self.tp.fac().L(), 3)
        self.snk = gr.vector_sink_b(self.tp.fac().L())
        
        self.tb.connect(self.src, self.head, self.snk)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        res = self.snk.data()
        # three consecutive FAC blocks
        ref =  (0,1,1,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, \
				0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,0,1, \
				1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1, \
				0,0,1,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, \
				0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,0,1, \
				1,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1, \
				0,1,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, \
				0,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,0,1, \
				1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0)
        # check data
        self.assertTupleEqual(res, ref)


if __name__ == '__main__':
    gr_unittest.main ()
