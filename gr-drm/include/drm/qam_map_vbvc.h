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


#ifndef INCLUDED_DRM_QAM_MAP_VBVC_H
#define INCLUDED_DRM_QAM_MAP_VBVC_H

#include <drm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace drm {

    /*!
     * \brief General QAM (symmetrical) mapping. Input is a matrix with bits_per_symbol (indexing through decimal interpretation of the bits) rows and 2 columns (I/Q).
	 * Works only for symmetrical signal constellations.
     * \ingroup drm
     *
     */
    class DRM_API qam_map_vbvc : virtual public gr::sync_block
    {
    public:
    	typedef boost::shared_ptr<qam_map_vbvc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of drm::qam_map_vbvc.
       *
       * To avoid accidental use of raw pointers, drm::qam_map_vbvc's
       * constructor is in a private implementation
       * class. drm::qam_map_vbvc::make is the public interface for
       * creating new instances.
       */
    	static sptr make(const float map_table[][2], int bits_per_symbol, int vlen_out, int n_inputs);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_QAM_MAP_VBVC_H */

