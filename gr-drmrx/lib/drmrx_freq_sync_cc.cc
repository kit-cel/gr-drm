/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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
#include <drmrx_freq_sync_cc.h>
#include <cmath>

drmrx_freq_sync_cc_sptr
drmrx_make_freq_sync_cc (drmrx_conf* rx)
{
	return drmrx_freq_sync_cc_sptr (new drmrx_freq_sync_cc (rx));
}

drmrx_freq_sync_cc::drmrx_freq_sync_cc (drmrx_conf* rx)
	: gr_block ("freq_sync_cc",
		gr_make_io_signature (1, 1, sizeof (gr_complex)),
		gr_make_io_signature (1, 1, sizeof (gr_complex)))
{
	d_rx = rx;
	d_nsamp_sym = FS * T_O; 
	
	// Generation of frequency pilot pattern - vector with ones at the pilot positions, zeroes otherwise
	
	double delta_f = 1/T_O; // frequency resolution in Hz
	double f1 = 750; // pilot position in Hz
	double f2 = 2250;
	double f3 = 3000;
	unsigned int i_f1 = round(f1/delta_f); // frequency pilot index
	unsigned int i_f2 = round(f2/delta_f);
	unsigned int i_f3 = round(f3/delta_f);

	d_pilot_pattern.assign(std::ceil(3000.0/delta_f) + 1, 0); // first bin represents DC, last one 3 kHz
	d_pilot_pattern[i_f1] = 1;
	d_pilot_pattern[i_f2] = 1;
	d_pilot_pattern[i_f3] = 1;
}


drmrx_freq_sync_cc::~drmrx_freq_sync_cc ()
{
}


int
drmrx_freq_sync_cc::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  const gr_complex *in = (const gr_complex *) input_items[0];
  gr_complex *out = (gr_complex *) output_items[0];

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (noutput_items);

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

