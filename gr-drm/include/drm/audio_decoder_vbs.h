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

#ifndef INCLUDED_DRM_AUDIO_DECODER_VBS_H
#define INCLUDED_DRM_AUDIO_DECODER_VBS_H

#include <drm/api.h>
#include <gnuradio/block.h>
#include <drm/transm_params.h>

namespace gr
{
  namespace drm
  {
    class DRM_API audio_decoder_vbs : virtual public block
    {
    public:
      typedef boost::shared_ptr<audio_decoder_vbs> sptr;
      /*!
       * \brief Decodes MPEG4 AAC LC encoded data, outputs a stream of shorts (raw PCM).
       *
       */
      static sptr
      make(transm_params* tp);

      virtual void
      decode_text_message() = 0;
      virtual bool
      make_faad_compliant() = 0;
      virtual bool
      aac_decode() = 0;
    };
  }
}

#endif /* INCLUDED_DRM_AUDIO_DECODER_VBS_H */

