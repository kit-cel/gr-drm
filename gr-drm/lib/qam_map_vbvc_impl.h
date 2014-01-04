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

#ifndef INCLUDED_DRM_QAM_map_vbvc_IMPL_H
#define INCLUDED_DRM_QAM_map_vbvc_IMPL_H

#include <drm/qam_map_vbvc.h>

namespace gr {
  namespace drm {
    class DRM_API qam_map_vbvc_impl : public qam_map_vbvc
    {
    private:
      float d_map_table[8][2]; // this table always has the maximum size, but only the relevant fields will be filled
      int d_bits_per_symbol;
      int d_vlen_out;
      int d_n_inputs;
      std::vector<int> d_pow2;
     public:
      /*!
       * \brief General QAM (symmetrical) mapping. Input is a matrix with bits_per_symbol (indexing through decimal interpretation of the bits) rows and 2 columns (I/Q).
       * Works only for symmetrical signal constellations.
       *
       */
      qam_map_vbvc_impl (const float map_table[][2], int bits_per_symbol, int vlen_out, int n_inputs);
      ~qam_map_vbvc_impl ();

      int work (int noutput_items,
          gr_vector_const_void_star &input_items,
          gr_vector_void_star &output_items);
    };
  }
}

#endif /* INCLUDED_DRM_QAM_map_vbvc_IMPL_H */

