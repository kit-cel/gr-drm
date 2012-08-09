/*
 * drm_transm_params.h
 *
 *  Created on: 20.06.2012
 *      Author: felixwunsch
 */

#ifndef DRM_TRANSM_PARAMS_H_
#define DRM_TRANSM_PARAMS_H_

#include "drm_config.h"
#include "drm_params.h"

class transm_params
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

	transm_params(unsigned short RM, 
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
				  std::string text_message);
	~transm_params(){};
};

#endif /* DRM_TRANSM_PARAMS_H_ */
