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

#ifndef INCLUDED_DRM_GENERATE_FAC_VB_IMPL_H
#define INCLUDED_DRM_GENERATE_FAC_VB_IMPL_H

#include <drm/generate_fac_vb.h>
#include "drm_util.h"

namespace gr {
  namespace drm {

    class generate_fac_vb_impl : public generate_fac_vb
    {
    private:
      	transm_params* d_tp;
		unsigned short d_tf_ctr; // transmission frame counter

    public:
		generate_fac_vb_impl(transm_params* tp);
		~generate_fac_vb_impl();

		void init_data(unsigned char* data); //  set FAC bitstream according to config parameters (see DRM standard chapter 6.3)

		void increment_tf_ctr(); // increments tf_ctr and takes account of wraparound;

        // Where all the action really happens
     	int work(int noutput_items,
	        gr_vector_const_void_star &input_items,
	        gr_vector_void_star &output_items);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_GENERATE_FAC_VB_IMPL_H */

