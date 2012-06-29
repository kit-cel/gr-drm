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
#include <drm_vec_demux_vbvb.h>


drm_vec_demux_vbvb_sptr
drm_make_vec_demux_vbvb (unsigned int vlen_in, unsigned int n_out, const std::vector<int> &vlen_out)
{
	return drm_vec_demux_vbvb_sptr (new drm_vec_demux_vbvb (vlen_in, n_out, vlen_out));
}


drm_vec_demux_vbvb::drm_vec_demux_vbvb (unsigned int vlen_in, unsigned int n_out, const std::vector<int> &vlen_out)
	: gr_sync_block ("vec_demux_vbvb",
		gr_make_io_signature (1, 1, sizeof (unsigned char) * vlen_in),
		gr_make_io_signaturev (1, -1, vlen_out)) // no sizeof() required as unsigned chars have length 1
{
	set_vlen_in(vlen_in);
	set_n_out(n_out);
	set_vlen_out(vlen_out);
}


drm_vec_demux_vbvb::~drm_vec_demux_vbvb ()
{
}


int
drm_vec_demux_vbvb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	unsigned char *out[n_out()]; // array of pointers to output ports
	for(int i = 0; i<n_out(); i++)
	{
		out[i] = (unsigned char*) output_items[i]; // assign pointers
	}
	unsigned char *in = (unsigned char *) input_items[0];
	
	std::cout << "noutput_items:\t" << noutput_items;
	std::cout << "vlen_out.size():\t" << vlen_out().size();
	std::cout << "n_out:\t" << n_out() << std::endl;
	
	// TODO check input values (if GRC is not used...)
	if(n_out() != vlen_out().size())
	{
		std::cout << "Dimension mismatch between vlen_out and n_out!\n";
	}

	// Divide input vector up into multiple output vectors
	for (int i = 0; i < noutput_items; i++)
	{
		for (int j = 0; j < n_out(); j++)
		{
			for (int k = 0; k < vlen_out()[k]; k++)
			{
				*(out[j])++ = *in++; 
			}
		}
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

