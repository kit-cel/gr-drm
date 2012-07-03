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

void
drm_generate_sdc_vb::init_data(unsigned char* data)
{
	unsigned char* data_start = data; // pointer to the beginning of the SDC data
	
	/* set vector to zero as unused data shall be set to zero (see DRM standard) */
	memset(data, 0, d_tp->sdc().L());
	
	/* AFS index */
	enqueue_bits(data, 4, (unsigned char[]) {0,0,0,1});
	
	/* data field 
	 * The data entities are consist of a header and a body
	 * header format: length of body (7 bits), version flag (1 bit), data entity type (4 bits) */
	
	/* Multiplex description data entity - type - */
	
	// header
	
}

drm_generate_sdc_vb::~drm_generate_sdc_vb ()
{
}


int
drm_generate_sdc_vb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const unsigned int sdc_length = d_tp->sdc().L();
	unsigned char* data = new unsigned char[sdc_length];
	
	init_data(data);
	memcpy( (void*) output_items[0], (void*) data, (size_t) sizeof(char) * sdc_length );
	delete[] data;
		
	// Tell runtime system how many output items we produced.
	return 1;
}

