/* This class holds the actual DRM receiver configuration */

#ifndef DRMRX_CONF_H
#define DRMRX_CONF_H

#include "drmrx_params.h"

class drmrx_conf
{
        drmrx_params* d_params; // holds all possible configuration values

        // current configuration that is updated at runtime
		int d_RM; // Robustness Mode; 0-4 corresponds to A-E
		int d_SO; // Spectrum Occupancy; 0-5


		public:
		// constructor, destructor
		drmrx_conf();
		~drmrx_conf(){}

		// get methods
		drmrx_params* p(){ return d_params; }
		int RM(){ return d_RM; }
		int SO(){ return d_SO; }

		// set methods
		void set_RM(int RM){ d_RM = RM; }
		void set_SO(int SO){ d_SO = SO; }
};

#endif
