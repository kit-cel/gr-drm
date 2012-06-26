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
#include <drm_scrambler_bb.h>


drm_scrambler_bb_sptr
drm_make_scrambler_bb (unsigned int reset_length)
{
	return drm_scrambler_bb_sptr (new drm_scrambler_bb (reset_length));
}

const short MIN_IN = 1;
const short MAX_IN = 1;
const short MIN_OUT = 1;
const short MAX_OUT = 1;

drm_scrambler_bb::drm_scrambler_bb (unsigned int reset_length)
	: gr_sync_block ("scrambler_bb",
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (unsigned char)),
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (unsigned char)))
{
	d_reset_length = reset_length;
}


drm_scrambler_bb::~drm_scrambler_bb ()
{
}

unsigned int
drm_scrambler_bb::reset_length()
{
	return d_reset_length;
}

int
drm_scrambler_bb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const unsigned char *in = (const  unsigned char*) input_items[0];
	unsigned char *out = (unsigned char*) output_items[0];
	
	const unsigned int n_reset = reset_length();

	// Generate PRBS of length reset_length (G(x) = x^9 + x^5 + 1)
	unsigned char prbs[n_reset]; // Pseudo random bit sequence array
	unsigned char lfsr[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; // inital state: all ones
	unsigned char lfsr_prev[9]; // holds previous shift register state while shifting
	unsigned char next_bit;
	
	for(int i = 0; i < n_reset; i++)
	{
		// calculate new leading bit and save it to prbs
		next_bit = (lfsr[4] + lfsr[8]) % 2; // this bit is going to be appended at the beginning
		prbs[i] = next_bit;
		
		// save current state, then shift
		
		// save
		for(int k = 0; k < 9; k++)
		{
			lfsr_prev[k] = lfsr[k];
		}
		
		// shift
		for(int k = 1; k < 9; k++)
		{
			lfsr[k] = lfsr_prev[k-1];
		}
		
		// append next prbs bit
		lfsr[0] = next_bit;
	}

	// Calculate output sequence by XORing input and prbs
	for(int i = 0; i < n_reset; i++)
	{
		out[i] = (in[i] + prbs[i]) % 2;
	}
	
	// Tell runtime system how many output items we produced.
	return noutput_items;
}

