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


#ifndef INCLUDED_DRM_INTERLEAVER_CC_H
#define INCLUDED_DRM_INTERLEAVER_CC_H

#include <drm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace drm {

    /*!
     * \brief Performs cell interleaving. The long_interl flag determines if long interleaving is used.
     * \ingroup drm
     *
     */
    class DRM_API interleaver_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<interleaver_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of drm::interleaver_cc.
       *
       * To avoid accidental use of raw pointers, drm::interleaver_cc's
       * constructor is in a private implementation
       * class. drm::interleaver_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::vector<int> interl_seq, bool long_interl, int depth);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_INTERLEAVER_cc_H */

