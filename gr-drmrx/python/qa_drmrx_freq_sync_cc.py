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

class qa_freq_sync_cc (gr_unittest.TestCase):

	def setUp (self):
		print "setUp()"
		self.tb = gr.top_block ()
		self.rx = drmrx_swig.drmrx_conf()
		self.freq_acq = drmrx_swig.freq_sync_cc(self.rx)
		self.src = gr.null_source(gr.sizeof_gr_complex*1)
		self.snk = gr.null_sink(gr.sizeof_gr_complex*1)
		self.head = gr.head(gr.sizeof_gr_complex*1, 110000)
		self.samp_rate = 250000;
		self.f1 = analog.sig_source_c(self.samp_rate, analog.GR_SIN_WAVE, 750, 1, 0) 
		self.f2 = analog.sig_source_c(self.samp_rate, analog.GR_SIN_WAVE, 2250, 1, 0) 
		self.f3 = analog.sig_source_c(self.samp_rate, analog.GR_SIN_WAVE, 3000, 1, 0) 
		self.add = gr.add_vcc(1)

	def tearDown (self):
		print "tearDown()"
		self.tb = None

	def test_001_t (self):
		print "test_001"
		# set up fg
		self.tb.connect(self.src, self.head, self.freq_acq, self.snk)
		self.tb.run ()

		# check data
		pil = self.freq_acq.pilot_pattern()
		self.assertEqual(self.freq_acq.nsamp_sym(), drmrx_swig.FS*drmrx_swig.T_O)
		self.assertEqual(len(pil), self.freq_acq.nsamp_sym())

	def test_002_t (self):
		print "test_002"
		self.tb.connect(self.f1, (self.add, 0))
		self.tb.connect(self.f2, (self.add, 1))
		self.tb.connect(self.f3, (self.add, 2))
		self.tb.connect(self.add, self.head, self.freq_acq, self.snk)
		self.tb.run()


if __name__ == '__main__':
    gr_unittest.main ()
