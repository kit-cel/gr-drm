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

#ifndef INCLUDED_DRM_PARTITIONING_VBVB_IMPL_H
#define INCLUDED_DRM_PARTITIONING_VBVB_IMPL_H

#include <drm/partitioning_vbvb.h>

namespace gr {
  namespace drm {
    class DRM_API partitioning_vbvb_impl : public partitioning_vbvb
    {
    private:

      unsigned int d_n_out; // number of output ports
      std::vector<int> d_vlen_out; // vector with output vector lengths

    public:
     /*!
      * \brief Performs departitioning (demultiplexing) of the input vector
      * At the moment, only EEP is supported.
      */
      partitioning_vbvb_impl (unsigned int vlen_in, std::vector<int> vlen_out);
      ~partitioning_vbvb_impl ();


      int work (int noutput_items,
          gr_vector_const_void_star &input_items,
          gr_vector_void_star &output_items);
    };
  }
}
#endif /* INCLUDED_DRM_PARTITIONING_VBVB_IMPL_H */

