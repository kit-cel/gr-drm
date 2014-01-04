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

#ifndef INCLUDED_DRM_INTERLEAVER_VBVB_IMPL_H
#define INCLUDED_DRM_INTERLEAVER_VBVB_IMPL_H

#include <drm/interleaver_vbvb.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace drm {
    class DRM_API interleaver_vbvb_impl : public interleaver_vbvb
    {
    private:

      std::vector< int > d_seq; // interleaver sequence representing the interleaved indices
      int d_seqsize; // stores the length of the sequence

    public:
      /*!
       * \brief Interleaves a sequence of the same length as the interleaver sequence as specified in the sequence (representing the interleaved indexes)
       *
       */
      interleaver_vbvb_impl (std::vector<int> interl_seq);
      ~interleaver_vbvb_impl ();


      int work (int noutput_items,
                gr_vector_const_void_star &input_items,
                gr_vector_void_star &output_items);
    };
  }
}
#endif /* INCLUDED_DRM_INTERLEAVER_VBVB_IMPL_H */

