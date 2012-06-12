/*
 * drm_init_params.cpp
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#include "drm_init_params.h"

/* config implementation */
void config::init()
{
	std::cout << "init cfg\n";
	RM = 1; //  B
	SO = 3; // 10 kHz
	UEP = false; // EEP
	text = false; // no text message included
	long_interl = false; // short interleaving (400ms)
	msc_mapping = 0; // 16-QAM SM
	sdc_mapping = 0; // 4-QAM
}

/* OFDM params implementation */
void ofdm_params::init(config* cfg)
{
	std::cout << "init ofdm\n";// TODO: create tables and don't hardcode the values
	nfft = 1024; //FIXME
	cp_ratio = 1/4; //FIXME
	n_cp = nfft * cp_ratio;
	K_min = -103;
	K_max = 103;
	N_S = 15; //FIXME
	M_TF = 3; //FIXME TODO: find a way to avoid double definition


}

/* MSC channel implementation */
void msc_params::init(config* cfg)
{
	std::cout << "init msc\n";
}

/* SDC channel implementation */
void sdc_params::init(config* cfg)
{
	std::cout << "init sdc\n";
}

/* FAC channel implementation */
void fac_params::init(config* cfg)
{
	std::cout << "init fac\n";
}
