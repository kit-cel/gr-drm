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
#include <fstream>


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
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (float)),
		gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (unsigned char) * tp->msc().L_MUX()))
{
	// set buffer pointers to NULL
	d_in = NULL;
	d_out = NULL;
	
	// define variables depending on input parameters
	
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
	d_encHandle = faacEncOpen(d_tp->cfg().audio_samp_rate(), d_n_channels, &d_transform_length, &d_n_max_bytes_out);

	if(d_encHandle == NULL)
	{
		std::cout << "FAAC encoder instance could not be opened. Exit.\n";
	}
	
	// configure encoder
	int sizeof_byte = 8;
	int n_bits_usage = (d_L_MUX_MSC / sizeof_byte) * sizeof_byte;
	int n_bytes_usage = n_bits_usage / sizeof_byte;

	if(d_tp->cfg().text()) // FIXME order of initialization
	{
		n_bytes_usage = n_bits_usage / sizeof_byte - 4; // last 4 bytes are used for text messaging
	}
	else
	{
		n_bytes_usage = n_bits_usage / sizeof_byte;
	}
	
	int n_bits_audio_frame = n_bits_usage; // no text message included! FIXME

	d_n_bytes_audio_payload = n_bits_audio_frame / sizeof_byte - d_n_header_bytes - d_n_aac_frames /* for CRCs */ ;
	const int n_bytes_act_enc = (int) (d_n_bytes_audio_payload / d_n_aac_frames);
	int bit_rate = (int) (( n_bytes_act_enc * sizeof_byte) / d_time_aac_superframe * 1000);

    /* set encoder configuration */
	faacEncConfigurationPtr cur_enc_format;
	cur_enc_format = faacEncGetCurrentConfiguration(d_encHandle);
	cur_enc_format->inputFormat = FAAC_INPUT_FLOAT; // TODO: check if float can be used here directly
	cur_enc_format->useTns = 1;
	cur_enc_format->aacObjectType = LOW;
	cur_enc_format->mpegVersion = MPEG4;
	cur_enc_format->outputFormat = 0;	/* (0 = Raw; 1 = ADTS) */
	cur_enc_format->bitRate = bit_rate;
	cur_enc_format->bandWidth = 0;	/* Let the encoder choose the bandwidth */
	faacEncSetConfiguration(d_encHandle, cur_enc_format);
}


drm_audio_encoder_svb::~drm_audio_encoder_svb ()
{
}

void
drm_audio_encoder_svb::forecast(int noutput_items, gr_vector_int &ninput_items_required)
{
	unsigned ninputs = ninput_items_required.size ();
    for (unsigned i = 0; i < ninputs; i++)
    {
        ninput_items_required[i] = d_n_aac_frames * d_transform_length;
    }
}

int
drm_audio_encoder_svb::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
	/* return if there are not enough samples to produce 1 super audio frame */
	if(ninput_items[0] < d_n_aac_frames * d_transform_length)
	{
		std::cout << "aac_encoder: not enough samples. got " << ninput_items[0] 
		<< ", need " << d_n_aac_frames * d_transform_length << ". returning.\n";
		return 0;
	}
	
	/* set pointers to input and output buffer */
	d_in = (float*) input_items[0];
	d_out = (unsigned char*) output_items[0];
	unsigned char* out_start = d_out;
	
	// set output buffer to zero (corresponds to zeropadding as defined in the DRM standard)
	memset(d_out, 0, sizeof(char) * d_L_MUX_MSC);
	
	/* encode PCM stream and make it DRM compliant. write to output buffer (in make_drm_compliant()) */
	// init AAC buffer
	unsigned char aac_buffer[(const unsigned long) d_n_max_bytes_out * (const unsigned long) d_n_aac_frames];
	aac_encode(aac_buffer); // encodes pcm data for 1 super transmission frame
	
	make_drm_compliant(aac_buffer); // reorders and processes the data produced by the encoder to be DRM compliant

	/* Call consume each and return */
	consume_each (d_transform_length * d_n_aac_frames);
	
	return 1; // n_aac_frames super audio frames -> 1 transmission frame was produced
}

