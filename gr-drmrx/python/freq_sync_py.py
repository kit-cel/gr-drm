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

import numpy as np
import pylab as pl
from gnuradio import gr

        
class freq_sync_py(gr.basic_block):
    """
    Perform frequency synchronization by correlating with the three continuous sine pilots. 
    FIXME: No-Signal detection does not work reliably. Maybe use difference between two highest peaks or cancel DC offset.
    TODO: Idea: This block only estimates the rough offset, attaches a tag but doesn't correct it. cp_sync then implements a closed loop with this estimate as starting point.
    """
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="freq_sync_py",
            in_sig=[np.complex64],
            out_sig=[np.complex64])

        self.rx = rx
        self.FS = 48000
        self.nfft = 3200 # 3200 points result in a spectral resolution of 15 Hz --> pilot frequencies can be represented without error   
        self.nsamp_sym = 1280  
        self.delta_f = self.FS / self.nfft
        self.f_pil_index = np.array([np.round(750/self.delta_f), np.round(2250/self.delta_f), np.round(3000/self.delta_f)], dtype=int)
        self.buf_ctr = 0
        self.buf_ctr_max = 5 # arbitrary value, longer -> better estimation, shorter -> faster resync
        self.buffer_filled = False
        self.fft_vec = np.zeros((self.buf_ctr_max, self.nfft))    
        self.fft_vec_avg = np.zeros((1, self.nfft))
        self.corr_vec = np.zeros((self.nfft, ))
        self.peak_avg_ratio = 0
        self.tracking_mode = False # 'True' basically deactivates this block because tracking is done in cp_sync
        self.cur_freq_offset = np.NaN        
        self.freq_offset_hist = []
        self.freq_offset_hist_len = 3 # arbitrary value
        self.freq_offset = []
        
        self.message_port_register_in(gr.pmt.pmt_intern('reset_in'))
        self.set_msg_handler(gr.pmt.pmt_intern('reset_in'), self.handle_msg)
        
        self.set_output_multiple(self.nfft) # set minimum buffer output size
        
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = self.nfft
            
    def handle_msg(self, msg):
        print "freq_sync_py: got message. reset."
        self.reset()
            
    def calc_avg_fft(self, in0):
        fft_tmp = np.fft.fft(in0[0:self.nfft], self.nfft)
        fft_tmp = np.fft.fftshift(fft_tmp)
        fft_tmp *= np.conj(fft_tmp) # square it for power spectrum density
        self.fft_vec[self.buf_ctr][:] = abs(fft_tmp)
        self.buf_ctr = (self.buf_ctr + 1) % self.buf_ctr_max
        # average FFT to get a better estimate of the spectrum
        self.fft_vec_avg = np.mean(self.fft_vec, 0)
        
    def pilot_corr(self):
        # add the FFT bins corresponding to the pilot positions for each shift and take the magnitude
        # the if conditions prevent out-of-bounds errors
        for i in range(self.nfft): 
            if i + self.f_pil_index[2] < self.nfft:
                self.corr_vec[i] = abs(self.fft_vec_avg[i+self.f_pil_index[0]] \
                + self.fft_vec_avg[i+self.f_pil_index[1]] \
                + self.fft_vec_avg[i+self.f_pil_index[2]])
            elif i + self.f_pil_index[1] < self.nfft:
                self.corr_vec[i] = abs(self.fft_vec_avg[i+self.f_pil_index[0]] \
                + self.fft_vec_avg[i+self.f_pil_index[1]])
            elif i + self.f_pil_index[0] < self.nfft:
                self.corr_vec[i] = abs(self.fft_vec_avg[i+self.f_pil_index[0]])
            else:
                self.corr_vec[i] = 0
                   
    def find_freq_offset(self):
        # find maximum and corresponding value
        peak_index = 0;
        peak_val = 0;
        for k in range(len(self.corr_vec)):
    		if self.corr_vec[k] > peak_val: 
    			peak_val = self.corr_vec[k]
    			peak_index = k	
       
        # wrap indices around because of fftshift
        peak_index -= self.nfft/2            
        self.cur_freq_offset = peak_index * self.delta_f
        
    def append_offset_to_hist(self):
        #append frequency offset to history if its equal to the existing estimates. otherwise, empty the list and set the current offset as first element
        if len(self.freq_offset_hist) == 0:
            self.freq_offset_hist.append(self.cur_freq_offset)
        elif len(self.freq_offset_hist) < self.freq_offset_hist_len:
            is_equal = True
            for i in range(len(self.freq_offset_hist)):
                if self.cur_freq_offset != self.freq_offset_hist[i]:
                    is_equal = False
            if is_equal:
                self.freq_offset_hist.append(self.cur_freq_offset)
            else:
                self.freq_offset_hist = list(self.cur_freq_offset)
        else:
            print "freq_sync_py: this should not happen!"
    
            
    def attach_tag(self):
        offset = self.nitems_written(0)
        key = gr.pmt.pmt_string_to_symbol("coarse_freq_offset")
        print "attach_tag(): freq_offset",self.freq_offset
        value = gr.pmt.pmt_from_long(self.freq_offset)
        self.add_item_tag(0, offset + self.nfft, key, value)
            
    def reset(self):
        self.buf_ctr = 0
        self.buffer_filled = False
        self.fft_vec = np.zeros((self.buf_ctr_max, self.nfft))    
        self.fft_vec_avg = np.zeros((1, self.nfft))
        self.corr_vec = np.zeros((self.nfft, ))
        self.peak_avg_ratio = 0
        self.tracking_mode = False
        self.freq_offset_hist = []
        self.freq_offset = []
        self.cur_freq_offset = np.NaN        
    
    def debug_plot(self):
        pl.subplot(311)
        pl.plot(self.corr_vec/max(self.corr_vec))
        pl.title("freq_sync_py")
        pl.ylabel("correlation (linear)")
        pl.subplot(312)
        pl.plot(pl.log10(self.fft_vec_avg))
        pl.ylabel("avg fft (db)")
        pl.subplot(313)
        pl.plot(pl.log10(self.fft_vec[abs(self.buf_ctr-1)]))
        pl.ylabel("current fft (db)")
        
        pl.show()
        
    def general_work(self, input_items, output_items):
        in0 = input_items[0]
        out = output_items[0]
        
        if len(in0) < self.nfft:
            print "freq_sync_py: not enough samples, skip work()"
            return 0
        
        if self.tracking_mode:
            min_buf_len = min((len(in0), len(out))) # return as many samples as possible
            self.consume_each(min_buf_len)
            out[:min_buf_len] = in0[:min_buf_len]
            return min_buf_len
            
        # compute averaged FFT of input signal
        self.calc_avg_fft(in0[:self.nfft])
        if self.buf_ctr >= self.buf_ctr_max - 1: # the flag is never set back to false once the buffer is filled
            self.buffer_filled = True
                    
        if self.buffer_filled:
            #self.debug_plot()
            self.consume_each(self.nfft)
            
            self.pilot_corr()  
            self.find_freq_offset()
            self.append_offset_to_hist()
            if len(self.freq_offset_hist) >= self.freq_offset_hist_len:
                self.tracking_mode = True
                self.freq_offset = self.freq_offset_hist[0] 
                print "freq_sync_py: coarse frequency offset estimation is", self.freq_offset, "Hz"
                
            if self.freq_offset != []:
                out[:self.nfft] = in0[:self.nfft]
                self.attach_tag()
                return self.nfft
                
            else:
                return 0
           
        else: # buffer not yet filled
            print "freq_sync_py: filling buffer", self.buf_ctr, "/", self.buf_ctr_max
            self.consume_each(self.nfft)
            return 0
        
        
        
