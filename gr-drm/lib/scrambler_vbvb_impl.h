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

#ifndef INCLUDED_DRM_scrambler_vbvb_IMPL_H
#define INCLUDED_DRM_scrambler_vbvb_IMPL_H

#include <drm/scrambler_vbvb.h>
#include <iostream>

namespace gr
{
  namespace drm
  {
    class DRM_API scrambler_vbvb_impl : public scrambler_vbvb
    {
    private:
      unsigned int d_block_len;
    public:
     /*!
      * \brief Scrambles the input sequence according to the following polynomial:
      * G(x) = x^9 + x^5 + 1. The scrambler gets reset after block_len samples.
      */
      scrambler_vbvb_impl(unsigned int block_len);
      ~scrambler_vbvb_impl();

      unsigned int block_len();

      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);
    };
  }
}
#endif /* INCLUDED_DRM_scrambler_vbvb_IMPL_H */

