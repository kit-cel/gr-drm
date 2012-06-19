/*
 * main_test.cpp
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#include "drm_params.h"

#include <iostream>

int main()
{
	std::cout << "entering init routine...\n";

	/* init tables */
	tables tab;

	/* init user defined configuration (currently hardcoded)*/
	config cfg;
	cfg.init(&tab);

	/* init transmission parameters matching the user defined configuration */
	ofdm_params ofdm;
	ofdm.init(&cfg);

	msc_params msc;
	msc.init(&cfg);

	sdc_params sdc;
	sdc.init(&cfg);

	fac_params fac;
	fac.init(&cfg);

	std::cout << "exiting init routine...\n";
	return 0;
}