void
drm_audio_encoder_svb::aac_encode(unsigned char* aac_buffer)
{
	// clear d_n_bytes_encoded
	d_n_bytes_encoded.clear();
	
	// allocate tmp input buffers for PCM and AAC samples
	float tmp_pcm_buffer[(const unsigned long) d_transform_length]; // if multiple super audio frames are processed, move this outside this function to avoid multiple allocation
	unsigned char tmp_aac_buffer[(const unsigned long) d_n_max_bytes_out];
	
	for (int j = 0; j < d_n_aac_frames; j++)
	{
		/* copy the part of the input data that is converted in this iteration */
		memcpy(tmp_pcm_buffer, d_in + j*d_transform_length, d_transform_length * sizeof(float));

		/* actual encoding */
		int n_bytes_encoded = faacEncEncode(d_encHandle, (int32_t*) tmp_pcm_buffer, d_transform_length, tmp_aac_buffer, d_n_max_bytes_out);
        d_n_bytes_encoded.push_back(n_bytes_encoded);
        memcpy(aac_buffer, tmp_aac_buffer, n_bytes_encoded * sizeof(char));
        aac_buffer += n_bytes_encoded;
	}
}

void
drm_audio_encoder_svb::make_drm_compliant(unsigned char* aac_buffer)
{
	/* init buffers for CRC, payload and frame lengths */
	unsigned char crc_bits[(const int) d_n_aac_frames];
	int frame_length[(const int) d_n_aac_frames];
	unsigned char* frame_pos[(const int) d_n_aac_frames];
	frame_pos[0] = aac_buffer;
	
	/* create header ( accumulated frame lengths | padding | CRC | audio ) */
	
	// calculate frame positions (pointer arithmetic!)
	for(int i = 1; i < d_n_aac_frames; i++)
	{
		if(d_n_bytes_encoded[i] > 0)
		{
			frame_pos[i] =  frame_pos[i-1] + d_n_bytes_encoded[i-1]; // CRC + data
		}
		else
		{
			frame_pos[i] =  frame_pos[i-1]; // no data was encoded in this step
		}
	}
	
	
	// extract frame lengths and CRC
	for(int i = 0; i < d_n_aac_frames; i++)
	{
		if(d_n_bytes_encoded[i] > 0)
		{
			crc_bits[i] = *(frame_pos[i]);
			frame_length[i] = d_n_bytes_encoded[i] - 1; // -1 for CRC
		}
		else
		{
			crc_bits[i] = 0;
			frame_length[i] = 0;
		}
		std::cout << "enc frame length: " << frame_length[i] << ", CRC: " << (int) crc_bits[i] << std::endl;
	}
	
	// append accumulated frame lengths, i. e. the frame borders
	unsigned int acc_frame_length = 0;
	for(int i = 0; i < d_n_aac_frames - 1; i++)
	{
		acc_frame_length += frame_length[i];
		enqueue_bits_dec(d_out, 12, acc_frame_length);
	}
	
	if(d_n_aac_frames == 10)
	{
		// add 4 padding bits for byte alignment
		enqueue_bits_dec(d_out, 4, 0);
	}
	
	// append CRC words
	for(int i = 0; i < d_n_aac_frames; i++)
	{
		enqueue_bits_dec(d_out, 8, crc_bits[i]);
	}
	
	/* append audio data (EEP is assumed -> no higher protected part) */
	/* Higher protected part */
    int cur_num_bytes = 0;
    int n_higher_prot_bytes = 0; // EEP

    /* Lower protected part */
    for (int j = 0; j < d_n_aac_frames; j++)
    {
        for (int i = n_higher_prot_bytes; i < frame_length[j]; i++)
        {
            /* If encoder produced too many bits, we have to drop them */
            if (cur_num_bytes < d_n_bytes_audio_payload){
                enqueue_bits_dec(d_out, 8, *(frame_pos[j] + 1 + i));
            }
            cur_num_bytes++;
        }
    }
}

