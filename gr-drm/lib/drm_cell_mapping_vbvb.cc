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
#include <drm_cell_mapping_vbvb.h>


drm_cell_mapping_vbvb_sptr
drm_make_cell_mapping_vbvb (transm_params* tp, std::vector< int > input_sizes)
{
	return drm_cell_mapping_vbvb_sptr (new drm_cell_mapping_vbvb (tp, input_sizes));
}


drm_cell_mapping_vbvb::drm_cell_mapping_vbvb (transm_params* tp, std::vector< int > input_sizes)
	: gr_sync_interpolator ("cell_mapping_vbvb",
		gr_make_io_signaturev (3, 3, input_sizes),
		gr_make_io_signature (1, 1, sizeof (gr_complex) * tp->ofdm().nfft()), tp->ofdm().N_S())
{
	d_tp = tp;
	d_N = tp->ofdm().N_S();
	d_nfft = tp->ofdm().nfft();
	d_tables = tp->cfg().ptables();

	switch(tp->cfg().RM())
	{
		case 0: // A
			d_time_rows = RMA_NUM_TIME_PIL;
			break;
		case 1: // B
			d_time_rows = RMB_NUM_TIME_PIL;
			break;
		case 2: // C
			d_time_rows = RMC_NUM_TIME_PIL;
			break;
		case 3: // D
			d_time_rows = RMD_NUM_TIME_PIL;
			break;
		case 4: // E
			d_time_rows = RME_NUM_TIME_PIL;
			break;
		default:
			break;
	}
}


drm_cell_mapping_vbvb::~drm_cell_mapping_vbvb ()
{
}


int
drm_cell_mapping_vbvb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	gr_complex *msc_in = (gr_complex *) input_items[0];
	gr_complex *sdc_in = (gr_complex *) input_items[1];
	gr_complex *fac_in = (gr_complex *) input_items[2];
	gr_complex *out = (gr_complex *) output_items[0];
	
	// init log for debug purposes
	std::ofstream log;
	log.open("cell_mapping_log.txt");
	
	// define offset value
	int k_off = d_nfft/2 - 1; // this should shift the DC carrier to the correct position TODO: evaluate this!
	
	/* Map pilots */
	const std::complex<double> j(0,1); // imaginary unit
	const double pi = M_PI;
	const double boost = sqrt(2);
	
	int freq_pil[3][2];
	switch(d_tp->cfg().RM())
	{
		case 0: // A
			memcpy(freq_pil, d_tables->d_freq_A, 3*2*sizeof(int));
			break;
		case 1: // B
			memcpy(freq_pil, d_tables->d_freq_B, 3*2*sizeof(int));
			break;
		case 2: // C
			memcpy(freq_pil, d_tables->d_freq_C, 3*2*sizeof(int));
			break;
		case 3: // D
			memcpy(freq_pil, d_tables->d_freq_D, 3*2*sizeof(int));
			break;
		default:
			break;
	}
		
	/* Frequency reference cells (not for RM E) */	
	if( d_tp->cfg().RM() != 4)
	{
		for(int s = 0; s < d_N; s++)
		{
			for(int i = 0; i < NUM_FREQ_PILOTS; i++)
			{
				if( (freq_pil[i][0] == 7 || freq_pil[i][0] == 21) && (s%2 == 1) ) // special treatment for carriers 7 and 21 in RM D
				{
					// multiply with -1 for odd values of s
					out[s*d_nfft + freq_pil[i][0] ] = -sqrt(2) * (cos(2*pi*freq_pil[i][1]/1024) + j*sin(2*pi*freq_pil[i][1]/1024) );
				}
				else
				{
					out[s*d_nfft + freq_pil[i][0] ] = sqrt(2) * (cos(2*pi*freq_pil[i][1]/1024) + j*sin(2*pi*freq_pil[i][1]/1024) );
				}
			}
		}
	}
	
	/* Time reference cells */
	const int time_rows = d_time_rows; // this way we can instance an array with this variable	
	/* Gain reference cells */
	
	/* AFS reference cells (only RM E)
	
	
	/* Map FAC */
	
	/* Map SDC */
	
	/* Map MSC */
	
	

	// Tell runtime system how many output items we produced.
	return d_N;
}

