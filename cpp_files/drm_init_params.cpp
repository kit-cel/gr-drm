/*
 * drm_init_params.cpp
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#include "drm_init_params.h"

/* config implementation */
void
config::init()
{
	std::cout << "init cfg\n";

	/* user defined parameters */
	d_RM = 1; // B
	d_SO = 3; // 10 kHz
	d_UEP = false; // EEP
	d_text = false; // no text message included
	d_long_interl = false; // short interleaving (400ms)
	d_msc_mapping = 0; // 16-QAM SM
	d_sdc_mapping = 0; // 4-QAM
}

unsigned short
config::RM()
{
	return d_RM;
}

unsigned short
config::SO()
{
	return d_SO;
}

bool
config::UEP()
{
	return d_UEP;
}

bool
config::text()
{
	return d_text;
}

bool
config::long_interl()
{
	return d_long_interl;
}

unsigned short
config::msc_mapping()
{
	return d_msc_mapping;
}

unsigned short
config::sdc_mapping()
{
	return d_sdc_mapping;
}

/* OFDM params implementation */
void
ofdm_params::init(config* cfg)
{
	std::cout << "init ofdm\n";// TODO: define values for RM E (DRM+)

	/* define tables to take the values for the current configuration from */
	unsigned int tab_nfft[NUM_RM] = {1152, 1024, 704, 448, 0}; //  taken from Dream FIXME: missing value for E
	unsigned int tab_N_S[NUM_RM] = {15, 15, 20, 24, 40}; // see DRM standard Table 82
	unsigned int tab_cp_ratio[NUM_RM][2] = {{1, 9}, {1, 4}, {4, 11}, {11, 14}, {1, 9}}; // denominator
	unsigned short tab_M_TF[NUM_RM] = {3, 3, 3, 3, 4}; // see DRM standard p. 137
	unsigned int tab_K_range[2*NUM_RM][NUM_SO] = {{2, 2, -102, -114, -98, -110}, // see DRM standard Table 84
												  {102, 114, 102, 114, 314, 350},
												  {1, 1, -91, -103, -87, -99},
												  {91, 103, 91, 103, 279, 311},
												  {0, 0, 0, -69, 0, -67},
												  {0, 0, 0, 69, 0, 213},
												  {0, 0, 0, -44, 0, -43},
												  {0, 0, 0, 44, 0, 135},
												  {-106, 0, 0, 0, 0, 0},
												  {106, 0, 0, 0, 0, 0}};
	unsigned short tab_n_unused[NUM_RM] = {3, 1, 1, 1, 0}; // see DRM standard Table 85

	d_nfft = tab_nfft[cfg->RM()];
	d_cp_ratio_enum = tab_cp_ratio[cfg->RM()][0];
	d_cp_ratio_denom = tab_cp_ratio[cfg->RM()][1];
	d_n_cp = d_nfft * d_cp_ratio_enum / d_cp_ratio_denom;
	d_N_S = tab_N_S[cfg->RM()];
	d_M_TF = tab_M_TF[cfg->RM()];
	d_K_min = tab_K_range[2*cfg->RM()][cfg->SO()];
	d_K_max = tab_K_range[2*cfg->RM() + 1][cfg->SO()];
	d_n_unused = tab_n_unused[cfg->RM()];
	d_fs_soundcard = FS_SOUNDCARD;
}

unsigned int
ofdm_params::nfft()
{
	return d_nfft;
}

unsigned short
ofdm_params::cp_ratio_enum()
{
	return d_cp_ratio_enum;
}

unsigned short
ofdm_params::cp_ratio_denom()
{
	return d_cp_ratio_denom;
}

unsigned int
ofdm_params::n_cp()
{
	return d_n_cp;
}

unsigned int
ofdm_params::N_S()
{
	return d_N_S;
}

unsigned short
ofdm_params::M_TF()
{
	return d_M_TF;
}

int
ofdm_params::K_min()
{
	return d_K_min;
}

int
ofdm_params::K_max()
{
	return d_K_max;
}

unsigned int
ofdm_params::n_unused()
{
	return d_n_unused;
}

unsigned int
ofdm_params::fs_soundcard()
{
	return d_fs_soundcard;
}

/* MSC channel implementation */
void
msc_params::init(config* cfg)
{
	std::cout << "init msc\n";
}

/* SDC channel implementation */
void
sdc_params::init(config* cfg)
{
	std::cout << "init sdc\n";
	if(cfg->RM() == 4) // see DRM standard 7.2.1.2
	{
		// RM E
//		d_N = ;
//		d_L = ;
	}
	else
	{
		// RM A-D
//		d_N = ;
//		d_L = ;
	}
}

/* FAC channel implementation */
void
fac_params::init(config* cfg)
{
	std::cout << "init fac\n";

	if(cfg->RM() == 4) // see DRM standard 7.2.1.2
	{
		// RM E
		d_N = N_FAC_DRMPLUS;
		d_L = L_FAC_DRMPLUS;
	}
	else
	{
		// RM A-D
		d_N = N_FAC_DRM;
		d_L = L_FAC_DRM;
	}
}
