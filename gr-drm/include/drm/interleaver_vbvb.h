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


#ifndef INCLUDED_DRM_INTERLEAVER_VBVB_H
#define INCLUDED_DRM_INTERLEAVER_VBVB_H

#include <drm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace drm {

    /*!
     * \brief Interleaves a sequence of the same length as the interleaver sequence as specified in the sequence (representing the interleaved indexes)
     * \ingroup drm
     *
     */
    class DRM_API interleaver_vbvb : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<interleaver_vbvb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of drm::interleaver_vbvb.
       *
       * To avoid accidental use of raw pointers, drm::interleaver_vbvb's
       * constructor is in a private implementation
       * class. drm::interleaver_vbvb::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::vector<int> interl_seq);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_INTERLEAVER_VBVB_H */

