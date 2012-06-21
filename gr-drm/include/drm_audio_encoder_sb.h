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

#ifndef INCLUDED_DRM_AUDIO_ENCODER_SB_H
#define INCLUDED_DRM_AUDIO_ENCODER_SB_H

#include <drm_api.h>
#include <gr_block.h>
#include "drm_transm_params.h"

class drm_audio_encoder_sb;
typedef boost::shared_ptr<drm_audio_encoder_sb> drm_audio_encoder_sb_sptr;

DRM_API drm_audio_encoder_sb_sptr drm_make_audio_encoder_sb (transm_params* tp);

/*!
 * \brief Audio source encoder using FAAC to produce AAC stream (LC, mono)
 *
 */
class DRM_API drm_audio_encoder_sb : public gr_block
{
	friend DRM_API drm_audio_encoder_sb_sptr drm_make_audio_encoder_sb (transm_params* tp);

	drm_audio_encoder_sb (transm_params* tp);

 public:
	~drm_audio_encoder_sb ();


  int general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_AUDIO_ENCODER_SB_H */

