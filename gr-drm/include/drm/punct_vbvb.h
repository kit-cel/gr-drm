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


#ifndef INCLUDED_DRM_PUNCT_VBVB_H
#define INCLUDED_DRM_PUNCT_VBVB_H

#include <drm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace drm {

    /*!
     * \brief Punctures a vector of (unpacked) bytes according to punct_pat_1. For the last num_tailbits bits punct_pat_2 is used.
     * \ingroup drm
     *
     */
    class DRM_API punct_vbvb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<punct_vbvb> sptr;

      /*!
       * \brief Punctures a vector of (unpacked) bytes according to punct_pat_1. For the last num_tailbits bits punct_pat_2 is used.
       *
       * To avoid accidental use of raw pointers, drm::punct_vbvb's
       * constructor is in a private implementation
       * class. drm::punct_vbvb::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::vector<unsigned char> punct_pat_1, std::vector<unsigned char> punct_pat_2, int vlen_in, int vlen_out, int num_tailbits);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_PUNCT_VBVB_H */

