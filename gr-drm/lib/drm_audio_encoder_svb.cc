/* -*- c++ -*- */
/* 
 * Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
 * Author: Felix Wunsch
 * 
 * The algorithm for the composition of the DRM compliant data has been taken
 * from DREAM (see http://sourceforge.net/apps/mediawiki/drm/index.php?title=Main_Page)
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
	d_out_start = NULL;
	
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
	
	int n_bits_audio_frame = n_bits_usage;
	if(tp->cfg().text())
	{
		n_bits_audio_frame -= 16; // if text message is carried, the last four bytes are used for this
	}

	d_n_bytes_audio_payload = n_bits_audio_frame / sizeof_byte - d_n_header_bytes - d_n_aac_frames /* for CRCs */ ;
	const int n_bytes_act_enc = (int) (d_n_bytes_audio_payload / d_n_aac_frames);
	int bit_rate = (int) (( n_bytes_act_enc * sizeof_byte) / d_time_aac_superframe * 1000);

    /* set encoder configuration */
	faacEncConfigurationPtr cur_enc_format;
	cur_enc_format = faacEncGetCurrentConfiguration(d_encHandle);
	cur_enc_format->inputFormat = FAAC_INPUT_FLOAT;
	cur_enc_format->useTns = 1;
	cur_enc_format->aacObjectType = LOW;
	cur_enc_format->mpegVersion = MPEG4;
	cur_enc_format->outputFormat = 0;	/* (0 = Raw; 1 = ADTS) */
	cur_enc_format->bitRate = bit_rate;
	cur_enc_format->bandWidth = 0;	/* Let the encoder choose the bandwidth */
	faacEncSetConfiguration(d_encHandle, cur_enc_format);
	
	/* fdk-aac encoder initialization */
	d_hAacEncoder = NULL;
	/* encoder handle */
	int ErrorStatus = AACENC_OK;
	if ( (ErrorStatus = aacEncOpen(&d_hAacEncoder,0x17,MODE_1)) != AACENC_OK ) // AAC + SBR, mono
	{
		std::cout << "Failed to open encoder instance!\n";
	}
	else
	{
		std::cout << "opened encoder instance.\n";
	}

	/* set parameters */
	if (aacEncoder_SetParam(d_hAacEncoder, AACENC_AOT, AOT_DRM_AAC) != AACENC_OK) // HeAAC (AAC-LC + SBR)
	{
		fprintf(stderr, "Unable to set the AOT\n");
	}
	if (aacEncoder_SetParam(d_hAacEncoder, AACENC_SAMPLERATE, tp->cfg().audio_samp_rate()) != AACENC_OK) // audio sample rate
	{
		fprintf(stderr, "Unable to set the sample rate\n");
	}
	/*if (aacEncoder_SetParam(d_hAacEncoder, AACENC_GRANULE_LENGTH, 960) != AACENC_OK) // 960 transform length
	{
		fprintf(stderr, "Unable to set the transform length to %li \n", d_transform_length);
	}*/
	if (aacEncoder_SetParam(d_hAacEncoder, AACENC_CHANNELMODE, MODE_1) != AACENC_OK) // 1 front channel
	{
		fprintf(stderr, "Unable to set the channel mode\n");
	}
	if (aacEncoder_SetParam(d_hAacEncoder, AACENC_BITRATE, bit_rate) != AACENC_OK) // set bitrate
	{
		fprintf(stderr, "Unable to set the bitrate\n");
	}
	if (aacEncoder_SetParam(d_hAacEncoder, AACENC_TRANSMUX, TT_DRM) != AACENC_OK) // raw
	{
		fprintf(stderr, "Unable to set the ADTS transmux\n");
	}/*
	if (aacEncoder_SetParam(d_hAacEncoder, AACENC_AFTERBURNER, 1) != AACENC_OK)  // afterburner is activated, deactivate for less cpu consumption
	{
		fprintf(stderr, "Unable to set the afterburner mode\n");
	}*/
	if (aacEncEncode(d_hAacEncoder, NULL, NULL, NULL, NULL) != AACENC_OK) // apply parameters
	{
		fprintf(stderr, "Unable to initialize the encoder\n");
	}
	AACENC_InfoStruct info;
	if (aacEncInfo(d_hAacEncoder, &info) != AACENC_OK) // get encoder info
	{
		fprintf(stderr, "Unable to get the encoder info\n");
	}
	
	// print info
	std::cout << "maxOutBufBytes: " << info.maxOutBufBytes << std::endl;
	std::cout << "maxAncBytes: " << info.maxAncBytes << std::endl;
	std::cout << "inBufFillLevel: " << info.inBufFillLevel << std::endl;
	std::cout << "inputChannels: " << info.inputChannels << std::endl;
	std::cout << "frameLength: " << info.frameLength << std::endl;
	std::cout << "encoderDelay: " << info.encoderDelay << std::endl;		
		
	// set text message if available
	if(d_tp->cfg().text())
	{
		d_text_msg = tp->cfg().text_message();
		d_text_ctr = 0;
		d_n_text_frames = 0;
		prepare_text_message();
		d_text_msg_index = n_bits_usage - 8 * 4; // set offset index for text message application
	}
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
	d_out_start = d_out;
	//std::cout << "start of work()" << std::endl;
	//std::cout << "d_out: " << (long) d_out << std::endl;
	unsigned char* d_out_prev = d_out;
	
	// set output buffer to zero (corresponds to zeropadding as defined in the DRM standard)
	memset(d_out, 0, sizeof(char) * d_L_MUX_MSC);
	
	/* encode PCM stream and make it DRM compliant. write to output buffer (in make_drm_compliant()) */
	// init AAC buffer
	//std::cout << "aac_encode()" << std::endl;
	unsigned char aac_buffer[(const unsigned long) d_n_max_bytes_out * (const unsigned long) d_n_aac_frames];
	aac_encode(aac_buffer); // encodes pcm data for 1 super transmission frame
	//std::cout << "d_out: " << (long) d_out << ", bits written: " << d_out - d_out_prev << std::endl;
	d_out_prev = d_out;
	
	//std::cout << "make_drm_compliant()" << std::endl;
	make_drm_compliant(aac_buffer); // reorders and processes the data produced by the encoder to be DRM compliant
	//std::cout << "d_out: " << (long) d_out << ", bits written: " << d_out - d_out_prev << std::endl;
	d_out_prev = d_out;	
	/* insert text message if available */
	if( d_tp->cfg().text())
	{
		//std::cout << "insert text()" << std::endl;
		insert_text_message();
		//std::cout << "d_out: " << (long) d_out << ", bits written: " << d_out - d_out_prev << std::endl;
	}

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
        //std::cout << "encoder: n_bytes_encoded: " << n_bytes_encoded << std::endl;
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

