/* -*- c++ -*- */
/* 
 * Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
 * Author: Felix Wunsch
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

#include <gnuradio/io_signature.h>
#include <drm_partitioning_vbvb.h>


drm_partitioning_vbvb_sptr
drm_make_partitioning_vbvb (unsigned int vlen_in, std::vector<int> vlen_out)
{
	return drm_partitioning_vbvb_sptr (new drm_partitioning_vbvb (vlen_in, vlen_out));
}


drm_partitioning_vbvb::drm_partitioning_vbvb (unsigned int vlen_in, std::vector<int> vlen_out)
	: gr::sync_block ("partitioning_vbvb",
		gr::io_signature::make (1, 1, sizeof (unsigned char) * vlen_in),
		gr::io_signature::makev (vlen_out.size(), vlen_out.size(), vlen_out))
{
	d_n_out = vlen_out.size();
	d_vlen_out = vlen_out;
}


drm_partitioning_vbvb::~drm_partitioning_vbvb ()
{
}


int
drm_partitioning_vbvb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	unsigned char *in = (unsigned char *) input_items[0];
	unsigned char *out[d_n_out];
	for(int i = 0; i < d_n_out; i++)
	{
		out[i] = (unsigned char*) output_items[i];
	}

	// Divide input vector up
	for(int n = 0; n < noutput_items; n++)
	{
		for (int j = 0; j < d_n_out; j++)
		{
			for (int k = 0; k < d_vlen_out[j]; k++)
			{
				*(out[j])++ = *in++; 
			}
		}
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

