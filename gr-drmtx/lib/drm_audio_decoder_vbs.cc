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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <drm_audio_decoder_vbs.h>
#include <fstream>


drm_audio_decoder_vbs_sptr
drm_make_audio_decoder_vbs (transm_params* tp)
{
	return drm_audio_decoder_vbs_sptr (new drm_audio_decoder_vbs (tp));
}


drm_audio_decoder_vbs::drm_audio_decoder_vbs (transm_params* tp)
	: gr_block ("audio_decoder_vbs",
		gr_make_io_signature (1, 1, sizeof (unsigned char) * tp->msc().L_MUX() ),
		gr_make_io_signature (1, 1, sizeof (gr_int16)))
{
	d_tp = tp;
	d_in = (unsigned char*) 0x1;
	d_out = NULL;
	d_audio_samp_rate = tp->cfg().audio_samp_rate();
	switch(d_audio_samp_rate)
	{
		case 12000:
			d_n_header_bytes = 6;
			d_n_aac_frames = 5;
			break;
		case 24000:
			d_n_header_bytes = 14;
			d_n_aac_frames = 10;
			break;
		default:
			std::cout << "AAC Decoder: Invalid sample rate!\n";
			break;
	}
	d_n_borders = d_n_aac_frames - 1;
	
	/* open and configure decoder instance */
	unsigned char drm_channel_mode = DRMCH_MONO; // used for decoding (mono, no SBR)
	int n_bytes_higher_protected;
	d_transform_length = 960;
	d_n_bytes_audio_payload = d_tp->msc().L_MUX() / 8 - d_n_header_bytes - d_n_aac_frames;
	if(d_tp->cfg().text()) // if text message is used, the last four bytes contain the text message
	{
		d_n_bytes_audio_payload -= 4;
	}
	int max_bytes_per_frame = d_n_bytes_audio_payload; // this is just a theoretical upper bound
	
	d_dec_handle = NEAACDECAPI NeAACDecOpen();
	NeAACDecConfigurationPtr conf;

	NEAACDECAPI NeAACDecInitDRM(&d_dec_handle, d_audio_samp_rate, drm_channel_mode);
	conf = NeAACDecGetCurrentConfiguration(d_dec_handle);	
	std::cout << "obj type: " << (int) conf->defObjectType << std::endl;
	std::cout << "defSampleRate: " << conf->defSampleRate << std::endl;
	std::cout << "output format: " << (int) conf->outputFormat << std::endl;
	std::cout << "down matrix: " << (int) conf->downMatrix << std::endl;
	std::cout << "old adts format: " << (int) conf->useOldADTSFormat << std::endl;
	std::cout << std::endl;
}


drm_audio_decoder_vbs::~drm_audio_decoder_vbs ()
{
}

void
drm_audio_decoder_vbs::decode_text_message()
{
	// do nothing as text messages are not implemented in the receiver
}

bool
drm_audio_decoder_vbs::make_faad_compliant()
{
	/* reshape data in a way faad2 can decode it (EEP is assumed) */
	
	//std::cout << "extract frame borders (" << d_n_borders << ")" << std::endl;
	/* AAC super frame header */
	unsigned int prev_border = 0;
	for(int i = 0; i < d_n_borders; i++)
	{
		unsigned int frame_border = dequeue_dec(d_in, 12);
		d_frame_length[i] = frame_border - prev_border; // distance between two consecutive chunks of aac bytes (does not include CRC)
		std::cout << "decoder frame length: " << d_frame_length[i] << std::endl;
		prev_border = frame_border;
	}
	
	/* byte alignment */
	if(d_n_borders == 9)
	{
		//std::cout << "perform byte alignment" << std::endl;
		int al = dequeue_dec(d_in, 4); // drop 4 bits as they were inserted for alignment
		if( al != 0 )
		{
			std::cout << "AAC Decoder: Alignment bits are not zero!\n";
		}
	}
	
	/* frame length of last AAC frame */
	d_frame_length[d_n_borders] = d_n_bytes_audio_payload - prev_border; // last frame duration is from last border to the end of audio payload (->longer than in the encoder! trailing zeros are dropped in the encoder?)
	
	/* check for plausible frame length entries and return if corrupted values are detected */
	for( int i = 0; i < d_n_aac_frames; i++)
	{
		if( d_frame_length[i] < 0 || d_frame_length[i] > d_n_bytes_audio_payload )
		{
			std::cout << "Invalid AAC frame length! Returning." << std::endl;
			//std::cout << "frame length: " << d_frame_length[i] << std::endl;
			return false;
		}
	}
	
	/* extract CRC bits */
	//std::cout << "extract CRC" << std::endl;
	for(int i = 0; i < d_n_aac_frames; i++)
	{
		d_crc_words[i] = dequeue_char(d_in);
		//std::cout << "dec frame length[" << i << "]: " << d_frame_length[i] << ", CRC: " << (int) d_crc_words[i] << std::endl;
	}
	
	/* insert CRC word and append payload (to be compliant with FAAD2 interface) */
	d_decoder_in.clear(); // clear data from earlier runs
	std::vector< unsigned char > tmp; // tmp vector for one audio frame
	for( int i = 0; i < d_n_aac_frames; i++)
	{
		tmp.clear();
		tmp.push_back(d_crc_words[i]); // insert CRC
		for( int k = 0; k < d_frame_length[i]; k++) // append payload
		{
			tmp.push_back(dequeue_char(d_in));
		}
		d_decoder_in.push_back(tmp);
	}
	return true;
}

