#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: DRM Transmitter 1
# Author: Felix Wunsch
# Description: Unisversal DRM Transmitter
# Generated: Wed Feb 10 15:24:56 2016
##################################################

#import os
#import sys
#sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from gnuradio import analog
from gnuradio import audio
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import filter
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import drm
import time

class drm_transmitter(gr.top_block):

    def __init__(self,DRMParameters):
        gr.top_block.__init__(self, "DRM Transmitter 1")

        ##################################################
        # Variables
        ##################################################
        self.text_message = text_message = DRMParameters.text_msg
        self.station_label = station_label = DRMParameters.station_label
        self.msc_prot_level_2 = msc_prot_level_2 = 1
        self.long_interl = long_interl = True
        self.audio_sample_rate = audio_sample_rate = DRMParameters.audio_samp*1000
        self.SO = SO = DRMParameters.so
        self.RM = RM = DRMParameters.rm
        self.tp = tp = drm.transm_params(RM, SO, False, 0, DRMParameters.msc_mod, 0, msc_prot_level_2, DRMParameters.sdc_mod, 0, long_interl, audio_sample_rate, station_label, text_message)
        self.samp_rate = samp_rate = 48e3
        self.usrp_addr = DRMParameters.usrp_id
        self.output_name = DRMParameters.output_name
        self.center_freq = DRMParameters.center_freq*1e6
        self.audio_file = DRMParameters.audio_file

        ##################################################
        # Blocks
        ##################################################
        if DRMParameters.uhd_found:
            self.uhd_usrp_sink_0 = uhd.usrp_sink(
                ",".join((self.usrp_addr, "")),
                uhd.stream_args(
                    cpu_format="fc32",
                    channels=range(1),
                ),
            )
            self.uhd_usrp_sink_0.set_samp_rate(samp_rate * 250 / 48)
            self.uhd_usrp_sink_0.set_center_freq(self.center_freq, 0)
            self.uhd_usrp_sink_0.set_gain(0, 0)
            self.uhd_usrp_sink_0.set_antenna("TXA", 0)
            self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                    interpolation=250,
                    decimation=48,
                    taps=None,
                    fractional_bw=None,
            )
        self.fft_vxx_0 = fft.fft_vcc(tp.ofdm().nfft(), False, (), True, 1)
        self.drm_scrambler_bb_0_1 = drm.scrambler_bb(tp.sdc().L())
        self.drm_scrambler_bb_0_0 = drm.scrambler_bb(tp.fac().L())
        self.drm_scrambler_bb_0 = drm.scrambler_bb(tp.msc().L_MUX())
        self.drm_mlc_fac_bc = drm.make_mlc(
            channel_type="FAC",
            tp=tp
        )

        #SDC Configuration
        self.drm_mlc_sdc_bc= drm.make_mlc(
                channel_type="SDC",
                tp=tp
            )
            
        #MSC Configuration
        self.drm_mlc_msc_bc = drm.make_mlc(
                channel_type="MSC",
                tp=tp
            )
        self.drm_interleaver_cc_0 = drm.interleaver_cc((tp.msc().cell_interl_seq()), long_interl, drm.INTL_DEPTH_DRM)
        self.drm_generate_sdc_b_0 = drm.generate_sdc_b(tp)
        self.drm_generate_fac_b_0 = drm.generate_fac_b(tp)
        self.drm_audio_encoder_sb_0 = drm.audio_encoder_sb(tp)
        self.digital_ofdm_cyclic_prefixer_1 = digital.ofdm_cyclic_prefixer(tp.ofdm().nfft(), tp.ofdm().nfft()+tp.ofdm().nfft()*tp.ofdm().cp_ratio_enum()/tp.ofdm().cp_ratio_denom(), 0, "")
        self.cell_mapping_cc_0 = drm.cell_mapping_cc(tp, (tp.msc().N_MUX() * tp.ofdm().M_TF() * 8, tp.sdc().N() * 8, tp.fac().N() * tp.ofdm().M_TF() * 8))
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_float*1)
        if DRMParameters.pulse_audio:
            self.audio_source_1 = audio.source(audio_sample_rate, "", True)
        else:
            self.blocks_wavfile_source_0 = blocks.wavfile_source(self.audio_file, False)
        if DRMParameters.gen_output:
            self.blocks_wavfile_sink_0 = blocks.wavfile_sink(self.output_name, 1, 48000, 16)
        self.blocks_multiply_xx_0 = blocks.multiply_vcc(1)
        self.blocks_multiply_const_vxx_1 = blocks.multiply_const_vcc((7e-3, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((32768, ))
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, 7000, 1, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_multiply_xx_0, 1))    
        if DRMParameters.gen_output:
            self.connect((self.blocks_complex_to_real_0, 0), (self.blocks_wavfile_sink_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.drm_audio_encoder_sb_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_1, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.blocks_complex_to_real_0, 0))
        if DRMParameters.pulse_audio:
            self.connect((self.audio_source_1, 0), (self.blocks_multiply_const_vxx_0, 0))
        else:
            self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.cell_mapping_cc_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.digital_ofdm_cyclic_prefixer_1, 0), (self.blocks_multiply_const_vxx_1, 0))    
        self.connect((self.drm_audio_encoder_sb_0, 0), (self.drm_scrambler_bb_0, 0))    
        self.connect((self.drm_generate_fac_b_0, 0), (self.drm_scrambler_bb_0_0, 0))    
        self.connect((self.drm_generate_sdc_b_0, 0), (self.drm_scrambler_bb_0_1, 0))    
        self.connect((self.drm_interleaver_cc_0, 0), (self.cell_mapping_cc_0, 0))    
        self.connect((self.drm_mlc_msc_bc, 0), (self.drm_interleaver_cc_0, 0))
        self.connect((self.drm_mlc_sdc_bc, 0), (self.cell_mapping_cc_0, 1))
        self.connect((self.drm_mlc_fac_bc, 0), (self.cell_mapping_cc_0, 2))    
        self.connect((self.drm_scrambler_bb_0, 0), (self.drm_mlc_msc_bc, 0))
        self.connect((self.drm_scrambler_bb_0_0, 0), (self.drm_mlc_fac_bc, 0))    
        self.connect((self.drm_scrambler_bb_0_1, 0), (self.drm_mlc_sdc_bc, 0))
        self.connect((self.fft_vxx_0, 0), (self.digital_ofdm_cyclic_prefixer_1, 0))    
        if DRMParameters.uhd_found:
            self.connect((self.rational_resampler_xxx_0, 0), (self.uhd_usrp_sink_0, 0))
            self.connect((self.blocks_multiply_const_vxx_1, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.blocks_null_sink_0, 0))


    def get_text_message(self):
        return self.text_message

    def set_text_message(self, text_message):
        self.text_message = text_message
        self.set_tp(drm.transm_params(self.RM, self.SO, False, 0, False, 1, 0, self.msc_prot_level_2, 1, 0, self.long_interl, self.audio_sample_rate, self.station_label, self.text_message))

    def get_station_label(self):
        return self.station_label

    def set_station_label(self, station_label):
        self.station_label = station_label
        self.set_tp(drm.transm_params(self.RM, self.SO, False, 0, False, 1, 0, self.msc_prot_level_2, 1, 0, self.long_interl, self.audio_sample_rate, self.station_label, self.text_message))

    def get_msc_prot_level_2(self):
        return self.msc_prot_level_2

    def set_msc_prot_level_2(self, msc_prot_level_2):
        self.msc_prot_level_2 = msc_prot_level_2
        self.set_tp(drm.transm_params(self.RM, self.SO, False, 0, False, 1, 0, self.msc_prot_level_2, 1, 0, self.long_interl, self.audio_sample_rate, self.station_label, self.text_message))

    def get_long_interl(self):
        return self.long_interl

    def set_long_interl(self, long_interl):
        self.long_interl = long_interl
        self.set_tp(drm.transm_params(self.RM, self.SO, False, 0, False, 1, 0, self.msc_prot_level_2, 1, 0, self.long_interl, self.audio_sample_rate, self.station_label, self.text_message))

    def get_audio_sample_rate(self):
        return self.audio_sample_rate

    def set_audio_sample_rate(self, audio_sample_rate):
        self.audio_sample_rate = audio_sample_rate
        self.set_tp(drm.transm_params(self.RM, self.SO, False, 0, False, 1, 0, self.msc_prot_level_2, 1, 0, self.long_interl, self.audio_sample_rate, self.station_label, self.text_message))

    def get_SO(self):
        return self.SO

    def set_SO(self, SO):
        self.SO = SO
        self.set_tp(drm.transm_params(self.RM, self.SO, False, 0, False, 1, 0, self.msc_prot_level_2, 1, 0, self.long_interl, self.audio_sample_rate, self.station_label, self.text_message))

    def get_RM(self):
        return self.RM

    def set_RM(self, RM):
        self.RM = RM
        self.set_tp(drm.transm_params(self.RM, self.SO, False, 0, False, 1, 0, self.msc_prot_level_2, 1, 0, self.long_interl, self.audio_sample_rate, self.station_label, self.text_message))

    def get_tp(self):
        return self.tp

    def set_tp(self, tp):
        self.tp = tp
        self.drm_mlc_16qam_bc_0.set_M_total(self.tp.msc().M_total())
        self.drm_mlc_16qam_bc_0.set_bits_per_symbol(self.tp.msc().mod_order())
        self.drm_mlc_16qam_bc_0.set_interl_seq_0_2(self.tp.msc().bit_interl_seq_0_2())
        self.drm_mlc_16qam_bc_0.set_interl_seq_1_2(self.tp.msc().bit_interl_seq_1_2())
        self.drm_mlc_16qam_bc_0.set_map_tab(self.tp.cfg().ptables().d_QAM16)
        self.drm_mlc_16qam_bc_0.set_part_len_bot(self.tp.msc().M_total()[1])
        self.drm_mlc_16qam_bc_0.set_part_len_top(self.tp.msc().M_total()[0])
        self.drm_mlc_16qam_bc_0.set_pp_0(self.tp.msc().punct_pat_0_2())
        self.drm_mlc_16qam_bc_0.set_pp_0_tail(self.tp.msc().punct_pat_tail_0_2())
        self.drm_mlc_16qam_bc_0.set_pp_1(self.tp.msc().punct_pat_1_2())
        self.drm_mlc_16qam_bc_0.set_pp_1_tail(self.tp.msc().punct_pat_tail_1_2())
        self.drm_mlc_16qam_bc_0.set_vlen_in(self.tp.msc().L_MUX())
        self.drm_mlc_16qam_bc_0.set_vlen_out(self.tp.msc().N_MUX())
        self.drm_mlc_4qam_bc_0.set_bits_per_symbol(self.tp.sdc().mod_order())
        self.drm_mlc_4qam_bc_0.set_interl_seq(self.tp.sdc().bit_interl_seq_0_2())
        self.drm_mlc_4qam_bc_0.set_map_tab(self.tp.cfg().ptables().d_QAM4)
        self.drm_mlc_4qam_bc_0.set_pp(self.tp.sdc().punct_pat_0())
        self.drm_mlc_4qam_bc_0.set_pp_tail(self.tp.sdc().punct_pat_tail_0())
        self.drm_mlc_4qam_bc_0.set_vlen_in(self.tp.sdc().L())
        self.drm_mlc_4qam_bc_0.set_vlen_out(self.tp.sdc().N())
        self.drm_mlc_4qam_bc_0_0.set_bits_per_symbol(self.tp.fac().mod_order())
        self.drm_mlc_4qam_bc_0_0.set_interl_seq(self.tp.fac().bit_interl_seq_0_2())
        self.drm_mlc_4qam_bc_0_0.set_map_tab(self.tp.cfg().ptables().d_QAM4)
        self.drm_mlc_4qam_bc_0_0.set_pp(self.tp.fac().punct_pat_0())
        self.drm_mlc_4qam_bc_0_0.set_pp_tail(self.tp.fac().punct_pat_0())
        self.drm_mlc_4qam_bc_0_0.set_vlen_in(self.tp.fac().L())
        self.drm_mlc_4qam_bc_0_0.set_vlen_out(self.tp.fac().N())

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate * 250 / 48)



