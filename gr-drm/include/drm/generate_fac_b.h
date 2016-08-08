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


#ifndef INCLUDED_DRM_GENERATE_FAC_B_H
#define INCLUDED_DRM_GENERATE_FAC_B_H

#include <drm/api.h>
#include <gnuradio/sync_block.h>
#include "drm_transm_params.h"

namespace gr {
  namespace drm {

    /*!
     * \brief Generates the Fast Access Channel (FAC) data
     * \ingroup drm
     *
     */
    class DRM_API generate_fac_b : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<generate_fac_b> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of drm::generate_fac_b.
       *
       * To avoid accidental use of raw pointers, drm::generate_fac_b's
       * constructor is in a private implementation
       * class. drm::generate_fac_b::make is the public interface for
       * creating new instances.
       */
      static sptr make(transm_params* tp);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_GENERATE_FAC_b_H */

