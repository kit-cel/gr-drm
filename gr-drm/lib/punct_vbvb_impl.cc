/* -*- c++ -*- */
/*
 * Copyright 2014 Felix Wunsch, Communications Engineering Lab (CEL) / Karlsruhe Institute of Technology (KIT).
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
#include "punct_vbvb_impl.h"

namespace gr {
  namespace drm {

    punct_vbvb::sptr
    punct_vbvb::make(std::vector<unsigned char> punct_pat_1, std::vector<unsigned char> punct_pat_2, int vlen_in, int vlen_out, int num_tailbits)
    {
      return gnuradio::get_initial_sptr
        (new punct_vbvb_impl(punct_pat_1, punct_pat_2, vlen_in, vlen_out, num_tailbits));
    }

    /*
     * The private constructor
     */
    punct_vbvb_impl::punct_vbvb_impl(std::vector<unsigned char> punct_pat_1, std::vector<unsigned char> punct_pat_2, int vlen_in, int vlen_out, int num_tailbits)
      : gr::block("punct_vbvb",
              gr::io_signature::make(1, 1, sizeof (unsigned char)),
              gr::io_signature::make(1, 1, sizeof (unsigned char)))
	{
		d_vlen_in = vlen_in;
		d_vlen_out = vlen_out;
		d_pp1 = punct_pat_1;
		d_pp2 = punct_pat_2;
		d_n_tail = num_tailbits;
		set_output_multiple(vlen_out);
	}


    /*
     * Our virtual destructor.
     */
    punct_vbvb_impl::~punct_vbvb_impl()
    {
    }

    int
    punct_vbvb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
		unsigned char *in = (unsigned char *) input_items[0];
		unsigned char *out = (unsigned char *) output_items[0];
		int n_vectors = noutput_items/d_vlen_out;

		int len_pp1 = d_pp1.size();
		int len_pp2 = d_pp2.size();
		//std::cout << "vlen_in: " << d_vlen_in << ", vlen_out: " << d_vlen_out << ", len_pp1: " << len_pp1 << ", len_pp2: " << len_pp2 << std::endl;

		for( int i = 0; i < n_vectors; i++)
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
		consume_each (n_vectors*d_vlen_in);

		// Tell runtime system how many output items we produced.
		return n_vectors*d_vlen_out;
    }

	void
	punct_vbvb_impl::forecast(int noutput_items,
								   gr_vector_int &ninput_items_required)
	{
		unsigned ninputs = ninput_items_required.size();
		float interpolation_factor = float(d_vlen_out)/d_vlen_in;
		for(unsigned i = 0; i < ninputs; i++)
			ninput_items_required[i] = noutput_items/interpolation_factor;
	}

  } /* namespace drm */
} /* namespace gr */

