/*
 * drm_transm_params.cpp
 *
 *  Created on: 20.06.2012
 *      Author: felixwunsch
 */

#include "drm_transm_params.h"
#include "drm_tables.h"
#include "drm_params.h"

transm_params::transm_params()
{
	d_cfg = NULL;
	d_ofdm = NULL;
	d_msc = NULL;
	d_sdc = NULL;
	d_fac = NULL;
}

void
transm_params::init()
{
	/* init tables */
	tables* t = new tables;

	/* init user defined configuration (currently hardcoded)*/
	config* c = new config;
	c->init(t);

	/* init transmission parameters matching the user defined configuration */
	ofdm_params* o = new ofdm_params;
	o->init(c);

	msc_params* m = new msc_params;
	m->init(c);

	sdc_params* s = new sdc_params;
	s->init(c);

	fac_params* f = new fac_params;
	f->init(c);

	/* set member variables (pointers to param instances) */
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
