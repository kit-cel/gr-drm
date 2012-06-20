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
	config* cfg();
	ofdm_params* ofdm();
	msc_params* msc();
	sdc_params* sdc();
	fac_params* fac();

	void init(config* c, ofdm_params* o, msc_params* m, sdc_params* s, fac_params* f);

	transm_params();
	~transm_params(){};
};

#endif /* DRM_TRANSM_PARAMS_H_ */
