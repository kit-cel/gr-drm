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
#include "interleaver_bb_impl.h"

namespace gr {
  namespace drm {

    interleaver_bb::sptr
    interleaver_bb::make(std::vector<int> interl_seq)
    {
      return gnuradio::get_initial_sptr
        (new interleaver_bb_impl(interl_seq));
    }

    /*
     * The private constructor
     */
    interleaver_bb_impl::interleaver_bb_impl(std::vector<int> interl_seq)
      : gr::sync_block("interleaver_bb",
              gr::io_signature::make(1, 1, sizeof (unsigned char)),
              gr::io_signature::make(1, 1, sizeof (unsigned char)))
	{
		d_seq = interl_seq;
		d_seqsize = interl_seq.size();
        set_output_multiple(interl_seq.size());

	}

    /*
     * Our virtual destructor.
     */
    interleaver_bb_impl::~interleaver_bb_impl()
    {
    }

    int
    interleaver_bb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
	{
		const unsigned char *in = (const unsigned char *) input_items[0];
		unsigned char *out = (unsigned char *) output_items[0];
        const int n_vectors = noutput_items/d_seqsize;

		for(int n = 0; n < n_vectors; n++)
		{
			// Interleave array entries according to the interleaver sequence
			for(int i = 0; i < d_seqsize; i++)
			{
				*out++ = in[d_seq[i] + n*d_seqsize];
			}
		}

		// Tell runtime system how many output items we produced.
		return n_vectors * d_seqsize;
	}

  } /* namespace drm */
} /* namespace gr */

