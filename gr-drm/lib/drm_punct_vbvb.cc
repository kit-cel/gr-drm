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
#include <drm_punct_vbvb.h>


drm_punct_vbvb_sptr
drm_make_punct_vbvb (std::vector<int> punct_pat_1, std::vector<int> punct_pat_2, int vlen_in, int vlen_out, int num_tailbits)
{
	return drm_punct_vbvb_sptr (new drm_punct_vbvb (punct_pat_1, punct_pat_2, vlen_in, vlen_out, num_tailbits));
}


drm_punct_vbvb::drm_punct_vbvb (std::vector<int> punct_pat_1, std::vector<int> punct_pat_2, int vlen_in, int vlen_out, int num_tailbits)
	: gr_block ("punct_vbvb",
		gr_make_io_signature (1, 1, sizeof (unsigned char) * vlen_in),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * vlen_out))
{
}


drm_punct_vbvb::~drm_punct_vbvb ()
{
}


int
drm_punct_vbvb::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  const float *in = (const float *) input_items[0];
  float *out = (float *) output_items[0];

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (noutput_items);

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

