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
#include "drm_config.h"
#include <iostream>
#include <vector>
#include <cmath>

/* classes holding the derived parameters and init routines (where parameters are calculated)*/
class global_params
{
public:
	virtual void init(config* cfg) = 0;

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

	ofdm_params();
	virtual ~ofdm_params(){};

	void init(config* cfg);
};

class control_chan_params : public global_params
{
protected:
	unsigned int d_L; // length in bits
	unsigned int d_N; // number of QAM symbols

	float d_R_0; // coderate R_0 TODO: is this value necessary?
	unsigned short d_R_0_enum; // enumerator of R_0
	unsigned short d_R_0_denom; // denominator of R_0

public:
	unsigned int L();
	unsigned int N();
	float R_0();
	unsigned short R_0_enum();
	unsigned short R_0_denom();

	control_chan_params();
	virtual ~control_chan_params(){};
};

class sdc_params : public control_chan_params
{
	float d_R_1; // coderate R_1
	unsigned short d_R_1_enum; // enumerator of R_1
	unsigned short d_R_1_denom; // denominator of R_1
	unsigned int d_n_bytes_datafield; // number of bytes in the SDC data field
	std::vector< int > d_M_index; // indexes needed for partitioning
	std::vector< int > d_M_total; // total number of samples per coding level
	unsigned short d_n_levels_mlc; // number of levels in the multi level coding process

public:
	float R_1();
	unsigned short R_1_enum();
	unsigned short R_1_denom();
	unsigned int n_bytes_datafield();
	std::vector< int > M_index();
	std::vector< int > M_total();
	unsigned short n_levels_mlc();

	sdc_params();
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
	/* NOTE: so far, only SM is implemented! TODO: implement hierarchical mapping */
	unsigned int d_L_MUX; // number of bits per multiplex frame
	unsigned int d_L_1; // higher protected bits
	unsigned int d_L_2; // lower protected bits
	unsigned int d_L_VSPP; // very strongly protected bits
	unsigned int d_N_MUX; // number of QAM cells per multiplex frame
	unsigned int d_N_1; // higher protected cells
	unsigned int d_N_2; // lower protected cells

	unsigned short d_MSC_prot_1; // MSC protection level for higher protected part (see 7.1 and tables 64-70 in the standard)
	unsigned short d_MSC_prot_2; // protection level for lower protected part

	unsigned short d_R_0_enum_1; // enumerator of R0 for the higher protected part
	unsigned short d_R_0_denom_1; // denominator of ...
	unsigned short d_R_1_enum_1;
	unsigned short d_R_1_denom_1;
	unsigned short d_R_2_enum_1;
	unsigned short d_R_2_denom_1;
	unsigned short d_R_Ylcm_1;
	unsigned short d_R_0_enum_2; // enumerator of R0 for the lower protected part
	unsigned short d_R_0_denom_2;
	unsigned short d_R_1_enum_2;
	unsigned short d_R_1_denom_2;
	unsigned short d_R_2_enum_2;
	unsigned short d_R_2_denom_2;
	unsigned short d_R_Ylcm_2;
	std::vector< int > d_M_index; // indexes needed for partitioning
	std::vector< int > d_M_total; // total number of bits on each encoder level
	unsigned short d_n_levels_mlc; // number of levels in the multi level coding process

public:
	unsigned int L_MUX();
	unsigned int L_1();
	unsigned int L_2();
	unsigned int L_VSPP();
	unsigned int N_MUX();
	unsigned int N_1();
	unsigned int N_2();

	unsigned short MSC_prot_1();
	unsigned short MSC_prot_2();
	unsigned short R_0_enum_1();
	unsigned short R_0_denom_1();
	unsigned short R_1_enum_1();
	unsigned short R_1_denom_1();
	unsigned short R_2_enum_1();
	unsigned short R_2_denom_1();
	unsigned short R_Ylcm_1();
	unsigned short R_0_enum_2();
	unsigned short R_0_denom_2();
	unsigned short R_1_enum_2();
	unsigned short R_1_denom_2();
	unsigned short R_2_enum_2();
	unsigned short R_2_denom_2();
	unsigned short R_Ylcm_2();
	std::vector< int > M_index();
	std::vector< int > M_total();
	unsigned short n_levels_mlc();
	
	void calc_vars_SM(config* cfg);

	msc_params();
	virtual ~msc_params(){};

	void init(config* cfg);
};

#endif /* DRM_INIT_PARAMS_H_ */
