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
#include "interleaver_vcvc_impl.h"

namespace gr {
  namespace drm {

    interleaver_vcvc::sptr
    interleaver_vcvc::make(std::vector<int> interl_seq, bool long_interl, int depth)
    {
      return gnuradio::get_initial_sptr
        (new interleaver_vcvc_impl(interl_seq, long_interl, depth));
    }

    /*
     * The private constructor
     */
    interleaver_vcvc_impl::interleaver_vcvc_impl(std::vector<int> interl_seq, bool long_interl, int depth)
      : gr::block("interleaver_vcvc",
              gr::io_signature::make(1, 1, sizeof (gr_complex) * interl_seq.size()),
              gr::io_signature::make(1, 1, sizeof (gr_complex) * interl_seq.size()))
	{
		d_seq = interl_seq;
		d_long_interl = long_interl;
		d_depth = depth;
		d_buffer.clear();
		d_tmp_buffer.clear();
	}

    /*
     * Our virtual destructor.
     */
    interleaver_vcvc_impl::~interleaver_vcvc_impl()
    {
    }

    int
    interleaver_vcvc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
	{
		const gr_complex *in = (const  gr_complex *) input_items[0];
		gr_complex *out = ( gr_complex *) output_items[0];
	
		// Tell runtime system how many input items we consumed on
		// each input stream.
		consume_each (1);
	
		/* For details on the cell interleaving process see chapter 7.6 in the DRM standard */
		if(d_long_interl) // perform long interleaving
		{
			// fill buffer as 'd_depth' consecutive transmission frames are needed to perform long interleaving
			for(int i = 0; i < d_seq.size(); i++)
			{
				d_tmp_buffer.push_back(in[i]);
			}
			d_buffer.push_back(d_tmp_buffer);
			d_tmp_buffer.clear();
		
			//std::cout << "d_buffer.size(): " << d_buffer.size() << ", d_depth: " << d_depth << ", d_seq.size(): " << d_seq.size() << std::endl;
			if( d_buffer.size() == d_depth ) // perform interleaving and delete the oldest frame that won't be necessary for the next step
			{
				//std::cout << "interleaving cells" << std::endl;
				for( int i = 0; i < d_seq.size(); i++)
				{
					//std::cout << "index i: " << i << ", accessing d_buffer[" << d_depth - (i % d_depth) - 1 << "][" << d_seq[i] << "]" << std::endl;
					out[i] = d_buffer[ d_depth - (i % d_depth) - 1][ d_seq[i] ];
				}
				//std::cout << "deleting first row in the vector vector matrix" << std::endl;
				d_buffer.erase( d_buffer.begin() ); // delete the first row (oldest frame)
			
				return 1; // return 1 transmission frame
			}
			else
			{
				std::cout << "cell interleaver init: filling buffer" << std::endl;
				return 0; // buffer is still being filled
			}
		}
		else // short interleaving
		{
			for(int i = 0; i < d_seq.size(); i++)
			{
				out[i] = in[ d_seq[i] ];
			}
			// Tell runtime system how many output items we produced.
			return 1;
		}
	}

  } /* namespace drm */
} /* namespace gr */

