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
#include "add_tailbits_bb_impl.h"


namespace gr {
  namespace drm {

    add_tailbits_bb::sptr
    add_tailbits_bb::make(int vlen_in, int n_tailbits)
    {
      return gnuradio::get_initial_sptr
        (new add_tailbits_bb_impl(vlen_in, n_tailbits));
    }

    /*
     * The private constructor
     */
    add_tailbits_bb_impl::add_tailbits_bb_impl(int vlen_in, int n_tailbits)
            : gr::block("add_tailbits_bb",
                                       gr::io_signature::make(1, 1, sizeof(unsigned char)),
                                       gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {
        d_vlen = vlen_in;
        d_n_tail = n_tailbits;
        set_output_multiple(vlen_in + n_tailbits);
    }

    /*
     * Our virtual destructor.
     */
    add_tailbits_bb_impl::~add_tailbits_bb_impl()
    {
    }

    int
    add_tailbits_bb_impl::general_work(int noutput_items,
              gr_vector_int &ninput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
		unsigned char *in = (unsigned char *) input_items[0];
		unsigned char *out = (unsigned char *) output_items[0];
        const int n_vectors = noutput_items/(d_vlen+d_n_tail);
        // set tailbits to zero TODO: make tailbits configurable
		__GR_VLA(unsigned char, tailbits, (const int) d_n_tail);
		memset(tailbits, 0, d_n_tail);

		for( int i = 0; i < n_vectors; i++)
		{
			// Append n_tailbits zeros to the input vector
			memcpy(out, in, d_vlen); // copy input to output

			memcpy(out + d_vlen, tailbits, d_n_tail);

			// move buffer pointers
			in = in + d_vlen;
			out = out + d_vlen + d_n_tail;
        }
        consume_each(noutput_items-n_vectors*d_n_tail);
		// Tell runtime system how many output items we produced.
		return n_vectors*(d_vlen+d_n_tail);
    }

      void
      add_tailbits_bb_impl::forecast(int noutput_items,
                gr_vector_int &ninput_items_required)
      {
          unsigned ninputs = ninput_items_required.size();
          float interpolation_factor = float(d_vlen+d_n_tail)/d_vlen;
          for(unsigned i = 0; i < ninputs; i++)
              ninput_items_required[i] = noutput_items/interpolation_factor;
      }

  } /* namespace drm */
} /* namespace gr */

