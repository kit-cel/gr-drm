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
#include "scrambler_vbvb_impl.h"

namespace gr {
  namespace drm {

    scrambler_vbvb::sptr
    scrambler_vbvb::make(unsigned int block_len)
    {
      return gnuradio::get_initial_sptr
        (new scrambler_vbvb_impl(block_len));
    }

    /*
     * The private constructor
     */
    scrambler_vbvb_impl::scrambler_vbvb_impl(unsigned int block_len)
      : gr::sync_block("scrambler_vbvb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
    	d_block_len = block_len;
        set_output_multiple(block_len);
    }

    /*
     * Our virtual destructor.
     */
    scrambler_vbvb_impl::~scrambler_vbvb_impl()
    {
    }

    int
    scrambler_vbvb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
        const int n_vectors = noutput_items/d_block_len;
				const unsigned int n_reset = block_len();

				for(int j = 0; j < n_vectors; j++)
				{
					// Generate PRBS of length block_len (G(x) = x^9 + x^5 + 1)
					unsigned char prbs[n_reset]; // Pseudo random bit sequence array
					unsigned char lfsr[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; // inital state: all ones
					unsigned char lfsr_prev[9]; // holds previous shift register state while shifting
					unsigned char next_bit;

					for(int i = 0; i < n_reset; i++)
					{
						// calculate new leading bit and save it to prbs
						next_bit = (lfsr[4] + lfsr[8]) % 2; // this bit is going to be appended at the beginning
						prbs[i] = next_bit;

						// save current state, then shift

						// save
						memcpy(&lfsr_prev[0], &lfsr[0], 9);

						// shift
						memcpy(&lfsr[1], &lfsr_prev[0], 8);

						// append next prbs bit
						lfsr[0] = next_bit;
					}

					// Calculate output sequence by XORing input and prbs
					for(int i = 0; i < n_reset; i++)
					{
						*out++ = (*in++ + prbs[i]) % 2;
					}
				}
        // Tell runtime system how many output items we produced.
        return n_vectors*d_block_len;
    }

  } /* namespace drm */
} /* namespace gr */

