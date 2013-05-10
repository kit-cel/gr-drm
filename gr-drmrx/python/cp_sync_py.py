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
import cmath
from gnuradio import gr
import pylab as pl

# custom implementation of numpy.corrcoef that works with complex arrays
def complex_corrcoef(x, y=None, rowvar=1, bias=0, ddof=None):
    c = complex_cov(x, y, rowvar, bias, ddof)
    try:
        d = np.diag(c)
    except ValueError: # scalar covariance
        return 1
    return c/np.sqrt(np.multiply.outer(d,d))
    
def complex_cov(m, y=None, rowvar=1, bias=0, ddof=None):
    # Check inputs
    if ddof is not None and ddof != int(ddof):
        raise ValueError("ddof must be integer")
     
    X = np.array(m, ndmin=2)#, dtype=float)
    if X.shape[0] == 1:
        rowvar = 1
    if rowvar:
        axis = 0
        tup = (slice(None),np.newaxis)
    else:
        axis = 1
        tup = (np.newaxis, slice(None))
     
     
    if y is not None:
        y = np.array(y, copy=False, ndmin=2)#, dtype=float)
        X = np.concatenate((X,y), axis)
     
    X -= X.mean(axis=1-axis)[tup]
    if rowvar:
        N = X.shape[1]
    else:
        N = X.shape[0]
     
    if ddof is None:
        if bias == 0:
            ddof = 1
        else:
            ddof = 0
    fact = float(N - ddof)
     
    if not rowvar:
        return (np.dot(X.T, X.conj()) / fact).squeeze()
    else:
        return (np.dot(X, X.T.conj()) / fact).squeeze()
    

 
	
