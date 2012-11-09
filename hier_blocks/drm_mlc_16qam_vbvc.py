#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: DRM MLC 16-QAM
# Author: Felix Wunsch
# Description: performs multilevelcoding (16-QAM)
# Generated: Wed Jul 18 16:06:39 2012
##################################################

from gnuradio import gr
from gnuradio import trellis
from gnuradio.gr import firdes
import drm

class drm_mlc_16qam_vbvc(gr.hier_block2):

	def __init__(self, vlen_in=1, vlen_out=1, n_tailbits=6, denom_mother_code_rate=6, gen_poly=(91, 121, 101, 91, 121, 101), bits_per_symbol=0, N=1, map_tab=0, pp_0=0, pp_0_tail=0, pp_1=0, pp_1_tail=0, part_len_top=1, part_len_bot=1, M_total=0, interl_seq_0_2=range(2), interl_seq_1_2=range(2)):
		gr.hier_block2.__init__(
			self, "DRM MLC 16-QAM",
			gr.io_signature(1, 1, gr.sizeof_char*vlen_in),
			gr.io_signature(1, 1, gr.sizeof_gr_complex*vlen_out),
		)

		##################################################
		# Parameters
		##################################################
		self.vlen_in = vlen_in
		self.vlen_out = vlen_out
		self.n_tailbits = n_tailbits
		self.denom_mother_code_rate = denom_mother_code_rate
		self.gen_poly = gen_poly
		self.bits_per_symbol = bits_per_symbol
		self.N = N
		self.map_tab = map_tab
		self.pp_0 = pp_0
		self.pp_0_tail = pp_0_tail
		self.pp_1 = pp_1
		self.pp_1_tail = pp_1_tail
		self.part_len_top = part_len_top
		self.part_len_bot = part_len_bot
		self.M_total = M_total
		self.interl_seq_0_2 = interl_seq_0_2
		self.interl_seq_1_2 = interl_seq_1_2

		##################################################
		# Blocks
		##################################################
		self.trellis_encoder_xx_0_0 = trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0)
		self.trellis_encoder_xx_0 = trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0)
		self.gr_vector_to_stream_1_0 = gr.vector_to_stream(gr.sizeof_char*1, part_len_bot+ n_tailbits)
		self.gr_vector_to_stream_1 = gr.vector_to_stream(gr.sizeof_char*1, part_len_top + n_tailbits)
		self.gr_unpack_k_bits_bb_0_0 = gr.unpack_k_bits_bb(denom_mother_code_rate)
		self.gr_unpack_k_bits_bb_0 = gr.unpack_k_bits_bb(denom_mother_code_rate)
		self.gr_stream_to_vector_0_0 = gr.stream_to_vector(gr.sizeof_char*1, (part_len_bot + n_tailbits) * denom_mother_code_rate)
		self.gr_stream_to_vector_0 = gr.stream_to_vector(gr.sizeof_char*1, (part_len_top + n_tailbits) * denom_mother_code_rate)
		self.drm_qam_map_vbvc_0 = drm.qam_map_vbvc(map_tab, bits_per_symbol, vlen_out, 2)
		self.drm_punct_vbvb_0_0 = drm.punct_vbvb(pp_1, pp_1_tail, (part_len_bot + n_tailbits) * denom_mother_code_rate, vlen_out * 2, n_tailbits * denom_mother_code_rate)
		self.drm_punct_vbvb_0 = drm.punct_vbvb(pp_0, pp_0_tail, (part_len_top + n_tailbits) * denom_mother_code_rate, vlen_out * 2, n_tailbits * denom_mother_code_rate)
		self.drm_partitioning_16_vbvb_0 = drm.partitioning_vbvb(vlen_in, M_total)
		self.drm_interleaver_vbvb_0_0 = drm.interleaver_vbvb((interl_seq_1_2))
		self.drm_interleaver_vbvb_0 = drm.interleaver_vbvb((interl_seq_0_2))
		self.add_tailbits_vbvb_0_0 = drm.add_tailbits_vbvb(part_len_bot, n_tailbits)
		self.add_tailbits_vbvb_0 = drm.add_tailbits_vbvb(part_len_top, n_tailbits)

		##################################################
		# Connections
		##################################################
		self.connect((self.drm_interleaver_vbvb_0, 0), (self.drm_qam_map_vbvc_0, 0))
		self.connect((self.drm_qam_map_vbvc_0, 0), (self, 0))
		self.connect((self.trellis_encoder_xx_0, 0), (self.gr_unpack_k_bits_bb_0, 0))
		self.connect((self.gr_stream_to_vector_0, 0), (self.drm_punct_vbvb_0, 0))
		self.connect((self.gr_unpack_k_bits_bb_0, 0), (self.gr_stream_to_vector_0, 0))
		self.connect((self.drm_punct_vbvb_0, 0), (self.drm_interleaver_vbvb_0, 0))
		self.connect((self.trellis_encoder_xx_0_0, 0), (self.gr_unpack_k_bits_bb_0_0, 0))
		self.connect((self.gr_stream_to_vector_0_0, 0), (self.drm_punct_vbvb_0_0, 0))
		self.connect((self.gr_unpack_k_bits_bb_0_0, 0), (self.gr_stream_to_vector_0_0, 0))
		self.connect((self.drm_punct_vbvb_0_0, 0), (self.drm_interleaver_vbvb_0_0, 0))
		self.connect((self.drm_interleaver_vbvb_0_0, 0), (self.drm_qam_map_vbvc_0, 1))
		self.connect((self.gr_vector_to_stream_1, 0), (self.trellis_encoder_xx_0, 0))
		self.connect((self.gr_vector_to_stream_1_0, 0), (self.trellis_encoder_xx_0_0, 0))
		self.connect((self, 0), (self.drm_partitioning_16_vbvb_0, 0))
		self.connect((self.add_tailbits_vbvb_0, 0), (self.gr_vector_to_stream_1, 0))
		self.connect((self.add_tailbits_vbvb_0_0, 0), (self.gr_vector_to_stream_1_0, 0))
		self.connect((self.drm_partitioning_16_vbvb_0, 1), (self.add_tailbits_vbvb_0_0, 0))
		self.connect((self.drm_partitioning_16_vbvb_0, 0), (self.add_tailbits_vbvb_0, 0))

	def get_vlen_in(self):
		return self.vlen_in

	def set_vlen_in(self, vlen_in):
		self.vlen_in = vlen_in

	def get_vlen_out(self):
		return self.vlen_out

	def set_vlen_out(self, vlen_out):
		self.vlen_out = vlen_out

	def get_n_tailbits(self):
		return self.n_tailbits

	def set_n_tailbits(self, n_tailbits):
		self.n_tailbits = n_tailbits

	def get_denom_mother_code_rate(self):
		return self.denom_mother_code_rate

	def set_denom_mother_code_rate(self, denom_mother_code_rate):
		self.denom_mother_code_rate = denom_mother_code_rate

	def get_gen_poly(self):
		return self.gen_poly

	def set_gen_poly(self, gen_poly):
		self.gen_poly = gen_poly

	def get_bits_per_symbol(self):
		return self.bits_per_symbol

	def set_bits_per_symbol(self, bits_per_symbol):
		self.bits_per_symbol = bits_per_symbol

	def get_N(self):
		return self.N

	def set_N(self, N):
		self.N = N

	def get_map_tab(self):
		return self.map_tab

	def set_map_tab(self, map_tab):
		self.map_tab = map_tab

	def get_pp_0(self):
		return self.pp_0

	def set_pp_0(self, pp_0):
		self.pp_0 = pp_0

	def get_pp_0_tail(self):
		return self.pp_0_tail

	def set_pp_0_tail(self, pp_0_tail):
		self.pp_0_tail = pp_0_tail

	def get_pp_1(self):
		return self.pp_1

	def set_pp_1(self, pp_1):
		self.pp_1 = pp_1

	def get_pp_1_tail(self):
		return self.pp_1_tail

	def set_pp_1_tail(self, pp_1_tail):
		self.pp_1_tail = pp_1_tail

	def get_part_len_top(self):
		return self.part_len_top

	def set_part_len_top(self, part_len_top):
		self.part_len_top = part_len_top

	def get_part_len_bot(self):
		return self.part_len_bot

	def set_part_len_bot(self, part_len_bot):
		self.part_len_bot = part_len_bot

	def get_M_total(self):
		return self.M_total

	def set_M_total(self, M_total):
		self.M_total = M_total

	def get_interl_seq_0_2(self):
		return self.interl_seq_0_2

	def set_interl_seq_0_2(self, interl_seq_0_2):
		self.interl_seq_0_2 = interl_seq_0_2

	def get_interl_seq_1_2(self):
		return self.interl_seq_1_2

	def set_interl_seq_1_2(self, interl_seq_1_2):
		self.interl_seq_1_2 = interl_seq_1_2


