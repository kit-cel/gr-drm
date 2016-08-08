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


#ifndef INCLUDED_DRM_PARTITIONING_BB_H
#define INCLUDED_DRM_PARTITIONING_BB_H

#include <drm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace drm {

    /*!
     * \brief Performs departitioning (demultiplexing) of the input vector
 	 * At the moment, only EEP is supported.
     * \ingroup drm
     *
     */
    class DRM_API partitioning_bb : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<partitioning_bb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of drm::partitioning_bb.
       *
       * To avoid accidental use of raw pointers, drm::partitioning_bb's
       * constructor is in a private implementation
       * class. drm::partitioning_bb::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int vlen_in, std::vector<int> vlen_out);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_PARTITIONING_bb_H */

