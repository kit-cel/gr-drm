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
#include <drm_add_tailbits_vbvb.h>


drm_add_tailbits_vbvb_sptr
drm_make_add_tailbits_vbvb (int vlen_in, int n_tailbits)
{
	return drm_add_tailbits_vbvb_sptr (new drm_add_tailbits_vbvb (vlen_in, n_tailbits));
}


drm_add_tailbits_vbvb::drm_add_tailbits_vbvb (int vlen_in, int n_tailbits)
	: gr_sync_block ("add_tailbits_vbvb",
		gr_make_io_signature (1, 1, sizeof (unsigned char) * vlen_in),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * (vlen_in + n_tailbits)))
{
	d_vlen = vlen_in;
	d_n_tail = n_tailbits;
}


drm_add_tailbits_vbvb::~drm_add_tailbits_vbvb ()
{
}


int
drm_add_tailbits_vbvb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	unsigned char *in = (unsigned char *) input_items[0];
	unsigned char *out = (unsigned char *) output_items[0];
	
	// set tailbits to zero TODO: make tailbits configurable
	unsigned char tailbits[(const int) d_n_tail];
	memset(tailbits, 0, d_n_tail);
	
	for( int i = 0; i < noutput_items; i++)
	{
		// Append n_tailbits zeros to the input vector
		memcpy(out, in, d_vlen); // copy input to output
		
		memcpy(out + d_vlen, tailbits, d_n_tail);
		
		// move buffer pointers
		in = in + d_vlen;
		out = out + d_vlen + d_n_tail;
	}
	
	// Tell runtime system how many output items we produced.
	return noutput_items;
}

