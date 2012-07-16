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
#include <drm_qam_map_vbvb.h>


drm_qam_map_vbvb_sptr
drm_make_qam_map_vbvb (const float* map_table, int bits_per_symbol, int vlen_out, int n_inputs)
{
	return drm_qam_map_vbvb_sptr (new drm_qam_map_vbvb (map_table, bits_per_symbol, vlen_out, n_inputs));
}


drm_qam_map_vbvb::drm_qam_map_vbvb (const float* map_table, int bits_per_symbol, int vlen_out, int n_inputs)
	: gr_sync_block ("qam_map_vbvb",
		gr_make_io_signature (n_inputs, n_inputs, sizeof (unsigned char) * vlen_out * 2 ),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * vlen_out ))
{
	d_map_table = map_table;
	d_bits_per_symbol = bits_per_symbol;
	d_vlen_out = vlen_out;
	d_n_inputs = n_inputs;
}


drm_qam_map_vbvb::~drm_qam_map_vbvb ()
{
}


int
drm_qam_map_vbvb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const unsigned char *in = (const unsigned char *) input_items[0];
	unsigned char *out = (unsigned char *) output_items[0];

	// Do <+signal processing+>

	// Tell runtime system how many output items we produced.
	return 1;
}