class cp_sync_py(gr.basic_block):
    """
    Perform time synchronization and fractional frequency offset correction by cylic prefix correlation.
    Symbol detection is based on a threshold decision (max. correlation value).
    Output is frequency corrected and starts with the first sample of a symbol with its cylic prefix removed.
    TODO: maybe divide this block up in timing offset detection -> fractional frequency offset correction -> cylic prefix removal
    
    DEBUG STATUS:
        -- works with a perfect baseband signal (delta_t == f_off == 0)
        -- works (always?) with a delayed signal
        -- works sometimes with a f_off=0
        -- does sometimes not work with the same(!?) signal out of the channel_simulator
        -- never works with frequency offset, although the first estimation is always correct
        -- always works for the first run (if the freq offset is not too big), then results deviate quickly
        
        
    """
    
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="cp_sync_py",
            in_sig=[np.complex64],
            out_sig=[np.complex64])
        self.rx = rx
        self.FS = rx.p().FS()
        self.nsamp_ts = self.rx.p().nsamp_Ts()
        self.nsamp_tu = self.rx.p().nsamp_Tu()
        self.nsamp_tg = self.rx.p().nsamp_Tg()
        self.tracking_mode = False # switches between acquisition and tracking mode
        self.prev_freq_offset = 0
        self.remaining_freq_offset = 0
        self.cont_phase_offset = 0
        self.corr_vec = [np.zeros(( self.nsamp_ts[0], 1), dtype=np.complex64), np.zeros(( self.nsamp_ts[1], 1), \
            dtype=np.complex64), np.zeros(( self.nsamp_ts[2], 1), dtype=np.complex64), np.zeros(( self.nsamp_ts[3], 1), dtype=np.complex64)]                           
        self.corr_vec_abs = [np.zeros(( self.nsamp_ts[0], 1)), np.zeros(( self.nsamp_ts[1], 1)), \
            np.zeros(( self.nsamp_ts[2], 1)), np.zeros(( self.nsamp_ts[3], 1))]              
        self.timing_offset = [-1, 0] # (peak index, peak value)
        self.timing_backoff = 10 # backoff to prevent late sync
        self.corr_threshold = 0.8
        
        self.message_port_register_out(gr.pmt.pmt_string_to_symbol("reset"))
        self.set_output_multiple(max(self.nsamp_ts))
        
    def forecast(self, noutput_items, ninput_items_required):
        # we need more samples in the input buffer than we consume because of the correlation
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = 2*max(self.nsamp_ts)
            
    def find_tags(self, tracking):
        if not(tracking): # if in tracking mode, this can be skipped
            nread = self.nitems_read(0)
            ninput_items = []
            if self.rx.RM() != self.rx.p().RM_NONE():
                ninput_items = self.nsamp_ts[self.rx.RM()]
            else:
                ninput_items = max(self.nsamp_ts)
                
            tags = self.get_tags_in_range(0, nread, nread+ninput_items, gr.pmt.pmt_string_to_symbol("coarse_freq_offset"))
            
            if len(tags) > 0:                
                self.prev_freq_offset = gr.pmt.pmt_to_long(tags[0].value) # start value for closed loop estimation
                print "cp_sync_py: found stream tag, new offset is", self.prev_freq_offset
                return True
            else:
                print "cp_sync_py: looking for a stream tag, but none found"
                return False
        else:
            return True        
        
                
    def calc_correlation(self, vec, tracking):
        if tracking: # reduced complexity because the RM is already known
            for k in range(self.nsamp_ts[self.rx.RM()]):
                self.corr_vec[self.rx.RM()][k] = complex_corrcoef(vec[k:self.nsamp_tg[self.rx.RM()]-1+k],vec[self.nsamp_tu[self.rx.RM()]+k:self.nsamp_ts[self.rx.RM()]-1+k])[0][1]
                self.corr_vec_abs[self.rx.RM()][:] = abs(self.corr_vec[self.rx.RM()][:])
                
        else: # acquisition mode, try all configurations (RMs)
           for i in range(4): # modes A, B, C, D
                for k in range(self.nsamp_ts[i]):
                    self.corr_vec[i][k] = complex_corrcoef(vec[k:self.nsamp_tg[i]-1+k], vec[self.nsamp_tu[i]+k:self.nsamp_ts[i]-1+k])[0][1]
                    self.corr_vec_abs[i][:] = abs(self.corr_vec[i][:]) 
        
    def find_RM(self, tracking):
        if not(tracking): # if already in tracking mode, this can be skipped
            # use maximum of the correlation coefficients' magnitude to determine the RM
            max_val = np.zeros((4,))
            for i in range(len(max_val)):
                max_val[i] = max(self.corr_vec_abs[i][:])
            print "cp_sync_py: max correlation values are", list(max_val)
                
            # threshold decision, if correlation value is high enough (normalized between 0 and 1)
            if max(max_val) > self.corr_threshold:
                # find robustness mode
                rm = 0 # start value 
                for i in range(len(max_val)-1):
                    if max_val[i+1] > max_val[rm]:
                        rm = i+1
                self.rx.set_RM(rm)
                self.tracking_mode = True
                print "cp_sync_py: detected RM", self.rx.RM()
                return True
            else:
                print "cp_sync_py: no RM detected"
                self.reset_all()
                return False
        else: # if already tracking, just check if correlation peak is still above the threshold
            if max(self.corr_vec_abs[self.rx.RM()][:]) > self.corr_threshold:
                return True
            else:
                print "cp_sync_py: lost sync. reset all."
                self.reset_all()
                return False
                
    def calc_timing_offset(self):
        self.timing_offset[0] = np.argmax(self.corr_vec_abs[self.rx.RM()][:])
        self.timing_offset[1] = self.corr_vec_abs[self.rx.RM()][self.timing_offset[0]]
        print "cp_sync_py: timing offset", self.timing_offset[0], "samples, correlation coefficient", self.timing_offset[1]
        
    def prevent_wraparound(self):
        if self.timing_offset[0] < 0.1*self.nsamp_ts[self.rx.RM()] or self.timing_offset[0] > 0.9*self.nsamp_ts[self.rx.RM()]:
            # consume half a symbol and return 0
            print "cp_sync_py: preventing wrap-arounds by consuming", self.nsamp_ts[self.rx.RM()]/2
            self.reset_block()
            return True
        else:
            return False
            
    def calc_frac_freq_offset(self):
        #calculate average phase difference between the two intervals and determine the fractional frequency offset
        phase_diff = cmath.phase(complex(self.corr_vec[self.rx.RM()][self.timing_offset[0]].real, self.corr_vec[self.rx.RM()][self.timing_offset[0]].imag))
        time_diff = float(self.nsamp_tu[self.rx.RM()]) / self.FS
        self.remaining_freq_offset = -phase_diff / (2*cmath.pi * time_diff)
        
    def update_prev_freq_offset(self): #TODO: maybe this could be more intelligent (e.g. with averaging)
        print "cp_sync_py: updating prev_freq_offset from", self.prev_freq_offset, "to", self.prev_freq_offset + self.remaining_freq_offset
        self.prev_freq_offset += self.remaining_freq_offset
        
                            
    def correct_freq_offset(self, vec, f_off):
        arg = -2j*np.pi*f_off/self.FS
        for i in range(len(vec)):
            vec[i] *= np.exp(arg*i + self.cont_phase_offset)
        self.cont_phase_offset = (arg*len(vec) + self.cont_phase_offset) % 2*np.pi # NCO must be continuous in phase
        return vec
           
    def remove_cp(self, vec):
        # remove cylic prefix. to prevent late synchronization, a timing backoff is applied. this
        # results in a constant phase shift.
        symbol_start = self.nsamp_tg[self.rx.RM()] - self.timing_backoff
        return vec[symbol_start : symbol_start + self.nsamp_tu[self.rx.RM()]]
        
    def reset_all(self):
        print "cp_sync_py: reset all"
        # reset block variables
        self.timing_offset = [-1, 0]
        self.prev_freq_offset = 0
        self.remaining_freq_offset = 0
        self.cont_phase_offset = 0
        self.tracking_mode = False
        # reset the RM
        self.rx.set_RM(self.rx.p().RM_NONE())
        # send reset to the upstream blocks
        self.message_port_pub(gr.pmt.pmt_string_to_symbol("reset"), gr.pmt.pmt_from_bool(True))
        
    def reset_block(self):
        print "cp_sync_py: reset block"
        # reset block variables
        self.timing_offset = [-1, 0]
        self.tracking_mode = False
        
    def debug_plot(self):
        pl.subplot(4,1,1)
        pl.plot(self.corr_vec_abs[0][:])
        pl.ylabel("RM A")
        pl.subplot(4,1,2)
        pl.plot(self.corr_vec_abs[1][:])
        pl.ylabel("RM B")
        pl.subplot(4,1,3)
        pl.plot(self.corr_vec_abs[2][:])
        pl.ylabel("RM C")
        pl.subplot(4,1,4)
        pl.plot(self.corr_vec_abs[3][:])
        pl.ylabel("RM D")
        pl.show()
        
    def general_work(self, input_items, output_items):
        in0 = input_items[0]   
        out = output_items[0]
        
        # drop, if not enough samples in input buffer
        if len(in0) < 2*max(self.nsamp_ts):
            print "cp_sync_py: not enough samples, skip work()"
            self.sync_step_counter = 0
            return 0            
        
        # look for tags with coarse frequency offset if not in tracking mode. return if tag is needed but not found
        if not(self.find_tags(self.tracking_mode)):
            # no need to reset because nothing happened so far
            print "cp_sync_py: no tag found. consuming", max(self.nsamp_ts)
            self.consume_each(max(self.nsamp_ts))
            return 0
                 
        in_vec = []
        if self.tracking_mode:
            in_vec[:] = in0[:2*self.nsamp_ts[self.rx.RM()]]
        else:
            in_vec[:] = in0[:2*max(self.nsamp_ts)]
        
        # correct last known frequency offset
        in_vec[:] = self.correct_freq_offset(in_vec, self.prev_freq_offset)
        
       # find current timing offset
        self.calc_correlation(in_vec, self.tracking_mode)
        #self.debug_plot()
        
        # determine the robustness mode
        if not(self.find_RM(self.tracking_mode)): # true if RM was found (or already in tracking mode)
            self.consume_each(max(self.nsamp_ts))
            return 0
            
        # find the timing offset 
        self.calc_timing_offset()
        
        # if the timing offset is close to zero, consume half a symbol to prevent a wrap-around
        if self.prevent_wraparound():
            self.consume_each(self.nsamp_ts[self.rx.RM()]/2)
            return 0
        
        # ... and the remaining fractional frequency offset
        self.calc_frac_freq_offset()
        
        # calculate new prev_freq_offset
        self.update_prev_freq_offset()
        
        # correct this offset
        #in_vec = self.correct_freq_offset(in_vec[self.timing_offset[0] : self.timing_offset[0] + self.nsamp_ts[self.rx.RM()]], self.remaining_freq_offset)
        #self.debug_plot()
        
        # remove cyclic prefix and write the aligned symbol in the output buffer
        out[:self.nsamp_tu[self.rx.RM()]] = self.remove_cp(in_vec)
        
        # consume and return 
        self.consume_each(self.nsamp_ts[self.rx.RM()])
        return self.nsamp_tu[self.rx.RM()]
