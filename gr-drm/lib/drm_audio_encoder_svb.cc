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
	memset(out, 0, n_in);
	
	// init temporary buffers
	gr_int16 tmp_in[(const unsigned int) d_n_samples_in]; // tmp buffer input
	unsigned char tmp_out[(const unsigned int) d_n_max_bytes_out]; // tmp buffer output
	memset(tmp_out, 0, d_n_max_bytes_out); //  set output buffer to zero
	unsigned char crc_bits[(const unsigned int) d_n_aac_frames];
	memset(crc_bits, 0, d_n_aac_frames);
	unsigned int frame_length[(const unsigned int) d_n_aac_frames];
	memset(frame_length, 0, d_n_aac_frames);
	unsigned char* audio_frame = new unsigned char[(const unsigned int) d_n_aac_frames * d_n_max_bytes_out]; // this does not work with static init
	unsigned char* audio_frame_start = audio_frame; // needed for delete[]
	
	// actual encoding
	for(int j = 0; j < d_n_aac_frames; j++)
	{			
		memcpy(tmp_in, in + j*d_n_samples_in, d_n_samples_in); // write input data to input tmp buffer	
		in += d_n_samples_in; // set pointer to the next block of samples			
		
		int n_bytes_encoded = faacEncEncode(d_encHandle, (int32_t*) tmp_in, d_n_samples_in, tmp_out, d_n_max_bytes_out);
		
		if(n_bytes_encoded > 0)
		{
			/* Extract CRC */

			crc_bits[j] = tmp_out[0];

			/* Extract actual data */
			memcpy(audio_frame, tmp_out + 1, n_bytes_encoded - 1); // copy encoded aac data to audio_frame
			audio_frame += n_bytes_encoded - 1; // set pointer to the beginning of the next audio frame
			
			/* Store block lengths for borders in AAC super-frame-header */
			frame_length[j] = n_bytes_encoded - 1;
		}
		else
		{
			/* Encoder is in initialization phase, reset CRC and length */
			crc_bits[j] = 0;
			frame_length[j] = 0;
		}
		
		
	}
	
	/* make AAC data DRM compliant */
	
	/* AAC super-frame header */
	int bits_written = 0; // counter to determine the net length of the super frame
    int acc_frame_length = 0;
    for (int j = 0; j < d_n_aac_frames - 1; j++)
    {
        acc_frame_length += frame_length[j];

        /* Frame border in bytes (12 bits) */
        enqueue_bits_dec(out, 12, acc_frame_length);
        bits_written += 12;
    }

    /* Byte-alignment (4 bits) in case of 10 audio frames */
    if (d_n_aac_frames == 10)
    {
    	enqueue_bits_dec(out, 4, 0);
        bits_written += 4;
    }

    /* Higher protected part */
    int cur_num_bytes = 0;
    int d_n_bytes_higher_prot = 0; // EEP FIXME: add UEP support here!!

	// write to output buffer

	delete[] audio_frame_start;
	
	// Tell runtime system how many input items we consumed on
	// each input stream.
	consume_each (d_n_samples_in * d_n_aac_frames);
	// Tell runtime system how many output items we produced.
	return 1; // n_aac_frames super audio frames -> 1 transmission frame was produced
	// TODO: process multiple vectors per call to general_work()
}