void
drm_audio_encoder_svb::prepare_text_message()
{
	//std::cout << "entering prepare()" << std::endl;
	/* prepare the text message string */
	

	//std::cout << "length of the original string: " << d_text_msg.size() << std::endl;
	// determine the number of segments, truncate if needed
	int n_segments = std::ceil( (float) d_text_msg.size()/16); // number of segments (16 bytes per segment)
	if ( n_segments > 8 ) // max number of segments is 8, truncate if necessary
	{
		std::cout << "Text message is too long! Message gets truncated to the maximal possible length.\n";
		n_segments = 8;
		d_text_msg.resize(8*16); // max number of segments * max number of bytes
	}
	//std::cout << "length of the resized string: " << d_text_msg.size() << std::endl;
	// zero-pad string if its length is not a multiple of four (bytes)
	int n_bytes_pad = (4 - d_text_msg.size() % 4) % 4;
	d_text_msg.append(n_bytes_pad, 0); // append zeros (0x00)
	
	int len_string_byte = d_text_msg.size(); // length of the string in bytes
	///std::cout << "len of padded string: " << len_string_byte << std::endl;
	
	// allocate bit array (unpacked) that will hold the real text message stream
	const int len_msg_bit = len_string_byte * 8 + n_segments * (16 + 32 + 16); // payload + header + leading zeros + CRC
	//std::cout << "len_msg_bit: " << len_msg_bit << std::endl;
	d_n_text_frames = len_msg_bit/(4*8);
	//std::cout << "d_n_text_frames: " << d_n_text_frames << ", n_segments: " << n_segments << std::endl;
	unsigned char msg[len_msg_bit]; 
	memset(msg, 9, len_msg_bit); // set to 9 for debugging purposes
	
	//std::cout << "msg address: " << (long) &msg[0] << std::endl;
	unsigned char* p_msg = &msg[0];
	
	// insert leading 0xFF bytes and header
	int ctr = 0; // byte-based counter
	int bits_written_total = 0;
	for(int i = 0; i < n_segments; i++)
	{
		int bits_written = 0;
		unsigned char* seg_ptr = p_msg; // pointer to the beginning of the current segment
		
		//std::cout << "insert leading ones and header" << std::endl;
		//std::cout << "msg address: " << (long) &msg[0] << std::endl;
		//std::cout << "segment " << i << ": " << std::endl;	
		/* beginning of the segment */
		enqueue_bits_dec(p_msg, 32, 0xFFFFFFFF); // 4 bytes, each set to 0xFF
		bits_written += 32;
		
		/* header */
		enqueue_bits_dec(p_msg, 1, 0); // toggle flag (changes when segments from different messages are transmitted)
		bits_written += 1;
		
		if(i == 0) // first flag
		{
			enqueue_bits_dec(p_msg, 1, 1); // first flag is set
		}
		else
		{
			enqueue_bits_dec(p_msg, 1, 0);
		}
		bits_written += 1;
		
		if(i == n_segments - 1) // last flag
		{
			enqueue_bits_dec(p_msg, 1, 1); // last flag is set
		}
		else
		{
			enqueue_bits_dec(p_msg, 1, 0);
		}
		enqueue_bits_dec(p_msg, 1, 0); // command flag (0 -> Field 1 signals the length of the body of the segment; if set to 1, field 2 is omitted)
		bits_written += 2;
		
		// field 1 (length of the segment in bytes, coded as unsigned number)
		if( i < n_segments - 1) // the last segment can hold less than 16 bytes of character data and has to be treated separately
		{
			enqueue_bits_dec(p_msg, 4, 15);
		}
		else
		{
			enqueue_bits_dec(p_msg, 4, len_string_byte - (n_segments-1) * 16 - 1);
		}
		bits_written += 4;
		
		// field 2
		if(i == 0) // first segment
		{
			enqueue_bits_dec(p_msg, 4, 0xF); // "1111"
		}
		else
		{
			enqueue_bits_dec(p_msg, 1, 0); // rfa
			enqueue_bits_dec(p_msg, 3, i); // segment number (1-7)
		}
		bits_written += 4;
		
		enqueue_bits_dec(p_msg, 4, 0); // rfa
		bits_written += 4;
		
		//std::cout << "insert body" << std::endl;
		//std::cout << "msg address: " << (long) &msg[0] << std::endl;
		/* body */
		if(i < n_segments - 1) // this is a 'full' segment
		{
			for(int j = 0; j < 16; j++)
			{
				enqueue_bits_dec(p_msg, 8, (unsigned char) d_text_msg[ctr]); // unpack one byte and write it to the stream
				bits_written += 8;
				ctr++;
			}
			enqueue_crc(seg_ptr + 32, d_tp, bits_written - 32, 162);
			p_msg += 16; // enqueue crc does not move the array pointer
			bits_written += 16;
		}
		else // last segment
		{
			for(int j = 0; j < len_string_byte - (n_segments-1) * 16; j++)
			{
				enqueue_bits_dec(p_msg, 8, (unsigned char) d_text_msg[ctr]);
				bits_written += 8;
				ctr++;
			}
			enqueue_crc(seg_ptr + 32, d_tp, bits_written - 32, 162);
			p_msg += 16; // enqueue crc does not move the array pointer
			bits_written += 16;
		}
		
		bits_written_total += bits_written;
		
		/*
		//std::cout << "prefix: ";
		for(int k = 0; k < 32; k++)
			std::cout << (int) seg_ptr[k];
		
		std::cout << std::endl;
		
		//std::cout << "header: ";
		for(int k = 0; k < 16; k++)
			std::cout << (int) seg_ptr[k+32];
		std::cout << std::endl;
		
		//std::cout << "body: ";
		for(int k = 0; k < bits_written - 32 - 16 - 16; k++)
			std::cout << (int) seg_ptr[k + 48];
		std::cout << std::endl;
		
		//std::cout << "CRC: ";
		for(int k = 0; k < 16; k++)
			std::cout << (int) seg_ptr[k + bits_written - 16];
		std::cout << std::endl;
		*/
	}
	
	//std::cout << "total bits written: " << bits_written_total << std::endl;
	
	//std::cout << "copy array into a vector" << std::endl;
	//std::cout << "msg address: " << (long) &msg[0] << std::endl;
	// copy the char array into a vector (more convenient)
	d_text_msg_fmt.assign(&msg[0], &msg[0] + len_msg_bit);
	//std::cout << "return from prepare()" << std::endl;
}

void
drm_audio_encoder_svb::insert_text_message()
{
	// text message handling (last 4 bytes of lower protected payload). For details see chapter 6.5 in the standard.
	d_out = d_out_start + d_text_msg_index; // set output buffer pointer to the beginning of the text message
	
	//std::cout << "write message to output stream" << std::endl;
	// determine the part of the message that is to be inserted in this call to work()
	//std::cout << "actually inserted bits (ctr: " << d_text_ctr << "): ";
	for(int i = 0; i < 32; i++)
	{
		*d_out++ = d_text_msg_fmt[d_text_ctr*32 + i];
		//std::cout << (int) *(d_out-1);	
		//if((i+1)%8 == 0){std::cout << std::endl;}
	}
	//std::cout << std::endl << std::endl;
	d_text_ctr = (d_text_ctr + 1) % d_n_text_frames;
	//std::cout << "return from insert()" << std::endl;
}














