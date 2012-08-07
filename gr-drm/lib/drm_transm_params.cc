/*
 * drm_transm_params.cpp
 *
 *  Created on: 20.06.2012
 *      Author: felixwunsch
 */

#include "drm_transm_params.h"
#include "drm_tables.h"
#include "drm_params.h"

transm_params::transm_params(unsigned short RM, 
							 unsigned short SO, 
							 bool UEP,
							 unsigned int n_bytes_A, 
							 bool text,
							 unsigned short msc_mapping,
							 unsigned short msc_prot_level_1, 
							 unsigned short msc_prot_level_2,
							 unsigned short sdc_mapping,
							 unsigned short sdc_prot_level,
							 bool long_interl,
							 unsigned int audio_samp_rate,
							 std::string station_label)
{
	/* init tables */
	tables* t = new tables;

	/* init user defined configuration (currently partly hardcoded)*/
	config* c = new config;
	c->init(t, RM, SO, UEP, n_bytes_A, text, msc_mapping, msc_prot_level_1, msc_prot_level_2,
			sdc_mapping, sdc_prot_level, long_interl, audio_samp_rate, station_label);

	/* init transmission parameters matching the user defined configuration */
	ofdm_params* o = new ofdm_params;
	o->init(c);

	msc_params* m = new msc_params;
	m->init(c);

	sdc_params* s = new sdc_params;
	s->init(c);

	fac_params* f = new fac_params;
	f->init(c);
	
	t->calc_gain_cell_params(c->RM(), c->SO(), o->N_S(), o->K_min(), o->K_max());

	/* set member variables (pointers to param instances) */
	d_cfg = c;
	d_ofdm = o;
	d_msc = m;
	d_sdc = s;
	d_fac = f;
}

config
transm_params::cfg()
{
	return *d_cfg;
}

ofdm_params
transm_params::ofdm()
{
	return *d_ofdm;
}

msc_params
transm_params::msc()
{
	return *d_msc;
}

sdc_params
transm_params::sdc()
{
	return *d_sdc;
}

fac_params
transm_params::fac()
{
	return *d_fac;
}
