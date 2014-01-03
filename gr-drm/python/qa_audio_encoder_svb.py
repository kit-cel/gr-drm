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

class qa_audio_encoder_svb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.tp = drm.transm_params(1, 3, False, 0, False, 1, 0, 1, 1, 0, False, 24000, "station label", "this is a long sample text message!")
        self.audio_enc = drm.audio_encoder_svb(self.tp)
        self.src = blocks.null_source(4)
        self.head = blocks.head(4,  960*10*3)
        self.snk = blocks.vector_sink_b(self.tp.msc().L_MUX())
        
        self.tb.connect(self.src, self.head, self.audio_enc, self.snk)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.run ()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_audio_encoder_svb, "qa_audio_encoder_svb.xml")
