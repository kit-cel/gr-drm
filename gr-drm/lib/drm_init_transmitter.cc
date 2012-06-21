/*
 * drm_init_transmitter.cpp
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#include "drm_transm_params.h"
#include <iostream>

int main()
{
	std::cout << "entering init routine...\n";

	transm_params transm;
	transm.init();

	std::cout << "exiting init routine...\n";
	return 0;
}



