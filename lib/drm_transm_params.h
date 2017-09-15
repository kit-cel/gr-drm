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
 
#ifndef DRM_TRANSM_PARAMS_H_
#define DRM_TRANSM_PARAMS_H_

#include <drm/api.h>
#include "drm_config.h"
#include "drm_params.h"

class DRM_API transm_params
{
	config* d_cfg;
	ofdm_params* d_ofdm;
	msc_params* d_msc;
	sdc_params* d_sdc;
	fac_params* d_fac;

public:
	config cfg();
	ofdm_params ofdm();
	msc_params msc();
	sdc_params sdc();
	fac_params fac();

	transm_params(   unsigned short RM=1, 
				     unsigned short SO=3, 
				     bool UEP=false,
				     unsigned int n_bytes_A=0, 
				     unsigned short msc_mapping=2,
				     unsigned short msc_prot_level_1=0, 
				     unsigned short msc_prot_level_2=0,
				     unsigned short sdc_mapping=1,
				     unsigned short sdc_prot_level=0,
				     bool long_interl=true,
				     unsigned int audio_samp_rate=12000,
				     std::string station_label="gr-drm",
	      			 std::string text_message="This is GNU Radio on DRM");
	~transm_params(){};
};

#endif /* DRM_TRANSM_PARAMS_H_ */
