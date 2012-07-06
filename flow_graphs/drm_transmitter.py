#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: DRM Transmitter
# Author: Felix Wunsch
# Description: EEP, SM (16/4), AAC mono, RM B, SO 3
# Generated: Fri Jul  6 14:06:33 2012
##################################################

from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from optparse import OptionParser
import drm
import drm_init

class drm_transmitter(gr.top_block):

	def __init__(self):
		gr.top_block.__init__(self, "DRM Transmitter")

		##################################################
		# Variables
		##################################################
		self.tp = tp = drm_init.transm_params()
		self.samp_rate = samp_rate = 32000
		self.M_SDC = M_SDC = tp.sdc().M_total()
		self.M_MSC = M_MSC = tp.msc().M_total()

		##################################################
		# Blocks
		##################################################
		self.gr_wavfile_source_0 = gr.wavfile_source("/home/felixwunsch/bachelor_thesis/gnuradio_drm/matlab_files/ifeelgood_24khz.wav", False)
		self.gr_vector_sink_x_2 = gr.vector_sink_b(M_MSC[1])
		self.gr_vector_sink_x_1 = gr.vector_sink_b(M_MSC[0] )
		self.gr_vector_sink_x_0_0 = gr.vector_sink_b(tp.fac().L())
		self.gr_vector_sink_x_0 = gr.vector_sink_b(tp.sdc().L())
		self.gr_float_to_short_0 = gr.float_to_short()
		self.drm_scrambler_vbvb_0_1 = drm.scrambler_vbvb(tp.fac().L())
		self.drm_scrambler_vbvb_0_0 = drm.scrambler_vbvb(tp.msc().L_MUX())
		self.drm_scrambler_vbvb_0 = drm.scrambler_vbvb(tp.sdc().L())
		self.drm_partitioning_4_vbvb_0_0 = drm.partitioning_vbvb(tp.fac().L(), tp.fac().M_total())
		self.drm_partitioning_4_vbvb_0 = drm.partitioning_vbvb(tp.sdc().L(), M_SDC)
		self.drm_partitioning_16_vbvb_0 = drm.partitioning_vbvb(tp.msc().L_MUX(), tp.msc().M_total())
		self.drm_generate_sdc_vb_0 = drm.generate_sdc_vb(tp)
		self.drm_generate_fac_vb_0 = drm.generate_fac_vb(tp)
		self.drm_audio_encoder_svb_0 = drm.audio_encoder_sb(tp)

		##################################################
		# Connections
		##################################################
		self.connect((self.drm_partitioning_16_vbvb_0, 0), (self.gr_vector_sink_x_1, 0))
		self.connect((self.drm_partitioning_16_vbvb_0, 1), (self.gr_vector_sink_x_2, 0))
		self.connect((self.gr_wavfile_source_0, 0), (self.gr_float_to_short_0, 0))
		self.connect((self.drm_scrambler_vbvb_0_0, 0), (self.drm_partitioning_16_vbvb_0, 0))
		self.connect((self.gr_float_to_short_0, 0), (self.drm_audio_encoder_svb_0, 0))
		self.connect((self.drm_audio_encoder_svb_0, 0), (self.drm_scrambler_vbvb_0_0, 0))
		self.connect((self.drm_partitioning_4_vbvb_0, 0), (self.gr_vector_sink_x_0, 0))
		self.connect((self.drm_scrambler_vbvb_0, 0), (self.drm_partitioning_4_vbvb_0, 0))
		self.connect((self.drm_generate_sdc_vb_0, 0), (self.drm_scrambler_vbvb_0, 0))
		self.connect((self.drm_generate_fac_vb_0, 0), (self.drm_scrambler_vbvb_0_1, 0))
		self.connect((self.drm_scrambler_vbvb_0_1, 0), (self.drm_partitioning_4_vbvb_0_0, 0))
		self.connect((self.drm_partitioning_4_vbvb_0_0, 0), (self.gr_vector_sink_x_0_0, 0))

	def get_tp(self):
		return self.tp

	def set_tp(self, tp):
		self.tp = tp
		self.set_M_SDC(self.tp.sdc().M_total())
		self.set_M_MSC(self.tp.msc().M_total())

	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate

	def get_M_SDC(self):
		return self.M_SDC

	def set_M_SDC(self, M_SDC):
		self.M_SDC = M_SDC

	def get_M_MSC(self):
		return self.M_MSC

	def set_M_MSC(self, M_MSC):
		self.M_MSC = M_MSC

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = drm_transmitter()
	tb.start()
	raw_input('Press Enter to quit: ')
	tb.stop()

