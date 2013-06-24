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

from gnuradio import gr, gr_unittest
import drmrx_swig
import os
from cell_demapping_py_cc import cell_demapping_py_cc

class qa_cell_demapping_py_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        print "instantiate rx instance..."
        self.rx = drmrx_swig.drmrx_conf()
        print "done. get p()..."
        self.p = self.rx.p()
        print "done."
        self.src = gr.null_source(gr.sizeof_gr_complex)
        self.head = gr.head(gr.sizeof_gr_complex, 100)
        self.cell_demapper = cell_demapping_py_cc(self.rx)
        self.snk1 = gr.null_sink(gr.sizeof_gr_complex)
        self.snk2 = gr.null_sink(gr.sizeof_gr_complex)
        self.snk3 = gr.null_sink(gr.sizeof_gr_complex)

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        self.tb.connect(self.src, self.head, self.cell_demapper)
        self.tb.connect((self.cell_demapper,0), self.snk1)
        self.tb.connect((self.cell_demapper,1), self.snk2)
        self.tb.connect((self.cell_demapper,2), self.snk3)
        self.tb.run ()
        # check data
        print sum(self.cell_demapper.channel_pos)


if __name__ == '__main__':
    print 'Blocked waiting for GDB attach (pid = %d)' % (os.getpid(),)
    raw_input ('Press Enter to continue: ')
    gr_unittest.run(qa_cell_demapping_py_cc)
