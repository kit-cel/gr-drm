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

	unsigned short rm = 0;
	unsigned short so = 0;

	ofdm_params ofdm;
	ofdm.init(rm, so);

	msc_params msc;
	msc.init(rm, so);
	sdc_params sdc;
	sdc.init(rm, so);
	fac_params fac;
	fac.init(rm, so);

	std::cout << "exiting test routine...\n";
	return 0;
}



