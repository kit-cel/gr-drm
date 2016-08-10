# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: MLC 16QAM
# Author: Felix Wunsch
# Description: MLC 16QAM
# Generated: Mon Aug  8 17:40:04 2016
##################################################

from gnuradio import blocks
from gnuradio import gr
from gnuradio import trellis
from gnuradio.filter import firdes
import drm


class drm_mlc_16qam_bc(gr.hier_block2):

    def __init__(self, vlen_in=1, vlen_out=1, n_tailbits=1, denom_mother_code_rate=1, gen_poly=0, bits_per_symbol=1, map_tab=0, pp_0=0, interl_seq_0_2=(0,1), interl_seq_1_2=(0,1), pp_1_tail=0, pp_1=0, pp_0_tail=0, M_total=0, part_len_top=3, part_len_bot=3):
        gr.hier_block2.__init__(
            self, "MLC 16QAM",
            gr.io_signature(1, 1, gr.sizeof_char*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
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
        self.map_tab = map_tab
        self.pp_0 = pp_0
        self.interl_seq_0_2 = interl_seq_0_2
        self.interl_seq_1_2 = interl_seq_1_2
        self.pp_1_tail = pp_1_tail
        self.pp_1 = pp_1
        self.pp_0_tail = pp_0_tail
        self.M_total = M_total
        self.part_len_top = part_len_top
        self.part_len_bot = part_len_bot

        ##################################################
        # Blocks
        ##################################################
        self.trellis_encoder_xx_top =  trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0, 0) if False else trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0) 
        self.trellis_encoder_xx_bot =  trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0, 0) if False else trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0) 
        self.drm_qam_map_bc_0 = drm.qam_map_bc(map_tab, bits_per_symbol, vlen_out, 2)
        self.drm_punct_bb_top = drm.punct_bb(pp_0, pp_0_tail, (part_len_top + n_tailbits) * denom_mother_code_rate, vlen_out * 2, n_tailbits * denom_mother_code_rate)
        self.drm_punct_bb_bot = drm.punct_bb(pp_1, pp_1_tail, (part_len_bot + n_tailbits) * denom_mother_code_rate, vlen_out * 2, n_tailbits * denom_mother_code_rate)
        self.drm_partitioning_16_bb_0 = drm.partitioning_bb(vlen_in, M_total)
        self.drm_interleaver_bb_top = drm.interleaver_bb((interl_seq_0_2))
        self.drm_interleaver_bb_bot = drm.interleaver_bb((interl_seq_1_2))
        self.drm_add_tailbits_bb_top = drm.add_tailbits_bb(part_len_top, n_tailbits)
        self.drm_add_tailbits_bb_bot = drm.add_tailbits_bb(part_len_bot, n_tailbits)
        self.blocks_unpack_k_bits_bb_top = blocks.unpack_k_bits_bb(denom_mother_code_rate)
        self.blocks_unpack_k_bits_bb_bot = blocks.unpack_k_bits_bb(denom_mother_code_rate)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_unpack_k_bits_bb_bot, 0), (self.drm_punct_bb_bot, 0))    
        self.connect((self.blocks_unpack_k_bits_bb_top, 0), (self.drm_punct_bb_top, 0))    
        self.connect((self.drm_add_tailbits_bb_bot, 0), (self.trellis_encoder_xx_bot, 0))    
        self.connect((self.drm_add_tailbits_bb_top, 0), (self.trellis_encoder_xx_top, 0))    
        self.connect((self.drm_interleaver_bb_bot, 0), (self.drm_qam_map_bc_0, 1))    
        self.connect((self.drm_interleaver_bb_top, 0), (self.drm_qam_map_bc_0, 0))    
        self.connect((self.drm_partitioning_16_bb_0, 1), (self.drm_add_tailbits_bb_bot, 0))    
        self.connect((self.drm_partitioning_16_bb_0, 0), (self.drm_add_tailbits_bb_top, 0))    
        self.connect((self.drm_punct_bb_bot, 0), (self.drm_interleaver_bb_bot, 0))    
        self.connect((self.drm_punct_bb_top, 0), (self.drm_interleaver_bb_top, 0))    
        self.connect((self.drm_qam_map_bc_0, 0), (self, 0))    
        self.connect((self, 0), (self.drm_partitioning_16_bb_0, 0))    
        self.connect((self.trellis_encoder_xx_bot, 0), (self.blocks_unpack_k_bits_bb_bot, 0))    
        self.connect((self.trellis_encoder_xx_top, 0), (self.blocks_unpack_k_bits_bb_top, 0))    

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
        self.trellis_encoder_xx_top.set_FSM(trellis.fsm(1, self.denom_mother_code_rate, self.gen_poly))
        self.trellis_encoder_xx_bot.set_FSM(trellis.fsm(1, self.denom_mother_code_rate, self.gen_poly))

    def get_gen_poly(self):
        return self.gen_poly

    def set_gen_poly(self, gen_poly):
        self.gen_poly = gen_poly
        self.trellis_encoder_xx_top.set_FSM(trellis.fsm(1, self.denom_mother_code_rate, self.gen_poly))
        self.trellis_encoder_xx_bot.set_FSM(trellis.fsm(1, self.denom_mother_code_rate, self.gen_poly))

    def get_bits_per_symbol(self):
        return self.bits_per_symbol

    def set_bits_per_symbol(self, bits_per_symbol):
        self.bits_per_symbol = bits_per_symbol

    def get_map_tab(self):
        return self.map_tab

    def set_map_tab(self, map_tab):
        self.map_tab = map_tab

    def get_pp_0(self):
        return self.pp_0

    def set_pp_0(self, pp_0):
        self.pp_0 = pp_0

    def get_interl_seq_0_2(self):
        return self.interl_seq_0_2

    def set_interl_seq_0_2(self, interl_seq_0_2):
        self.interl_seq_0_2 = interl_seq_0_2

    def get_interl_seq_1_2(self):
        return self.interl_seq_1_2

    def set_interl_seq_1_2(self, interl_seq_1_2):
        self.interl_seq_1_2 = interl_seq_1_2

    def get_pp_1_tail(self):
        return self.pp_1_tail

    def set_pp_1_tail(self, pp_1_tail):
        self.pp_1_tail = pp_1_tail

    def get_pp_1(self):
        return self.pp_1

    def set_pp_1(self, pp_1):
        self.pp_1 = pp_1

    def get_pp_0_tail(self):
        return self.pp_0_tail

    def set_pp_0_tail(self, pp_0_tail):
        self.pp_0_tail = pp_0_tail

    def get_M_total(self):
        return self.M_total

    def set_M_total(self, M_total):
        self.M_total = M_total

    def get_part_len_top(self):
        return self.part_len_top

    def set_part_len_top(self, part_len_top):
        self.part_len_top = part_len_top

    def get_part_len_bot(self):
        return self.part_len_bot

    def set_part_len_bot(self, part_len_bot):
        self.part_len_bot = part_len_bot
