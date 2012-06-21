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
		gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (char)))
{
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
  char *out = (char *) output_items[0];

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (noutput_items);

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

