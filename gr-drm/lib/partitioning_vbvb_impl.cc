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
#include "partitioning_vbvb_impl.h"

namespace gr {
  namespace drm {

    partitioning_vbvb::sptr
    partitioning_vbvb::make(unsigned int vlen_in, std::vector<int> vlen_out)
    {
      return gnuradio::get_initial_sptr
        (new partitioning_vbvb_impl(vlen_in, vlen_out));
    }

    /*
     * The private constructor
     */
    partitioning_vbvb_impl::partitioning_vbvb_impl(unsigned int vlen_in, std::vector<int> vlen_out)
      : gr::sync_block("partitioning_vbvb",
              gr::io_signature::make(<+MIN_IN+>, <+MAX_IN+>, sizeof(<+ITYPE+>)),
              gr::io_signature::make(<+MIN_OUT+>, <+MAX_OUT+>, sizeof(<+OTYPE+>)))
    {}

    /*
     * Our virtual destructor.
     */
    partitioning_vbvb_impl::~partitioning_vbvb_impl()
    {
    }

    int
    partitioning_vbvb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
        <+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

        // Do <+signal processing+>

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace drm */
} /* namespace gr */

