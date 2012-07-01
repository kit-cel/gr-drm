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
#include <drm_generate_sdc_vb.h>


drm_generate_sdc_vb_sptr
drm_make_generate_sdc_vb (transm_params* tp)
{
	return drm_generate_sdc_vb_sptr (new drm_generate_sdc_vb (tp));
}


drm_generate_sdc_vb::drm_generate_sdc_vb (transm_params* tp)
	: gr_sync_block ("generate_sdc",
		gr_make_io_signature (0, 0, 0),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * tp->sdc().L() ))
{
	d_tp = tp;
}


drm_generate_sdc_vb::~drm_generate_sdc_vb ()
{
}


int
drm_generate_sdc_vb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	unsigned char *out = (unsigned char *) output_items[0];
	unsigned char *out_start = out; // pointer to the beginning of output buffer
	
	// Set the data to zero (as defined in the DRM standard) because not all bits might be used
	for( int i = 0; i < noutput_items; i++)
	{
		for( int j = 0; j < d_tp->sdc().L(); j++)
		{
			*out++ = 0;
		}
	}
	out = out_start; // reset pointer to the beginning
		
	// Tell runtime system how many output items we produced.
	return noutput_items;
}

