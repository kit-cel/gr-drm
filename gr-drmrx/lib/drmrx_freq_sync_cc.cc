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
    d_nfft = -1;
    d_corr_vec = NULL;
    d_corr_maxval = -1;
    d_corr_pos = -1;
    d_corr_avg = -1;
    d_signal_present = false;
	//d_corr_vec = (gr_complex*) fftwf_malloc(sizeof(gr_complex)*d_nsamp_sym);
	d_corr_vec = (float*) malloc(sizeof(float)*d_nsamp_sym*2-1);
	//d_fft = drmrx_fft(); // FIXME

	// Generation of frequency pilot pattern - vector with ones at the pilot positions, zeroes otherwise
	double delta_f = double(1.0/T_O); // frequency resolution in Hz
	double f1 = 750.0; // pilot position in Hz
	double f2 = 2250.0;
	double f3 = 3000.0;
	int if1 = round(f1/delta_f);
	int if2 = round(f2/delta_f);
	int if3 = round(f3/delta_f);

    // use the shortest FFT that reaches a spectral resolution of 1/T_0
	d_nfft = pow(2, ceil(log(f3/delta_f)/log(2))); // log2(x) = ln(x)/ln(2)
	printf("freq_sync: using FFT lenght of %d; frequency resolution is %f", d_nfft, delta_f); printf(" >= %f (1/T_0)\n", 1.0/T_O);
	d_pilot_pattern = (gr_complex*) fftwf_malloc(sizeof(gr_complex)*d_nfft);
	memset( (void*) d_pilot_pattern, 0, sizeof(gr_complex)*d_nfft ); // set all values to zero

	d_pilot_pattern[if1] = 1;
	d_pilot_pattern[if2] = 1;
	d_pilot_pattern[if3] = 1;
}


drmrx_freq_sync_cc::~drmrx_freq_sync_cc ()
{
	free(d_corr_vec);
	fftwf_free(d_pilot_pattern);
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
  //drmrx_corr correlator(&d_buf[0], &d_pilot_pattern[0], d_corr_vec, d_nsamp_sym);
  /*drmrx_corr correlator( &d_pilot_pattern[0], &d_pilot_pattern[0], d_corr_vec, d_nsamp_sym);
  correlator.execute();
  correlator.get_maximum(d_corr_pos, d_corr_maxval, d_corr_avg);


  drmrx_corr_t(d_pilot_pattern, d_pilot_pattern, d_corr_vec, d_nsamp_sym, d_nsamp_sym, d_corr_pos, d_corr_maxval, d_corr_avg);
  // evaluate, if maxval is high enough (-> DRM signal present) and if so, correct the signal
  if(d_corr_maxval/d_corr_avg > 5) // maybe find a better value than 5
  {
  	printf("signal present! max/avg: %f/%f=%f. copy and return.", d_corr_maxval, d_corr_avg, d_corr_maxval/d_corr_avg);
    d_signal_present = true;
    // copy samples from intermediate to output buffer and delete from buffer
    memcpy(out, &d_buf[0], d_nsamp_sym);
    d_buf.erase(d_buf.begin(), d_buf.begin()+d_nsamp_sym); // this is inefficient!

	return 0; // FIXME
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
  */
  return 0;
}

