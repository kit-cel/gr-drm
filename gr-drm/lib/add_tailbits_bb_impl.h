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

#ifndef INCLUDED_DRM_ADD_TAILBITS_BB_IMPL_H
#define INCLUDED_DRM_ADD_TAILBITS_BB_IMPL_H

#include <drm/add_tailbits_bb.h>


namespace gr {
  namespace drm {

    class add_tailbits_bb_impl : public add_tailbits_bb
    {
    private:
		int d_vlen;
		int d_n_tail;

    public:
		add_tailbits_bb_impl(int vlen_in, int n_tailbits);
		~add_tailbits_bb_impl();

      // Where all the action really happens
      	int general_work(int noutput_items,
            gr_vector_int &ninput_items,
	       	gr_vector_const_void_star &input_items,
	       	gr_vector_void_star &output_items);
        void forecast(int noutput_items, gr_vector_int &ninput_items_required);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_ADD_TAILBITS_bb_IMPL_H */

