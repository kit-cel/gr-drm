/* -*- c++ -*- */
/* 
 * Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
 * Author: Felix Wunsch
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
 
#ifndef DRM_CONFIG_H_
#define DRM_CONFIG_H_

#include "drm_tables.h"
#include "drm_global_constants.h"
#include <iostream>

/* class holding the initial (user defined) parameters from which the others are derived */
class config
{
	unsigned short d_RM; // robustness mode (0-4 correspond to A-E)
	unsigned short d_SO; // spectrum occupancy (0-5)
	bool d_UEP; // 0: EEP, 1: UEP (only MSC, SDC & FAC are restricted to EEP and SM)
	unsigned int d_n_bytes_A; // number of higher protected bytes (if UEP is used)
	bool d_text; // 0: text message not used, 1: text message used
	unsigned short d_msc_mapping; // 0: 4-QAM SM (only RM E), 1: 16-QAM SM, 2: 64-QAM SM, 3: 64-QAM HMsym, 4: 64-QAM HMmix
	unsigned short d_msc_prot_level_1; // Protection level for higher protected part (if UEP is used)
	unsigned short d_msc_prot_level_2; // Protection level for lower protected part (or for EEP)
	unsigned short d_sdc_mapping; // 1: 4-QAM, 0: 16-QAM
	unsigned short d_sdc_prot_level; // (only for RM E) 0: 0.5, 1: 0.25
	bool d_long_interl; // 0: short interleaving, 1: long interleaving
	unsigned int d_audio_samp_rate; // Input audio sampling rate
	std::string d_station_label; // station label
	std::string d_text_message; // text message

	tables* d_ptables; // pointer to tables needed for init

public:
	/* accessor methods */
	unsigned short RM();
	unsigned short SO();
	bool UEP();
	unsigned int n_bytes_A(); // TODO: extend check_args()
	bool text();
	unsigned short msc_mapping();
	unsigned short msc_prot_level_1(); // TODO: extend check_args()
	unsigned short msc_prot_level_2();
	unsigned short sdc_mapping();
	unsigned short sdc_prot_level();
	bool long_interl();
	unsigned int audio_samp_rate();
	std::string station_label();
	std::string text_message();
	tables* ptables();

	config();
	~config(){};

	void init(tables* ptr_tables,
			  unsigned short RM, 
			  unsigned short SO, 
			  bool UEP,
			  unsigned int n_bytes_A, 
			  bool text,
			  unsigned short msc_mapping,
			  unsigned short msc_prot_level_1, 
			  unsigned short msc_prot_level_2,
			  unsigned short sdc_mapping,
			  unsigned short sdc_prot_level,
			  bool long_interl,
			  unsigned int audio_samp_rate,
			  std::string station_label,
			  std::string text_message); // set member variables
			  
	void check_arguments(); // some range and plausibility checks
};

#endif
