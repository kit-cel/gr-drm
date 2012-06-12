/*
 * drm_init_params.cpp
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#include "drm_init_params.h"

/* OFDM params implementation */
void ofdm_params::init(unsigned short rm, unsigned short so)
{
	std::cout << "init ofdm\n";
}

/* MSC channel implementation */
void msc_params::init(unsigned short rm, unsigned short so)
{
	std::cout << "init msc\n";
}

/* SDC channel implementation */
void sdc_params::init(unsigned short rm, unsigned short so)
{
	std::cout << "init sdc\n";
}

/* FAC channel implementation */
void fac_params::init(unsigned short rm, unsigned short so)
{
	std::cout << "init fac\n";
}
