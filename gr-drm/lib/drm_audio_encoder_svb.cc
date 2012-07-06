/* -*- c++ -*- */
/* TODO: add Dream copyright notice!
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
#include <drm_audio_encoder_svb.h>


drm_audio_encoder_svb_sptr
drm_make_audio_encoder_svb (transm_params* tp)
{
	return drm_audio_encoder_svb_sptr (new drm_audio_encoder_svb (tp));
}

const short MIN_IN = 1; // only mono input
const short MAX_IN = 1;
const short MIN_OUT = 1; // only mono output
const short MAX_OUT = 1;

drm_audio_encoder_svb::drm_audio_encoder_svb (transm_params* tp)
	: gr_block ("audio_encoder_svb",
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_int16)),
		gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (unsigned char) * tp->msc().L_MUX()))
{
	d_tp = tp;
	switch(tp->cfg().audio_samp_rate())
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
	case 48000:
		std::cout << "RM E not yet supported!\n";
		break;
	default:
		std::cout << "Unsupported audio sample rate!\n";
		break;
	}

	d_transform_length = 960; // see DRM standard
	d_n_channels = 1; // mono
	d_L_MUX_MSC = (tp->msc()).L_MUX();
	
	// open encoder
	d_encHandle = faacEncOpen(d_tp->cfg().audio_samp_rate(), d_n_channels, &d_n_samples_in, &d_n_max_bytes_out);
	std::cout << "samples in:\t" << d_n_samples_in << "\t max_bytes_out:\t" << d_n_max_bytes_out << std::endl;
	
	if(d_encHandle == NULL)
	{
		std::cout << "FAAC encoder instance could not be opened. Exit.\n";
	}
	
	// configure encoder
	int sizeof_byte = 8;
	int iTotNumBitsForUsage = (d_L_MUX_MSC / sizeof_byte) * sizeof_byte;
	int iTotNumBytesForUsage = iTotNumBitsForUsage / sizeof_byte;
	
	if(d_tp->cfg().text())
	{
		iTotNumBytesForUsage = iTotNumBitsForUsage / sizeof_byte - 4; // last 4 bytes are used for text messaging
	}
	else
	{
		iTotNumBytesForUsage = iTotNumBitsForUsage / sizeof_byte;
	}
	
	int iTotAudFraSizeBits = iTotNumBitsForUsage; // no text message included!

	int iAudioPayloadLen = iTotAudFraSizeBits / sizeof_byte - d_n_header_bytes - d_n_aac_frames /* for CRCs */ ;
	const int iActEncOutBytes = (int) (iAudioPayloadLen / d_n_aac_frames);
	int iBitRate = (int) (( iActEncOutBytes * sizeof_byte) / d_time_aac_superframe * 1000);
    
    /* set encoder configuration */
	faacEncConfigurationPtr CurEncFormat;
	CurEncFormat = faacEncGetCurrentConfiguration(d_encHandle);
	CurEncFormat->inputFormat = FAAC_INPUT_16BIT; // TODO: check if float can be used here directly
	CurEncFormat->useTns = 1;
	CurEncFormat->aacObjectType = LOW;
	CurEncFormat->mpegVersion = MPEG4;
	CurEncFormat->outputFormat = 0;	/* (0 = Raw; 1 = ADTS) */
	CurEncFormat->bitRate = iBitRate;
	CurEncFormat->bandWidth = 0;	/* Let the encoder choose the bandwidth */
	faacEncSetConfiguration(d_encHandle, CurEncFormat);
}


drm_audio_encoder_svb::~drm_audio_encoder_svb ()
{
}


int
drm_audio_encoder_svb::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
	gr_int16 *in = (gr_int16 *) input_items[0];
	unsigned char *out = (unsigned char *) output_items[0];
	unsigned int n_in = (unsigned int) ninput_items[0];
	
	// init output buffer to zero (as defined in the DRM standard)
	for(int i = 0; i < n_in; i++) // truncate to whole super transmission frames
	{
		out[i] = i;
	}

	// actual encoding
		
	// write to output buffer
	
	// Tell runtime system how many input items we consumed on
	// each input stream.
	consume_each (d_n_samples_in * d_n_aac_frames);
	// Tell runtime system how many output items we produced.
	return 1; // n_aac_frames super audio frames -> 1 transmission frame was produced
	// TODO: process multiple vector per call to general_work()
}

