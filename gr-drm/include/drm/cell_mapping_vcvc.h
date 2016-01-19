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


#ifndef INCLUDED_DRM_CELL_MAPPING_VCVC_H
#define INCLUDED_DRM_CELL_MAPPING_VCVC_H

#include <drm/api.h>
#include <gnuradio/sync_interpolator.h>
#include <cmath>
#include <complex>
#include "drm_transm_params.h"

namespace gr {
  namespace drm {

    /*!
     * \brief Performs mapping of MSC, SDC, FAC and pilot cells onto the OFDM super transmission frame.
     * \ingroup drm
     *
     */
    class DRM_API cell_mapping_vcvc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<cell_mapping_vcvc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of drm::cell_mapping_vcvc.
       *
       * To avoid accidental use of raw pointers, drm::cell_mapping_vcvc's
       * constructor is in a private implementation
       * class. drm::cell_mapping_vcvc::make is the public interface for
       * creating new instances.
       */
      static sptr make(transm_params* tp, std::vector< int > input_sizes);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_CELL_MAPPING_VCVC_H */

