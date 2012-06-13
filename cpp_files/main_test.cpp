/*
 * main_test.cpp
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#include "drm_init_params.h"
#include <iostream>

int main()
{
	std::cout << "entering test routine...\n";

	/* init tables */
	tables tab;

	/* init user defined configuration (currently hardcoded)*/
	config cfg(&tab);
	cfg.init();

	/* init transmission parameters matching the user defined configuration */
	ofdm_params ofdm;
	ofdm.init(&cfg);

	msc_params msc;
	msc.init(&cfg);

	sdc_params sdc;
	sdc.init(&cfg);

	fac_params fac;
	fac.init(&cfg);

	std::cout << "exiting test routine...\n";
	return 0;
}



