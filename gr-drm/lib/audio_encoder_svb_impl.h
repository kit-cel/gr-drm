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

#ifndef INCLUDED_DRM_AUDIO_ENCODER_SVB_IMPL_H
#define INCLUDED_DRM_AUDIO_ENCODER_SVB_IMPL_H

#include <drm/audio_encoder_svb.h>

namespace gr {
  namespace drm {

    class audio_encoder_svb_impl : public audio_encoder_svb
    {
     private:
		//audio_encoder_svb_impl (transm_params* tp);

		/* member variables */
		float* d_in; // input buffer
		unsigned char* d_out; // output buffer
		unsigned char* d_out_start; // start of output buffer
		int d_n_aac_frames; // number of AAC frames per super audio frame
		int d_time_aac_superframe; // in ms
		int d_n_header_bytes; // number of header bytes
		unsigned int d_n_channels; // number of channels (mono/stereo/...)
		unsigned long d_transform_length; // number of input samples per conversion step (transform length, set by encoder)
		unsigned long d_n_max_bytes_out; // max number of bytes (encoder output, set by encoder)
		int d_n_bytes_audio_payload; // number of bytes of audio payload
		unsigned int d_L_MUX_MSC; // number of decoded bits MSC

		std::string d_text_msg; // text message to be appended to the audio stream
		std::vector< unsigned char > d_text_msg_fmt; // formatted text message
		int d_text_ctr; // counter for the current text frame as per call to work() only a small part of the text stream is processed
		int d_n_text_frames; // number of text frames (4 byte pieces)
		int d_text_msg_index; // indicates the distance from the beginning of the buffer where the text message shall be positioned

		std::vector<int> d_n_bytes_encoded; // return value of faacEncEncode(). Used to determine frame lengths.
		faacEncHandle d_encHandle;
		transm_params* d_tp; // transmission params (holding config/msc/... values)
      // Nothing to declare in this block.

     public:
		audio_encoder_svb_impl(transm_params* tp);
		~audio_encoder_svb_impl();    

		void aac_encode(unsigned char* aac_buffer); // encodes PCM stream to AAC
		void make_drm_compliant(unsigned char* aac_buffer); // reorders and processes the encoded AAC data to make it DRM compliant. writes to output buffer.
		void prepare_text_message(); // format text message string according to the DRM standard
		void insert_text_message(); // insert text message
		int general_work (int noutput_items,
			gr_vector_int &ninput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_AUDIO_ENCODER_SVB_IMPL_H */

