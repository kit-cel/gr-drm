/* -*- c++ -*- */
/* 
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <drm_generate_fac_vb.h>


drm_generate_fac_vb_sptr
drm_make_generate_fac_vb (transm_params* tp)
{
	return drm_generate_fac_vb_sptr (new drm_generate_fac_vb (tp));
}


drm_generate_fac_vb::drm_generate_fac_vb (transm_params* tp)
	: gr_sync_block ("generate_fac_vb",
		gr_make_io_signature (0, 0, 0),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * tp->fac().L() ))
{
	d_tp = tp;
	d_data = init_data(); // set FAC bitstream
}


drm_generate_fac_vb::~drm_generate_fac_vb ()
{
}

unsigned char*
drm_generate_fac_vb::init_data()
{
	/* allocate storage for the FAC bits */
	const unsigned int fac_length = d_tp->fac().L();
	unsigned char* fac_data = new unsigned char[fac_length];
	
	/* set vector to zero as unused data shall be set to zero (see DRM standard) */
	for( unsigned int i = 0; i < fac_length; i++)
	{
		*fac_data++ = 0;
	}
		
	/* Channel parameters */
	
	
	/* Service parameters */
	
	
	return fac_data;
}

int
drm_generate_fac_vb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	unsigned char *out = (unsigned char *) output_items[0];

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

