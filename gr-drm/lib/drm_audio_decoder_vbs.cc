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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <drm_audio_decoder_vbs.h>


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
	d_in = NULL;
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
	int transform_length = 960;
	int n_bytes_audio_payload = d_tp->msc().L_MUX() / 8 - d_n_header_bytes - d_n_aac_frames;
	if(d_tp->cfg().text()) // if text message is used, the last four bytes contain the text message
	{
		n_bytes_audio_payload -= 4;
	}
	int max_bytes_per_frame = n_bytes_audio_payload; // this is just a theoretical upper bound
	
	d_dec_handle = NEAACDECAPI NeAACDecOpen();
	NEAACDECAPI NeAACDecInitDRM(&d_dec_handle, d_audio_samp_rate, drm_channel_mode);
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
	
	/* AAC super frame header */
	unsigned int prev_border = 0;
	for(int i = 0; i < d_n_borders; i++)
	{
		unsigned int frame_border = dequeue_dec(d_in, 12);
		d_frame_length[i] = frame_border - prev_border;
		prev_border = frame_border;
	}
	
	/* byte alignment */
	if(d_n_borders == 9)
	{
		dequeue_dec(d_in, 4); // drop 4 bits
	}
	
	/* frame length of last AAC frame */
	d_frame_length[d_n_borders] = d_n_bytes_decoder_in - prev_border;
	
	/* check for plausible frame length entries and return if corrupted values are detected */
	for( int i = 0; i < d_n_aac_frames; i++)
	{
		if( d_frame_length[i] < 0 || d_frame_length[i] > d_max_bytes_aac_frame )
		{
			std::cout << "Invalid AAC frame length! Returning." << std::endl;
			return false;
		}
	}
	
	/* extract CRC bits */
	for(int i = 0; i < d_n_aac_frames; i++)
	{
		d_crc_words[i] = dequeue_char(d_in);
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
}

void
drm_audio_decoder_vbs::aac_decode()
{
	/* perform actual aac decoding */
	gr_int16* dec_out_buffer;
	NeAACDecFrameInfo dec_frame_info;
	for(int i = 0; i < d_n_aac_frames; i++)
	{
		dec_out_buffer = (gr_int16*) NeAACDecDecode(d_dec_handle,
															 &dec_frame_info,
															 &(d_decoder_in[i])[0],
															 d_decoder_in[i].size() );
		memcpy(d_out, dec_out_buffer, 960 * sizeof(gr_int16));
		d_out += 960;
	}
}

int
drm_audio_decoder_vbs::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  unsigned char *d_in = (unsigned char *) input_items[0];
  gr_int16 *d_out = (gr_int16 *) output_items[0];
  
  // Text message handling
  if( d_tp->cfg().text() )
  {
  	  decode_text_message(); // this is merely a placeholder at the moment as text messages are not implemented in the transmitter
  }
  
  // AAC super frame header decomposition
  unsigned char crc_words[(const int) d_n_aac_frames];
  int frame_length[(const int) d_n_aac_frames];
  d_crc_words = &crc_words[0];
  d_frame_length = &frame_length[0];
  
  if( !make_faad_compliant() )
  {
  	return 0; // drop output
  }
  
  // actual decoding
  aac_decode();

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (1); // consume one vector of length L_MUX per call to general_work()

  // Tell runtime system how many output items we produced.
  return 0; // FIXME
}

