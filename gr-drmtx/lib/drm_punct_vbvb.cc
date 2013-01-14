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

#include <gr_io_signature.h>
#include <drm_punct_vbvb.h>
#include <iostream>


drm_punct_vbvb_sptr
drm_make_punct_vbvb (std::vector<unsigned char> punct_pat_1, std::vector<unsigned char> punct_pat_2, int vlen_in, int vlen_out, int num_tailbits)
{
	return drm_punct_vbvb_sptr (new drm_punct_vbvb (punct_pat_1, punct_pat_2, vlen_in, vlen_out, num_tailbits));
}


drm_punct_vbvb::drm_punct_vbvb (std::vector<unsigned char> punct_pat_1, std::vector<unsigned char> punct_pat_2, int vlen_in, int vlen_out, int num_tailbits)
	: gr_block ("punct_vbvb",
		gr_make_io_signature (1, 1, sizeof (unsigned char) * vlen_in),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * vlen_out))
{
	d_vlen_in = vlen_in;
	d_vlen_out = vlen_out;
	d_pp1 = punct_pat_1;
	d_pp2 = punct_pat_2;
	d_n_tail = num_tailbits;
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
  unsigned char *in = (unsigned char *) input_items[0];
  unsigned char *out = (unsigned char *) output_items[0];
  
  int len_pp1 = d_pp1.size();
  int len_pp2 = d_pp2.size();
  //std::cout << "vlen_in: " << d_vlen_in << ", vlen_out: " << d_vlen_out << ", len_pp1: " << len_pp1 << ", len_pp2: " << len_pp2 << std::endl;

  for( int i = 0; i < noutput_items; i++)
  {
	  for( int j = 0; j < d_vlen_in; j++)
	  {
	  	  //std::cout << "index j: " << j;
		  if(j < d_vlen_in - d_n_tail) // "normal" puncturing
		  {
		  	  //std::cout << " normal puncturing. pp1[" << j % len_pp1 << "]=" << (int) d_pp1[j % len_pp1];
		  	  if(d_pp1[j % len_pp1]) // keep bit if the corresponding puncturing pattern entry is '1'
		  	  {
		  	  	  //std::cout << "-> KEPT " << (int) in[j + i*d_vlen_in] << ".\n";
		  	  	  *out++ = in[j + i*d_vlen_in];
		  	  }
		  	  else
		  	  {
		  	  	  //std::cout << "-> DROPPED " << (int) in[j + i*d_vlen_in] << ".\n";
		  	  }
		  }
		  else // tail bit puncturing
		  {
		  	  //std::cout << " tail biting. pp2[" << j % len_pp2 << "]=" << (int) d_pp2[(j  + i*d_vlen_in) % len_pp2];
		  	  if(d_pp2[j % len_pp2]) // keep bit if the corresponding puncturing pattern entry is '1'
		  	  {
		  	  	  //std::cout << "-> KEPT " << (int) in[j + i*d_vlen_in] << ".\n";
		  	  	  *out++ = in[j + i*d_vlen_in];
		  	  }
		  	  else
		  	  {
		  	  	  //std::cout << "-> DROPPED " << (int) in[j + i*d_vlen_in] << ".\n";
		  	  }
		  }
	  }
  }
  
  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (noutput_items);

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

