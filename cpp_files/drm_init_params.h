/*
 * drm_init_params.h
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#ifndef DRM_INIT_PARAMS_H_
#define DRM_INIT_PARAMS_H_

#include <iostream>
#include <vector>

/* class holding the initial (user defined) parameters from which the others are derived */
class config
{
public:
	unsigned short RM; // robustness mode (0-4 correspond to A-E)
	unsigned short SO; // spectrum occupancy (0-5)
	bool UEP; // 0: EEP, 1: UEP
	bool text; // 0: text message not used, 1: text message used
	unsigned short msc_mapping; // 0: 16-QAM SM, 1: 64-QAM HMsym, 2: 64-QAM HMmix
	unsigned short sdc_mapping; // 0: 4-QAM, 1: 16-QAM
	bool long_interl; // 0: short interleaving, 1: long interleaving

	config(){};
	~config(){};

	void init();
};

/* classes holding the derived parameters and init routines (where parameters are calculated)*/
class global_params
{
public:
	virtual void init(config* config) = 0;

	global_params(){};
	virtual ~global_params(){};
};

class ofdm_params : public global_params
{
public:
	unsigned int nfft; // FFT length
	unsigned int n_cp; // length of guard interval (samples)
	float cp_ratio; // ratio of n_cp / nfft
	int K_min; // index of lowest carrier
	int K_max; // index of highest carrier
	std::vector<short> K_unused; // unused carrier indices
	std::vector<int> pos_ref_freq; // position of frequency reference cells
	unsigned int N_S; // number of OFDM symbols per transmission frame
	unsigned short M_TF; // number of transmission frames per super transmission frame

	ofdm_params(){};
	virtual ~ofdm_params(){};

	void init(config* cfg);
};

class control_chan_params : public global_params
{
public:
	unsigned int L;
	unsigned int N;

	float coderate;

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
	unsigned int L_MUX;
	unsigned int L_1;
	unsigned int L_2;
	unsigned int N_MUX;
	unsigned int N_1;
	unsigned int N_2;

	msc_params(){};
	virtual ~msc_params(){};

	void init(config* cfg);
};


#endif /* DRM_INIT_PARAMS_H_ */
