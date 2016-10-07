/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "audio_decoder_bf_impl.h"

namespace gr {
  namespace drm {

    audio_decoder_bf::sptr
    audio_decoder_bf::make(transm_params* tp)
    {
      return gnuradio::get_initial_sptr
        (new audio_decoder_bf_impl(tp));
    }

    /*
     * The private constructor
     */
    audio_decoder_bf_impl::audio_decoder_bf_impl(transm_params* tp)
      : gr::block("audio_decoder_bf",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(float))),
              d_tp(tp),
              d_L_MUX(tp->msc().L_MUX()),
              d_audio_samp_rate(tp->cfg().audio_samp_rate()),
              d_dec_handle(NULL)
    {
      open_decoder();
    }
    
    void audio_decoder_bf_impl::open_decoder()
    {
      int aac_samp_rate = 12000;
      if (d_dec_handle == NULL)
        d_dec_handle = NeAACDecOpen();
      if (d_dec_handle != NULL)
      {
        /* Only 12 kHz and 24 kHz is allowed */
        switch (d_audio_samp_rate)
        {
        case 12000:
	        aac_samp_rate = 12000;
	        break;

        case 24000:
	        aac_samp_rate = 24000;
	        break;

        default:
	        break;
        }
      /* Only allow MONO without SBR at the moment */
      NeAACDecInitDRM(&d_dec_handle, aac_samp_rate, (unsigned char)DRMCH_MONO);

      d_audio_samp_rate = aac_samp_rate;
      d_len_dec_out_per_chan = d_AUD_DEC_TRANSFORM_LENGTH;
      }
      else
      {
          throw std::runtime_error("Failed to open AAC decoder");
      }
    }

    short* audio_decoder_bf_impl::decode(unsigned char *frameptr, unsigned long len_frame)
    {
      short* audioptr = NULL;
      NeAACDecFrameInfo DecFrameInfo;
      DecFrameInfo.channels = 1;
      DecFrameInfo.error = 1;
      if (d_dec_handle != NULL)
      {
          audioptr = (short*) NeAACDecDecode(d_dec_handle, &DecFrameInfo, frameptr, len_frame);
      }
      if(DecFrameInfo.error != 0)
      {
        throw std::runtime_error("AAC Decoder error");
      }
      return audioptr;
    }

    /*
     * Our virtual destructor.
     */
    audio_decoder_bf_impl::~audio_decoder_bf_impl()
    {
    }

    void
    audio_decoder_bf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = d_L_MUX;
    }

    int
    audio_decoder_bf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in = (const char*) input_items[0];
      float *out = (float*) output_items[0];

      // TODO: find out how the audio frame length is determined (needed for the decoding)
      // TODO: find out how the audio is encoded into the buffer of shorts (just scaling to [-1, 1] required?)
      // TODO: find out what frame format NeAACDecDecode expects (something with CRC at the beginning)

      consume_each(noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace drm */
} /* namespace gr */

