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
#include <drm/config.h>
#endif

#include <gnuradio/io_signature.h>
#include "interleaver_vbvb_impl.h"

namespace gr  {
  namespace drm {
    interleaver_vbvb::sptr interleaver_vbvb::make (std::vector<int> interl_seq)
    {
            return gnuradio::get_initial_sptr (new interleaver_vbvb_impl (interl_seq));
    }


    interleaver_vbvb_impl::interleaver_vbvb_impl (std::vector<int> interl_seq)
            : sync_block ("interleaver_vbvb",
                          io_signature::make (1, 1, sizeof (unsigned char) * interl_seq.size() ),
                          io_signature::make (1, 1, sizeof (unsigned char) * interl_seq.size() ))
    {
            d_seq = interl_seq;
            d_seqsize = interl_seq.size();
    }


    interleaver_vbvb_impl::~interleaver_vbvb_impl ()
    {
    }


    int
    interleaver_vbvb_impl::work (int noutput_items,
                            gr_vector_const_void_star &input_items,
                            gr_vector_void_star &output_items)
    {
            const unsigned char *in = (const unsigned char *) input_items[0];
            unsigned char *out = (unsigned char *) output_items[0];

            for(int n = 0; n < noutput_items; n++)
            {
                    // Interleave array entries according to the interleaver sequence
                    for(int i = 0; i < d_seqsize; i++)
                    {
                            *out++ = in[d_seq[i] + n*d_seqsize];
                    }
            }

            // Tell runtime system how many output items we produced.
            return noutput_items;
    }

  }
}
