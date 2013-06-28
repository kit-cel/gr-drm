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

from gnuradio import gr, gr_unittest, blocks
import pylab as pl
import drmrx_swig
from qam_demod import qam_demod

class qa_qam_demod (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.rx = drmrx_swig.drmrx_conf()
        self.rx.set_sdc_const_size(16)
        self.rx.set_msc_const_size(64)
        test_vec = [1+1j, 1-1j, -1+1j, -1-1j]
        self.src_fac = blocks.vector_source_c([x/pl.sqrt(2) for x in test_vec])
        self.src_sdc = blocks.vector_source_c([x/pl.sqrt(10) for x in test_vec])
        self.src_msc = blocks.vector_source_c([x/pl.sqrt(42) for x in test_vec])
        self.head = blocks.head(gr.sizeof_gr_complex, 100)
        self.snk_fac = blocks.vector_sink_b()
        self.snk_sdc = blocks.vector_sink_b()
        self.snk_msc = blocks.vector_sink_b()
        self.qamdemod_fac = qam_demod(self.rx, "FAC")
        self.qamdemod_sdc = qam_demod(self.rx, "SDC")
        self.qamdemod_msc = qam_demod(self.rx, "MSC")

    def tearDown (self):
        self.tb = None

    def test_001_t (self): # FAC
        print "test 4 QAM"
        # set up fg
        self.tb.connect(self.src_fac, self.head, self.qamdemod_fac, self.snk_fac)
        self.tb.run ()
        # check data
        print self.snk_fac.data()
        
    def test_002_t (self): # SDC
        print "test 16 QAM"
        # set up fg
        self.tb.connect(self.src_sdc, self.head, self.qamdemod_sdc, self.snk_sdc)
        self.tb.run ()
        # check data
        print self.snk_sdc.data()
        
    def test_003_t (self): # MSC
        print "test 64 QAM"
        # set up fg
        self.tb.connect(self.src_msc, self.head, self.qamdemod_msc, self.snk_msc)
        self.tb.run ()
        # check data
        print self.snk_msc.data()
        
if __name__ == '__main__':
    gr_unittest.run(qa_qam_demod)
