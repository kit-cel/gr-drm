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
#include <drmrx_corr.h>

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
    // set (default) values for variables)
	d_rx = rx;
	d_nsamp_sym = FS * T_O; 
    d_freq_off = 0;
    d_corr_vec = NULL;
    d_corr_maxval = 0;
    d_corr_pos = 0;
    d_corr_avg = 0;
    d_signal_present = false;
	
	// Generation of frequency pilot pattern - vector with ones at the pilot positions, zeroes otherwise
	
	double delta_f = 1/T_O; // frequency resolution in Hz
	double f1 = 750; // pilot position in Hz
	double f2 = 2250;
	double f3 = 3000;
    std::vector<double> t; // time vector

    for(int i = 0; i < d_nsamp_sym; i++)
    {
        d_pilot_pattern.push_back(sin(2*M_PI*f1*i/T_O) + 
                                  sin(2*M_PI*f2*i/T_O) +
                                  sin(2*M_PI*f3*i/T_O));
    }

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
  
  // copy input buffer to intermediate buffer
  for(int i = 0; i < ninput_items[0]; i++)
  {
      d_buf.push_back(in[i]); // do this via insert to be faster
  }

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (ninput_items[0]);

  // If there are enough samples, determine the frequency offset through correlation with the frequency pilots
  if(d_buf.size() < d_nsamp_sym) 
  {
      return 0; // wait for more samples
  }
  else
  {
      double freq_off_tmp = 0;
      int nsym_in_buf = std::floor(d_buf.size() / d_nsamp_sym);
      for(int i = 0; i < nsym_in_buf; i++)
      {
          // correlate pilot positions with fouriertransformed OFDM signal 
         drmrx_corr correlator(&d_buf[0], &d_pilot_pattern[0],
                               d_corr_vec, d_nsamp_sym);
         correlator.execute();
         correlator.get_maximum(d_corr_pos, d_corr_maxval, d_corr_avg);

         // evaluate, if maxval is high enough (-> DRM signal present) and if so, correct the signal
         if(d_corr_maxval/d_corr_avg > 5) // maybe find a better value than 5 
         {
             d_signal_present = true;
            // copy samples from intermediate to output buffer and delete from buffer
            memcpy(out, &d_buf[0], d_nsamp_sym);
            d_buf.erase(d_buf.begin(), d_buf.begin()+d_nsamp_sym); // this is inefficient!

            return nsym_in_buf * d_nsamp_sym;
         }
         else
         {
             d_signal_present = false; // reset flag if it was true before
            // don't copy samples to output, just delete them from the buffer
            d_buf.erase(d_buf.begin(), d_buf.begin()+d_nsamp_sym); // this is inefficient!

            return 0;
         }
      }
  }
}

