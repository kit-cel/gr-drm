/* -*- c++ -*- */
/* 
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
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

#include <drm_api.h>
#include <gr_block.h>
#include "drm_transm_params.h"
#include "drm_util.h"
#include "neaacdec.h"

class drm_audio_decoder_vbs;
typedef boost::shared_ptr<drm_audio_decoder_vbs> drm_audio_decoder_vbs_sptr;

DRM_API drm_audio_decoder_vbs_sptr drm_make_audio_decoder_vbs (transm_params* tp);

/*!
 * \brief Decodes MPEG4 AAC LC encoded data, outputs a stream of shorts (raw PCM).
 *
 */
class DRM_API drm_audio_decoder_vbs : public gr_block
{
	friend DRM_API drm_audio_decoder_vbs_sptr drm_make_audio_decoder_vbs (transm_params* tp);

	drm_audio_decoder_vbs (transm_params* tp);
	
	transm_params* d_tp;
	unsigned char* d_in;
	gr_int16* d_out;
	
	NeAACDecHandle d_dec_handle;
	
	int d_n_borders;
	int d_n_aac_frames;
	int d_n_header_bytes;
	int d_audio_samp_rate;
	int d_transform_length;
	int* d_frame_length;
	int d_n_bytes_audio_payload;
	unsigned char* d_crc_words;
	std::vector< std::vector<unsigned char> > d_decoder_in;

 public:
	~drm_audio_decoder_vbs ();

    void decode_text_message();
    
    bool make_faad_compliant();
    
    bool aac_decode();

    int general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_AUDIO_DECODER_VBS_H */

