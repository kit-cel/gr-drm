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

#ifndef INCLUDED_DRM_PUNCT_VBVB_IMPL_H
#define INCLUDED_DRM_PUNCT_VBVB_IMPL_H

#include <drm/punct_vbvb.h>

namespace gr
{
  namespace drm
  {
    class DRM_API punct_vbvb_impl : public punct_vbvb
    {
    private:
      int d_vlen_in; // input vector length
      int d_vlen_out; // output vector length
      std::vector<unsigned char> d_pp1; // puncturing pattern
      std::vector<unsigned char> d_pp2; // tail bit pattern
      int d_n_tail;

    public:
      /*!
       * \brief Punctures a vector of (unpacked) bytes according to punct_pat_x. For the last num_tailbits bits punct_pat_2 is used.
       *
       */
      punct_vbvb_impl(std::vector<unsigned char> punct_pat_1,
                      std::vector<unsigned char> punct_pat_2, int vlen_in, int vlen_out,
                      int num_tailbits);
      ~punct_vbvb_impl();

      int
      general_work(int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items);
    };
  }
}
#endif /* INCLUDED_DRM_PUNCT_VBVB_IMPL_H */

