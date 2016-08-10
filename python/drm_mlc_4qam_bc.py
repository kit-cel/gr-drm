# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: MLC 4QAM
# Author: Felix Wunsch
# Description: MLC 4QAM
# Generated: Mon Aug  8 17:40:02 2016
##################################################

from gnuradio import blocks
from gnuradio import gr
from gnuradio import trellis
from gnuradio.filter import firdes
import drm


class drm_mlc_4qam_bc(gr.hier_block2):

    def __init__(self, vlen_in=1, vlen_out=1, n_tailbits=1, denom_mother_code_rate=1, map_tab=0, interl_seq=(0,1), bits_per_symbol=1, pp=0, pp_tail=0, gen_poly=0):
        gr.hier_block2.__init__(
            self, "MLC 4QAM",
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
        self.map_tab = map_tab
        self.interl_seq = interl_seq
        self.bits_per_symbol = bits_per_symbol
        self.pp = pp
        self.pp_tail = pp_tail
        self.gen_poly = gen_poly

        ##################################################
        # Blocks
        ##################################################
        self.trellis_encoder_xx_0 =  trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0, 0) if False else trellis.encoder_bb(trellis.fsm(1, denom_mother_code_rate, gen_poly), 0) 
        self.drm_qam_map_bc_0 = drm.qam_map_bc(map_tab, bits_per_symbol, vlen_out, 1)
        self.drm_punct_bb_0 = drm.punct_bb(pp, pp_tail, (vlen_in + n_tailbits) * denom_mother_code_rate, vlen_out * 2, n_tailbits * denom_mother_code_rate)
        self.drm_interleaver_bb_0 = drm.interleaver_bb(((interl_seq)))
        self.blocks_unpack_k_bits_bb_0 = blocks.unpack_k_bits_bb(denom_mother_code_rate)
        self.add_tailbits_bb_0 = drm.add_tailbits_bb(vlen_in, n_tailbits)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.add_tailbits_bb_0, 0), (self.trellis_encoder_xx_0, 0))    
        self.connect((self.blocks_unpack_k_bits_bb_0, 0), (self.drm_punct_bb_0, 0))    
        self.connect((self.drm_interleaver_bb_0, 0), (self.drm_qam_map_bc_0, 0))    
        self.connect((self.drm_punct_bb_0, 0), (self.drm_interleaver_bb_0, 0))    
        self.connect((self.drm_qam_map_bc_0, 0), (self, 0))    
        self.connect((self, 0), (self.add_tailbits_bb_0, 0))    
        self.connect((self.trellis_encoder_xx_0, 0), (self.blocks_unpack_k_bits_bb_0, 0))    

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
        self.trellis_encoder_xx_0.set_FSM(trellis.fsm(1, self.denom_mother_code_rate, self.gen_poly))

    def get_map_tab(self):
        return self.map_tab

    def set_map_tab(self, map_tab):
        self.map_tab = map_tab

    def get_interl_seq(self):
        return self.interl_seq

    def set_interl_seq(self, interl_seq):
        self.interl_seq = interl_seq

    def get_bits_per_symbol(self):
        return self.bits_per_symbol

    def set_bits_per_symbol(self, bits_per_symbol):
        self.bits_per_symbol = bits_per_symbol

    def get_pp(self):
        return self.pp

    def set_pp(self, pp):
        self.pp = pp

    def get_pp_tail(self):
        return self.pp_tail

    def set_pp_tail(self, pp_tail):
        self.pp_tail = pp_tail

    def get_gen_poly(self):
        return self.gen_poly

    def set_gen_poly(self, gen_poly):
        self.gen_poly = gen_poly
        self.trellis_encoder_xx_0.set_FSM(trellis.fsm(1, self.denom_mother_code_rate, self.gen_poly))
