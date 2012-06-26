/* -*- c++ -*- */
/* 
 * Copyright 2012 CEL KIT.
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

#include <gr_io_signature.h>
#include <drm_audio_encoder_sb.h>


drm_audio_encoder_sb_sptr
drm_make_audio_encoder_sb (transm_params* tp)
{
	return drm_audio_encoder_sb_sptr (new drm_audio_encoder_sb (tp));
}

const short MIN_IN = 1; // only mono input
const short MAX_IN = 1;
const short MIN_OUT = 1; // only mono output
const short MAX_OUT = 1;

drm_audio_encoder_sb::drm_audio_encoder_sb (transm_params* tp)
	: gr_block ("audio_encoder_sb",
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_int16)),
		gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (unsigned char)))
{
	//switch(tp->cfg()->audio_samp_rate())
	unsigned int audio_samp_hardcoded = 24000; // FIXME: audio sample rate hard coded, add to config
	switch(audio_samp_hardcoded)
	{
	case 12000:
		d_n_aac_frames = 5;
		d_time_aac_superframe = 80;
		d_n_header_bytes = 6;
		break;
	case 24000:
		d_n_aac_frames= 10;
		d_time_aac_superframe = 40;
		d_n_header_bytes = 14;
		break;
	default:
		std::cout << "Unsupported audio sample rate!\n";
		break;
	}

	d_transform_length = 960; // see DRM standard
	d_n_channels = 1; // mono
	d_L_MUX_MSC = (tp->msc()).L_MUX();
}


drm_audio_encoder_sb::~drm_audio_encoder_sb ()
{
}


int
drm_audio_encoder_sb::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
	const gr_int16 *in = (const gr_int16 *) input_items[0];
	unsigned char *out = (unsigned char *) output_items[0];
  
	// open encoder
	faacEncHandle encHandle;
	encHandle = faacEncOpen(24000, d_n_channels, &d_n_samples_in, &d_n_max_bytes_out);     // FIXME: remove hardcoded value
	std::cout << "samples in:\t" << d_n_samples_in << "\t max_bytes_out:\t" << d_n_max_bytes_out << std::endl;
	if(encHandle == NULL)
	{
		std::cout << "FAAC encoder instance could not be opened. Exit.\n";
	}
	
	// init output buffer to zero (as defined in the DRM standard)
	for(int i = 0; i < d_L_MUX_MSC; i++)
	{
		out[i] = 0;
	}
	
	// configure encoder

	// actual encoding

	// write to output buffer

	// Tell runtime system how many input items we consumed on
	// each input stream.
	consume_each (noutput_items);

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

