/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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
#include "qam_map_vbvc_impl.h"

namespace gr {
  namespace drm {

    qam_map_vbvc::sptr
    qam_map_vbvc::make(const float map_table[][2], int bits_per_symbol, int vlen_out, int n_inputs)
    {
      return gnuradio::get_initial_sptr
        (new qam_map_vbvc_impl(map_table, bits_per_symbol, vlen_out, n_inputs));
    }

    /*
     * The private constructor
     */
    qam_map_vbvc_impl::qam_map_vbvc_impl(const float map_table[][2], int bits_per_symbol, int vlen_out, int n_inputs)
      : gr::sync_block("qam_map_vbvc",
              gr::io_signature::make(n_inputs, n_inputs, sizeof (unsigned char) * vlen_out * 2),
              gr::io_signature::make(1, 1, sizeof (gr_complex) * vlen_out))
	{
		// set values for d_map_table
		memset(d_map_table, 0, 16); // set d_map_table to zero
		int rows = std::pow(2, bits_per_symbol/2); // FIXME: this only works for symmetric QAM constellations (not for 8-QAM for example)
		for( int i = 0; i < rows; i++)
		{
			d_map_table[i][0] = map_table[i][0];
			d_map_table[i][1] = map_table[i][1];
		}
	
		d_bits_per_symbol = bits_per_symbol;
		d_vlen_out = vlen_out;
		d_n_inputs = n_inputs;
		for(int i = 0; i < 8; i++)
		{
			d_pow2.push_back(pow(2,i));
		}
	}

    /*
     * Our virtual destructor.
     */
    qam_map_vbvc_impl::~qam_map_vbvc_impl()
    {
    }

    int
    qam_map_vbvc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
	{
		unsigned char *in[d_n_inputs];
		gr_complex *out = (gr_complex *) output_items[0];
	
		// define input pointers
		for( int i = 0; i < d_n_inputs; i++)
		{
			in[i] = (unsigned char*) input_items[i];
		}
	
		std::vector< int > bits; // holds the bit to be processed for one symbol
		int iq_index[2]; // first index is I, 2nd is Q
		int tmp_index;

		for(int n = 0; n < noutput_items; n++)
		{
			// Map bits from several streams to symbols (SM only)
			for( int i = 0; i < d_vlen_out; i++)
			{
				// reset index and bit vector
				iq_index[0] = 0;
				iq_index[1] = 0;
				bits.clear();
		
				// take 2 bits from every stream
				for( int k = 0; k < 2; k++)
				{
					tmp_index = 0;
			
					// iterate over streams
					for( int j = 0; j < d_n_inputs; j++)
					{
						// take one bit from each stream
						bits.push_back( *(in[j])++ );

						tmp_index += d_pow2[d_n_inputs - j - 1] * (int) bits.back(); // add bit in decimal representation to the index
					}
					iq_index[k] = tmp_index;
				}
				// map bits through decimal indexes to output symbol
				*out++ = gr_complex( d_map_table[ iq_index[0] ][0], d_map_table[ iq_index[1] ][1] );		
			}
		}
		// Tell runtime system how many output items we produced.
		return noutput_items;
	}

  } /* namespace drm */
} /* namespace gr */

