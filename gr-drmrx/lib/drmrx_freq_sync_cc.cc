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
    d_corr_maxval = -1;
    d_corr_pos = -1;
    d_corr_avg = -1;
    d_signal_present = false;
	d_corr_vec = (gr_complex*) fftwf_malloc(sizeof(gr_complex)*d_nsamp_sym); 
	
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
	fftwf_free(d_corr_vec);	
}

/*void
drmrx_freq_sync_cc::forecast(int noutput_items, gr_vector_int &ninput_items_required)
{
	ninput_items_required[0] = d_nsamp_sym; // we need d_nsamp_sym input samples for the work() algorithm
}*/


int
drmrx_freq_sync_cc::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  printf("\nentering work()\n");
  const gr_complex *in = (const gr_complex *) input_items[0];
  gr_complex *out = (gr_complex *) output_items[0];
  
  printf("ninput items: %i \n", ninput_items[0]);

  // copy input buffer to intermediate buffer and make sure the buffer doesn't constantly 
  int new_samples = 0;
  if(ninput_items[0] > d_nsamp_sym)
  {
	  new_samples = d_nsamp_sym;
  }
  else
  {
      new_samples = ninput_items[0];
  }
  for(int i = 0; i < new_samples; i++)
  {
      d_buf.push_back(in[i]); // TODO: do this via insert to be faster
  }

  if(d_buf.size() < d_nsamp_sym) 
  {
  	  printf("not enough samples. returning.\n");
      return 0; // wait for more samples
  }

  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (new_samples);

  // determine the frequency offset through correlation with the frequency pilots
  printf("performing correlation \n");
  
  // correlate pilot positions with fourier-transformed OFDM signal 
  drmrx_corr correlator(&d_buf[0], &d_pilot_pattern[0],
                               d_corr_vec, d_nsamp_sym);
  correlator.execute();
  correlator.get_maximum(d_corr_pos, d_corr_maxval, d_corr_avg);

  // evaluate, if maxval is high enough (-> DRM signal present) and if so, correct the signal
  if(d_corr_maxval/d_corr_avg > 5) // maybe find a better value than 5 
  {
  	printf("signal present! max/avg: %f/%f=%f. copy and return.", d_corr_maxval, d_corr_avg, d_corr_maxval/d_corr_avg);
    d_signal_present = true;
    // copy samples from intermediate to output buffer and delete from buffer
    memcpy(out, &d_buf[0], d_nsamp_sym);
    d_buf.erase(d_buf.begin(), d_buf.begin()+d_nsamp_sym); // this is inefficient!

	return 0;
    //return d_nsamp_sym;
  }
  else
  {
	printf("no signal detected.  max/avg: %f/%f=%f. return.", d_corr_maxval, d_corr_avg, d_corr_maxval/d_corr_avg);
    d_signal_present = false; // reset flag if it was true before
    // don't copy samples to output, just delete them from the buffer
    d_buf.erase(d_buf.begin(), d_buf.begin()+d_nsamp_sym); // this is inefficient!

    return 0;
  }
}

