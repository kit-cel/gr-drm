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
#

from gnuradio import analog, gr, gr_unittest
import drmrx_swig

class qa_cp_sync_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.rx = drmrx_swig.drmrx_conf()
        self.cp_sync = drmrx_swig.cp_sync_cc(self.rx)

    def tearDown (self):
        self.tb = None

    def test_001_t (self): # dummy test, just testing if work() produces sensible output
        # set up fg
        print "test_001"
        self.src_noise = analog.noise_source_c(analog.GR_GAUSSIAN, 1, 0)
        self.head = gr.head(gr.sizeof_gr_complex*1, 10000)
        self.snk = gr.null_sink(gr.sizeof_gr_complex*1)
        self.tb.connect(self.src_noise, self.head, self.cp_sync, self.snk)
        self.tb.run ()
        # check data
        
    def test_002_t (self): # test with real data created with drmtx
        print "test_002"
        self.src_file = gr.file_source(gr.sizeof_gr_complex*1, "testdata_cpsync.bin", False)
        self.head = gr.head(gr.sizeof_gr_complex*1, 10000)
        self.snk = gr.null_sink(gr.sizeof_gr_complex*1)
        self.tb.connect(self.src_file, self.head, self.cp_sync, self.snk)
        self.tb.run ()


if __name__ == '__main__':
    gr_unittest.main ()
