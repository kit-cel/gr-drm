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

#ifndef INCLUDED_DRM_AUDIO_ENCODER_SVB_H
#define INCLUDED_DRM_AUDIO_ENCODER_SVB_H

#include <drm/api.h>
#include <gnuradio/block.h>
#include <drm/transm_params.h>

namespace gr
{
  namespace drm
  {
    class DRM_API audio_encoder_svb : virtual public block
    {
    public:
      typedef boost::shared_ptr<audio_encoder_svb> sptr;
      /*!
       * \brief Audio source encoder using FAAC to produce AAC stream (LC, mono)
       * Input: PCM stream (float), output: bit vector (unpacked, unsigned chars)
       */
      static sptr
      make(transm_params* tp);

      virtual void
      forecast(int noutput_items, gr_vector_int &ninput_items_required) = 0;
      virtual void
      aac_encode(unsigned char* aac_buffer) = 0; // encodes PCM stream to AAC
      virtual void
      make_drm_compliant(unsigned char* aac_buffer) = 0; // reorders and processes the encoded AAC data to make it DRM compliant. writes to output buffer.
      virtual void
      prepare_text_message() = 0; // format text message string according to the DRM standard
      virtual void
      insert_text_message() = 0; // insert text message
    };
  }
}

#endif /* INCLUDED_DRM_AUDIO_encoder_svb_H */

