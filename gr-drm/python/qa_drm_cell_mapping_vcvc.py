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
import drm_swig as drm
#import drm

class qa_cell_mapping_vcvc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.tp = drm.transm_params(1, 3, False, 0, False, 1, 0, 1, 1, 0, False, 24000, "station label", "text message")
        vlen_msc = self.tp.msc().N_MUX() * self.tp.ofdm().M_TF()
        vlen_sdc = self.tp.sdc().N()
        vlen_fac = self.tp.fac().N() * self.tp.ofdm().M_TF()
        self.cell_mapping = drm.cell_mapping_vcvc(self.tp, (vlen_msc, vlen_sdc, vlen_fac))

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        # check data


if __name__ == '__main__':
    gr_unittest.main ()
