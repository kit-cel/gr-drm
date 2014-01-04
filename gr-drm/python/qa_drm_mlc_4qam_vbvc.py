#!/usr/bin/env python
# 
# Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
# Copyright 2014 REFSETH Sanntidssystemer, rfz.com
# Author: George Refseth
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
import numpy as np
import drm_swig as drm
from drm_mlc_4qam_vbvc import drm_mlc_4qam_vbvc

# Remember to add <>/gr-drm/hier_blocks/ to your PYTHONPATH before attempting to run this selftest

class qa_mlc_4qam_vbvc (gr_unittest.TestCase):

    def setUp (self):
	self.tp = drm.transm_params(1, 3, False, 0, False, 1, 0, 1, 1, 0, False, 24000, "station label", "text message")
        self.tb = gr.top_block ()
        self.src = blocks.vector_source_b((0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0), 0, 72)
        self.mlc_4qam = drm_mlc_4qam_vbvc(vlen_in=self.tp.fac().L(), vlen_out=self.tp.fac().N(), n_tailbits=drm.N_TAILBITS / drm.DENOM_MOTHER_CODE, denom_mother_code_rate=drm.DENOM_MOTHER_CODE, gen_poly=(91, 121, 101, 91, 121, 101), N=self.tp.fac().N(), bits_per_symbol=self.tp.fac().mod_order(), pp=self.tp.fac().punct_pat_0(), pp_tail=self.tp.fac().punct_pat_0(), interl_seq=self.tp.fac().bit_interl_seq_0_2(), map_tab=self.tp.cfg().ptables().d_QAM4)
        self.snk = blocks.vector_sink_b(520)
        self.tb.connect(self.src, self.mlc_4qam, self.snk)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        res = self.snk.data()
        ref = (243, 4, 53, 63)
        # check data
        self.assertTupleEqual(res[0:4], ref)


if __name__ == '__main__':
    gr_unittest.main ()
