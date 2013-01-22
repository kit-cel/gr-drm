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

from gnuradio import gr, gr_unittest
import drmrx_swig

class qa_freq_sync_cc (gr_unittest.TestCase):

	def setUp (self):
		self.tb = gr.top_block ()
		self.rx = drmrx_swig.drmrx_conf()
		self.freq_acq = drmrx_swig.freq_sync_cc(self.rx)
		self.src = gr.null_source(gr.sizeof_gr_complex*1)
		self.snk = gr.null_sink(gr.sizeof_gr_complex*1)
		self.head = gr.head(gr.sizeof_gr_complex*1, 210000)
		self.tb.connect(self.src, self.head, self.freq_acq, self.snk)

	def tearDown (self):
		self.tb = None

	def test_001_t (self):
		# set up fg
		self.tb.run ()

		# check data
		pil = self.freq_acq.pilot_pattern()
		self.assertEqual(self.freq_acq.nsamp_sym(), drmrx_swig.FS*drmrx_swig.T_O)
		self.assertEqual(len(pil), self.freq_acq.nsamp_sym())


if __name__ == '__main__':
    gr_unittest.main ()
