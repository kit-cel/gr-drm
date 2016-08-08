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

#ifndef INCLUDED_DRM_INTERLEAVER_CC_IMPL_H
#define INCLUDED_DRM_INTERLEAVER_CC_IMPL_H

#include <drm/interleaver_cc.h>

namespace gr {
  namespace drm {

    class interleaver_cc_impl : public interleaver_cc
    {
    private:
		std::vector<int> d_seq;
		bool d_long_interl;
		int d_depth;
		std::vector< std::vector<gr_complex> > d_buffer;
		std::vector< gr_complex > d_tmp_buffer;

    public:
		interleaver_cc_impl(std::vector<int> interl_seq, bool long_interl, int depth);
		~interleaver_cc_impl();

      	// Where all the action really happens
      	int general_work(int noutput_items,
			gr_vector_int &ninput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_INTERLEAVER_cc_IMPL_H */

