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
				: gr::block("partitioning_vbvb",
								 gr::io_signature::make(1, 1, sizeof (unsigned char)),
								 gr::io_signature::make(vlen_out.size(), vlen_out.size(), sizeof (unsigned char)))
		{
			d_n_out = vlen_out.size();
			d_vlen_out = vlen_out;
			d_vlen_in = vlen_in;

			// Find maximum element in vlen_out
			int max_vlen_out = 0;
			for(int i = 0; i < d_n_out;i++){
				if(vlen_out[i] > max_vlen_out)
					max_vlen_out = vlen_out[i];
			}
			set_output_multiple(max_vlen_out);
		}

		/*
         * Our virtual destructor.
         */
		partitioning_vbvb_impl::~partitioning_vbvb_impl()
		{
		}

		void
		partitioning_vbvb_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
		{
			ninput_items_required[0] = d_vlen_in;
		}

		int
		partitioning_vbvb_impl::general_work(int noutput_items,
											 gr_vector_int &ninput_items,
											 gr_vector_const_void_star &input_items,
											 gr_vector_void_star &output_items)
		{
			unsigned char *in = (unsigned char *) input_items[0];
			for (int j = 0; j < d_n_out; j++) {
				unsigned char *out = (unsigned char *) output_items[j];
				memcpy(out, in, d_vlen_out[j]);
					// move buffers
				in += d_vlen_out[j];
			}
			// consume samples from input port
			consume_each(d_vlen_in);
			// tell runtime system how many output items we produced for each output port
			for(int n = 0; n < d_n_out;n++)
				produce(n,d_vlen_out[n]);
			// Tell runtime system how many output items we produced.
			return WORK_CALLED_PRODUCE;
		}
	} /* namespace drm */
} /* namespace gr */