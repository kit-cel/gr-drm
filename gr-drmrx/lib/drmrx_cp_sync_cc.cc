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
#include <drmrx_cp_sync_cc.h>


drmrx_cp_sync_cc_sptr
drmrx_make_cp_sync_cc (drmrx_conf* rx)
{
	return drmrx_cp_sync_cc_sptr (new drmrx_cp_sync_cc (rx));
}


drmrx_cp_sync_cc::drmrx_cp_sync_cc (drmrx_conf* rx)
	: gr_block ("cp_sync_cc",
		gr_make_io_signature (1, 1, sizeof (gr_complex)),
		gr_make_io_signature (1, 1, sizeof (gr_complex)))
{
    d_rx = rx;
    d_rm_detected = false;

    d_symboltime[RM_A][0] = TU_A;
    d_symboltime[RM_B][0] = TU_B;
    d_symboltime[RM_C][0] = TU_C;
    d_symboltime[RM_D][0] = TU_D;
    d_symboltime[RM_A][1] = TG_A;
    d_symboltime[RM_B][1] = TG_B;
    d_symboltime[RM_C][1] = TG_C;
    d_symboltime[RM_D][1] = TG_D;

    for(int i = 0; i < 4; i++)
    {
        d_len[i] = ceil( FS * (d_symboltime[i][0] + d_symboltime[i][1]) );
        d_off[i] = ceil( FS * d_symboltime[i][0] );
        d_corr_out[i] = new gr_complex[d_len[i]];
    }
}


drmrx_cp_sync_cc::~drmrx_cp_sync_cc ()
{
}


int
drmrx_cp_sync_cc::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  gr_complex *in = (gr_complex *) input_items[0];
  gr_complex *out = (gr_complex *) output_items[0];

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (noutput_items);

  int nsamp_sym_max = ceil((TU_A + TG_A)*FS); // maximum number of samples required is based on the longest OFDM symbol
  if(ninput_items[0] < nsamp_sym_max && nsamp_sym_max > 0) { return 0; }

  for(int i = 0; i < 4; i++) // test for every mode
  {
        // create corr instance for every mode (TODO: adjust corr class to make this more efficient)
        drmrx_corr correlator(in, in + d_off[i], d_corr_out[i], d_len[i]);
        correlator.execute();
        correlator.get_maximum( d_pos[i], d_max[i], d_avg[i] );
        printf("i = %d. pos: %d; max: %f; avg: %f ;ratio: %f \n", i, d_pos[i], d_max[i], d_avg[i], d_max[i]/d_avg[i]);
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

