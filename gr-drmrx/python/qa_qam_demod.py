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
        self.src = blocks.null_source(gr.sizeof_gr_complex)
        self.head = blocks.head(gr.sizeof_gr_complex, 100)
        self.snk = blocks.null_sink(gr.sizeof_char)
        self.qamdemod = qam_demod(self.rx, "FAC")

    def tearDown (self):
        self.tb = None
        
    def debug_plot(self):
        print "constellation:",self.qamdemod.constellation

    def test_001_t (self):
        # set up fg
        self.tb.connect(self.src, self.head)
        self.tb.connect(self.head, self.qamdemod)
        self.tb.connect(self.qamdemod, self.snk)
        self.tb.run ()
        # check data
        self.debug_plot()


if __name__ == '__main__':
    gr_unittest.run(qa_qam_demod)
