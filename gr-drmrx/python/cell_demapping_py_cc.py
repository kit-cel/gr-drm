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
from gnuradio import gr

class cell_demapping_py_cc(gr.basic_block):
    """
    Demultiplexing of the logical channels (MSC, SDC, FAC). 
    NOTE: only supports RM B, SO 3
    """
    def __init__(self, rx):
        gr.basic_block.__init__(self,
            name="cell_demapping_py_cc",
            in_sig=[np.complex64],
            out_sig=[np.complex64, np.complex64, np.complex64])
            
        self.rx = rx
        self.p = rx.p()
        self.SO = self.p.SO_3()
        self.RM = self.p.RM_B()
        self.nfft = self.p.nfft()[self.RM]
        self.k_min = self.p.k_min()[self.RM][self.SO]
        self.k_max = self.p.k_max()[self.RM][self.SO]
        self.sym_ctr = 0 # counter to keep the number of the current frame
        self.nsym_frame = 15 # number of symbols per frame
        self.nframes = 3 # number of frames per superframe
#        self.ncell_msc = self.p.ncells_msc()[self.RM][self.SO]
#        self.ncells_sdc = self.p.ncells_sdc()[self.RM][self.SO]
#        self.ncells_fac = self.p.ncells_fac()[self.RM][self.SO]
        # these flags are inserted in channel_pos to indicate to which channel the cell belongs 
        self.time_pil_pos = self.p.time_pil_pos()[self.RM]
        self.freq_pil_pos = self.p.freq_pil_pos()[self.RM]
        self.gain_pil_pos = self.p.gain_pil_pos()[self.RM][self.SO]
        self.fac_pos = self.p.fac_pos()[self.RM]
        self.flag_drop = 0
        self.flag_msc = 1
        self.flag_sdc = 2
        self.flag_fac = 3
        self.channel_pos = self.calc_channel_positions()
        self.fac_cells = []
        self.sdc_cells = []
        self.msc_cells = []
        self.sym_ctr = 0 # keep track of current position in the superframe
    
        self.set_output_multiple(self.nfft) # this is way too much...
    
    def mark_pilots(self, mat): # RM B specific! works on an entire superframe!
        # time pilots
        for i in self.time_pil_pos:
            for n in [0, self.nsym_frame, 2*self.nsym_frame]: # time pilots occur in every 15th symbol in RM B
                if i-self.k_min >= 0:
                    mat[i-self.k_min,n] = self.flag_drop # pilots are dropped, offset is needed because pilot positions are arranged around DC
                elif self.SO < 3:
                    print "cell_demapping: shifted carrier index <0. something goes wrong here!"
        
        # frequency pilots        
        for i in self.freq_pil_pos:
            if i-self.k_min >= 0:
                mat[i-self.k_min][:] = self.flag_drop
            elif self.SO < 3:
                print "cell_demapping: shifted carrier index <0. something goes wrong here!"     
        
        # gain pilots NOTE: gain_pilot_pos is only defined for one frame, so use modulo to wrap
        for n in range(self.nsym_frame*self.nframes):
            for i in self.gain_pil_pos[n%self.nsym_frame]:
                if i-self.k_min >= 0:
                    mat[i-self.k_min,n] = self.flag_drop
                elif self.SO < 3:
                    print "cell_demapping: shifted carrier index <0. something goes wrong here!"

    def mark_unused(self, mat): # there are unused carriers, mostly DC
        if self.k_min <= 0:
            mat[-self.k_min][:] = self.flag_drop
            
    def mark_sdc(self, mat): # RM B SO 3 specific! marks SDC cells on the grid
        # There are 322 SDC cells for RM B and SO 3, located in the first 2 symbols of each super frame
        for n in range(2): # to generalize, just replace 2 with the correct number (see standard)
            for i in range(len(mat[:,n])):
                if mat[i,n] == -1: # cell is not marked yet
                    mat[i,n] = self.flag_sdc
           
    def mark_fac(self, mat): # again, RM B SO 3 specific. mark FAC cells on the grid
        for i in range(len(self.fac_pos)):
            for k in range(self.nframes):
                if self.fac_pos[i][1] - self.k_min >=0:
                    mat[ self.fac_pos[i][1] - self.k_min ][ self.fac_pos[i][0] + k*self.nsym_frame] = self.flag_fac
    
    def mark_msc(self, mat): # MSC cells are all the unflagged cells left
        for n in range(mat.shape[1]): # symbols
            for i in range(mat.shape[0]): # carrier
                if mat[i][n] == -1:
                    mat[i][n] = self.flag_msc
                    
    def calc_channel_positions(self):
        cell_grid = np.zeros((self.k_max - self.k_min + 1, self.nsym_frame * self.nframes)) - 1  # -1 to indicate still uncategorized positions
        self.mark_pilots(cell_grid)
        self.mark_unused(cell_grid)        
        self.mark_sdc(cell_grid)
        self.mark_fac(cell_grid) 
        self.mark_msc(cell_grid)      
        return cell_grid
    
    def extract_channels(self, sym):
        for i in range(self.channel_pos.shape[0]):
            if self.channel_pos[i][self.sym_ctr] == self.flag_msc:
                self.msc_cells.append(sym[self.nfft/2+self.k_min+i])
            elif self.channel_pos[i][self.sym_ctr] == self.flag_sdc:
                self.sdc_cells.append(sym[self.nfft/2+self.k_min+i])
            elif self.channel_pos[i][self.sym_ctr] == self.flag_fac:
                self.fac_cells.append(sym[self.nfft/2+self.k_min+i])
                
    def forecast(self, noutput_items, ninput_items_required):
        #setup size of input_items[i] for work call
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = self.nfft

    def general_work(self, input_items, output_items):
        in0 = input_items[0]
        self.extract_channels(in0[:self.nfft])
        output_items[0][:len(self.msc_cells)] = self.msc_cells[:]
        self.produce(0, len(self.msc_cells))
        output_items[1][:len(self.sdc_cells)] = self.sdc_cells[:]
        self.produce(1, len(self.sdc_cells))
        output_items[2][:len(self.fac_cells)] = self.fac_cells[:]
        self.produce(2, len(self.fac_cells))
        
        self.consume_each(self.nfft)
        self.fac_cells = []
        self.sdc_cells = []
        self.msc_cells = []
        self.sym_ctr = (self.sym_ctr + 1)%(self.nframes*self.nsym_frame)
        return -2 # return value for WORK_CALLED_PRODUCE