bool
drm_audio_decoder_vbs::aac_decode()
{
	/* perform actual aac decoding */
	gr_int16* dec_out_buffer;
	NeAACDecFrameInfo dec_frame_info;

	for(int i = 0; i < d_n_aac_frames; i++)
	{
		//std::cout << "decoding frame number " << i << std::endl;
		//std::cout << "frame length[" << i << "]: " << d_decoder_in[i].size() - 1 << ", crc: " << (int) (d_decoder_in[i])[0] << std::endl;
		// EXPERIMENTAL: skip frame if CRC is 0
		if((d_decoder_in[i])[0] == 0 )
		{
			i++; // move on to next frame
			if(i>d_n_aac_frames-1)
			{
				break; // avoid segfault
			}
		}
		dec_out_buffer = (gr_int16*) NeAACDecDecode(d_dec_handle,
															 &dec_frame_info,
															 &(d_decoder_in[i])[0],
															 (unsigned long) d_decoder_in[i].size());
    
    	/*std::cout << "bytesconsumed: " << dec_frame_info.bytesconsumed << std::endl;
    	std::cout << "samples: " << dec_frame_info.samples << std::endl;
    	std::cout << "channels: " << (int) dec_frame_info.channels << std::endl;
    	std::cout << "error: " << (int) dec_frame_info.error << std::endl;
    	std::cout << "samplerate: " << dec_frame_info.samplerate << std::endl;
    	std::cout << "sbr: " << (int) dec_frame_info.sbr << std::endl;
    	std::cout << "object type: " << (int) dec_frame_info.object_type << std::endl;
    	std::cout << "header_type: " << (int) dec_frame_info.header_type << std::endl;
    	std::cout << "num_front_channels: " << (int) dec_frame_info.num_front_channels << std::endl;
    	std::cout << "num_side_channels: " << (int) dec_frame_info.num_side_channels << std::endl;
    	std::cout << "num_back_channels: " << (int) dec_frame_info.num_back_channels << std::endl;
    	std::cout << "num_lfe_channels: " << (int) dec_frame_info.num_lfe_channels << std::endl;
    	std::cout << "channel_position: " << (int) dec_frame_info.channel_position[64] << std::endl;
    	std::cout << "ps: " << (int) dec_frame_info.ps << std::endl;
		std::cout << std::endl;*/
		
		if(dec_out_buffer != NULL)
		{
			for( int x = 0; x < dec_frame_info.samples/2; x++)
			{
				*d_out++ = dec_out_buffer[x*2];
			}
		}
		else if(dec_frame_info.error != 0)
		{
			std::cout << "Decoder error: " << (int) dec_frame_info.error << std::endl;
			memset(d_out, 0, d_transform_length * sizeof(gr_int16)); // set output buffer to zero for this aac frame
			d_out += d_transform_length;
		}
		else
		{
			std::cout << "Pointer is NULL and we have no error!\n";
			memset(d_out, 0, d_transform_length * sizeof(gr_int16)); // set output buffer to zero for this aac frame
			d_out += d_transform_length;
		}
	}
	return true; // FIXME change return type to void if no errors are handled
}

int
drm_audio_decoder_vbs::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  //std::cout << "entering general_work()" << std::endl;
  d_in = (unsigned char *) input_items[0];
  d_out = (gr_int16 *) output_items[0];
  
  consume_each (1); // consume one vector of length L_MUX per call to general_work()
  
  // Text message handling
  if( d_tp->cfg().text() )
  {
  	  std::cout << "handle text message" << std::endl;
  	  decode_text_message(); // this is merely a placeholder at the moment as text messages are not implemented in the transmitter
  }
  
  // AAC super frame header decomposition
  unsigned char crc_words[(const int) d_n_aac_frames];
  int frame_length[(const int) d_n_aac_frames];
  d_crc_words = &crc_words[0];
  d_frame_length = &frame_length[0];
  
  //std::cout << "calling make_faad_compliant(). d_in: " << (long) d_in << std::endl;
  if( make_faad_compliant() )
  {
  	aac_decode();
  	return d_n_aac_frames * d_transform_length;
  	
  }
  else
  {
  	std::cout << "make_faad_compliant(): corrupted data, return from general_work()" << std::endl;
  	return 0; // no output, drops 10 aac frames TODO: make this more intelligent
  } 
}

