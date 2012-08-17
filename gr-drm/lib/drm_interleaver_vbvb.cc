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
#include <drm_interleaver_vbvb.h>


drm_interleaver_vbvb_sptr
drm_make_interleaver_vbvb (std::vector<int> interl_seq)
{
	return drm_interleaver_vbvb_sptr (new drm_interleaver_vbvb (interl_seq));
}


drm_interleaver_vbvb::drm_interleaver_vbvb (std::vector<int> interl_seq)
	: gr_sync_block ("interleaver_vbvb",
		gr_make_io_signature (1, 1, sizeof (unsigned char) * interl_seq.size() ),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * interl_seq.size() ))
{
	d_seq = interl_seq;
}


drm_interleaver_vbvb::~drm_interleaver_vbvb ()
{
}


int
drm_interleaver_vbvb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const unsigned char *in = (const unsigned char *) input_items[0];
	unsigned char *out = (unsigned char *) output_items[0];

	for(int n = 0; n < noutput_items; n++)
	{
		// Interleave array entries according to the interleaver sequence
		for(int i = 0; i < d_seq.size(); i++)
		{
			out[i+n*d_seq.size()] = in[d_seq[i] + n*d_seq.size()];
		}
	}
	// Tell runtime system how many output items we produced.
	return noutput_items;
}

