/*
 * drm_init_params.h
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 *
 *  Description: 	Some classes holding the parameters for transmission.
 *  				The control_channel_params and msc_params include
 *  				everything for the binary domain, the ofdm_params
 *  				class yields the needed variables for the complex
 *  				domain.
 *
 *  				These classes shall be instantiated at the
 *  				beginning. The init() routines determine all
 *  				important parameters. At the moment, they are not
 *  				thought to be changeable during transmission.
 */

#ifndef DRM_INIT_PARAMS_H_
#define DRM_INIT_PARAMS_H_

#include "drm_global_constants.h"
#include "drm_tables.h"
#include <iostream>

/* class holding the initial (user defined) parameters from which the others are derived */
class config
{
	unsigned short d_RM; // robustness mode (0-4 correspond to A-E)
	unsigned short d_SO; // spectrum occupancy (0-5)
	bool d_UEP; // 0: EEP, 1: UEP
	bool d_text; // 0: text message not used, 1: text message used
	unsigned short d_msc_mapping; // 0: 16-QAM SM, 1: 64-QAM HMsym, 2: 64-QAM HMmix
	unsigned short d_sdc_mapping; // 0: 4-QAM, 1: 16-QAM
	bool d_long_interl; // 0: short interleaving, 1: long interleaving

	tables* ptr_tables; // pointer to tables needed for init

public:
	/* accessor methods */
	unsigned short RM();
	unsigned short SO();
	bool UEP();
	bool text();
	unsigned short msc_mapping();
	unsigned short sdc_mapping();
	bool long_interl();

	config(tables* ptr)
	{
		ptr_tables = ptr;
	};
	~config(){};

	void init();
};

/* classes holding the derived parameters and init routines (where parameters are calculated)*/
class global_params
{
	config* p_tables; // pointer to tables needed for init

public:
	virtual void init(config* config) = 0;

	global_params(){};
	virtual ~global_params(){};
};

class ofdm_params : public global_params
{
	unsigned int d_nfft; // FFT length
	unsigned int d_n_cp; // length of guard interval (samples)
	unsigned short d_cp_ratio_enum; // enumerator of n_cp / nfft ratio
	unsigned short d_cp_ratio_denom; // denominator of n_cp / nfft ratio
	int d_K_min; // index of lowest carrier
	int d_K_max; // index of highest carrier
	unsigned int d_n_unused; // number of unused carriers (always symmetric to DC)
	unsigned int d_N_S; // number of OFDM symbols per transmission frame
	unsigned short d_M_TF; // number of transmission frames per super transmission frame
	unsigned int d_fs_soundcard; // soundcard sample rate

public:
	/* accessor methods */
	unsigned int nfft();
	unsigned int n_cp();
	unsigned short cp_ratio_enum();
	unsigned short cp_ratio_denom();
	int K_min();
	int K_max();
	unsigned int n_unused();
	unsigned int N_S();
	unsigned short M_TF();
	unsigned int fs_soundcard();

	ofdm_params(){};
	virtual ~ofdm_params(){};

	void init(config* cfg);
};

class control_chan_params : public global_params
{
public:
	unsigned int d_L;
	unsigned int d_N;

	float d_coderate;

	control_chan_params(){};
	virtual ~control_chan_params(){};
};

class sdc_params : public control_chan_params
{
public:
	sdc_params(){};
	virtual ~sdc_params(){};

	void init(config* cfg);
};

class fac_params : public control_chan_params
{
public:
	fac_params(){};
	virtual ~fac_params(){};

	void init(config* cfg);
};

class msc_params : public global_params
{
public:
	unsigned int d_L_MUX;
	unsigned int d_L_1;
	unsigned int d_L_2;
	unsigned int d_N_MUX;
	unsigned int d_N_1;
	unsigned int d_N_2;

	msc_params(){};
	virtual ~msc_params(){};

	void init(config* cfg);
};


#endif /* DRM_INIT_PARAMS_H_ */
