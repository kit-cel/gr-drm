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
import drm
import drm_init
#import drm_swig

class qa_generate_sdc_vb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.tp = drm_init.transm_params(1, 3, False, 0, False, 1, 0, 1, 1, 0, False, 24000)
        self.src = drm.generate_sdc_vb(self.tp)
        self.head = gr.head(self.tp.sdc().L(), 1)
        self.snk = gr.vector_sink_b(self.tp.sdc().L())
        self.tb.connect(self.src, self.head, self.snk)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        res = self.snk.data()
        ref = (0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,0,1,0,0,0,1,0,1,0,1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,1,1,1,0,0,1,1,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,0,0,1,1,0,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,1,1,1,1,1,0,0)
        # check data
        self.assertTupleEqual(res, ref)
       

if __name__ == '__main__':
    gr_unittest.main ()
