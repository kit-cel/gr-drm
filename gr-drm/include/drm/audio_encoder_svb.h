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


#ifndef INCLUDED_DRM_AUDIO_ENCODER_SVB_H
#define INCLUDED_DRM_AUDIO_ENCODER_SVB_H

#include <drm/api.h>
#include <gnuradio/block.h>
#include "drm_transm_params.h"
#include "drm_util.h"
#include <faac.h>
#include <iostream>
#include <string>

namespace gr {
  namespace drm {

    /*!
     * \brief <+description of block+>
     * \ingroup drm
     *
     */
    class DRM_API audio_encoder_svb : virtual public gr::block
    {
     public:
  		typedef boost::shared_ptr<audio_encoder_svb> sptr;

		static sptr make(transm_params* tp);
		/*!
		* \brief Return a shared_ptr to a new instance of drm::audio_encoder_svb.
		*
		* To avoid accidental use of raw pointers, drm::audio_encoder_svb's
		* constructor is in a private implementation
		* class. drm::audio_encoder_svb::make is the public interface for
		* creating new instances.
		*/      	
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_AUDIO_ENCODER_SVB_H */

