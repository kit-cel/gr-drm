/*
 * drm_transm_params.cpp
 *
 *  Created on: 20.06.2012
 *      Author: felixwunsch
 */

#include "drm_transm_params.h"

transm_params::transm_params()
{
	d_cfg = NULL;
	d_ofdm = NULL;
	d_msc = NULL;
	d_sdc = NULL;
	d_fac = NULL;
}

void
transm_params::init(config* c, ofdm_params* o, msc_params* m, sdc_params* s, fac_params* f)
{
	d_cfg = c;
	d_ofdm = o;
	d_msc = m;
	d_sdc = s;
	d_fac = f;
}

config*
transm_params::cfg()
{
	return d_cfg;
}

ofdm_params*
transm_params::ofdm()
{
	return d_ofdm;
}

msc_params*
transm_params::msc()
{
	return d_msc;
}

sdc_params*
transm_params::sdc()
{
	return d_sdc;
}

fac_params*
transm_params::fac()
{
	return d_fac;
}
