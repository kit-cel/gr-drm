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
		gr_make_io_signature (1, 1, sizeof (gr_complex) * tp->ofdm().nfft()), tp->ofdm().N_S() * tp->ofdm().M_TF() )
{
	d_tp = tp;
	d_N = tp->ofdm().N_S();
	d_n_dummy_cells = tp->cfg().ptables()->d_MSC_N_L[tp->cfg().RM()][tp->cfg().SO()];
	d_nfft = tp->ofdm().nfft();
	d_tables = tp->cfg().ptables();
	d_M_TF = tp->ofdm().M_TF();
	d_k_min = tp->ofdm().K_min();
	d_k_max = tp->ofdm().K_max();

	switch(tp->cfg().RM())
	{
		case 0: // A
			d_time_rows = RMA_NUM_TIME_PIL;
			d_fac_rows = N_FAC_DRM;
			d_n_sdc_sym = 2;
			break;
		case 1: // B
			d_time_rows = RMB_NUM_TIME_PIL;
			d_fac_rows = N_FAC_DRM;
			d_n_sdc_sym = 2;
			break;
		case 2: // C
			d_time_rows = RMC_NUM_TIME_PIL;
			d_fac_rows = N_FAC_DRM;
			d_n_sdc_sym = 3;
			break;
		case 3: // D
			d_time_rows = RMD_NUM_TIME_PIL;
			d_fac_rows = N_FAC_DRM;
			d_n_sdc_sym = 3;
			break;
		case 4: // E
			d_time_rows = RME_NUM_TIME_PIL;
			d_fac_rows = N_FAC_DRMPLUS;
			d_n_sdc_sym = 5;
			break;
		default:
			break;
	}
	
	switch(tp->msc().mod_order())
	{
		case 2: // 4-QAM
			d_boost_coeff = 1/sqrt(42);
			break;
		case 4: // 16-QAM
			d_boost_coeff = 1/sqrt(10);
			break;
		case 6: // 64-QAM
			d_boost_coeff = 1/sqrt(2);
			break;
		default:
			break;
	}
	d_dummy_cells[0] = gr_complex(d_boost_coeff*1, d_boost_coeff*1); // a*(1+j1)
	d_dummy_cells[1] = gr_complex(d_boost_coeff*1, -d_boost_coeff*1); // a*(1-j1)
}


drm_cell_mapping_vbvb::~drm_cell_mapping_vbvb ()
{
}

bool
drm_cell_mapping_vbvb::is_used_carrier(int k)
{
	for(int i = 0; i < d_unused_carriers.size(); i++)
	{
		if( k == d_unused_carriers[i] )
		{
			return false; // carrier shall not be used
		}
	}
	return true;
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
	
	// set output buffer to zero
	memset(out, 0, d_N * d_M_TF * d_nfft * sizeof(gr_complex));
	
	// init log for debug purposes
	std::ofstream log;
	log.open("cell_mapping_log.txt");
	log << "beginning of cell mapping log" << std::endl;
	log.flush();
	// define offset value
	int k_off = d_nfft/2; // this should shift the DC carrier to the correct position TODO: evaluate this!
	
	// define some constants
	const std::complex<double> j(0,1); // imaginary unit
	const double pi = M_PI;
	const double boost = sqrt(2);
	
	// copy mapping tables to access them flexibly
	int freq_pil[3][2];
	const int time_rows = d_time_rows; // this way we can instance an array with this variable	
	int time_pil[time_rows][2];
	const int fac_rows = d_fac_rows;
	int fac_pos[fac_rows][2];
	int unused_carriers_A[3] = {-1, 0, 1};
	
	switch(d_tp->cfg().RM())
	{
		case 0: // A
			memcpy(freq_pil, d_tables->d_freq_A, 3*2*sizeof(int));
			memcpy(time_pil, d_tables->d_time_A, RMA_NUM_TIME_PIL*2*sizeof(int));
			memcpy(fac_pos, d_tables->d_FAC_A, N_FAC_DRM*2*sizeof(int));
			d_unused_carriers.assign(unused_carriers_A, unused_carriers_A + 3);
			break;
		case 1: // B
			memcpy(freq_pil, d_tables->d_freq_B, 3*2*sizeof(int));
			memcpy(time_pil, d_tables->d_time_B, RMB_NUM_TIME_PIL*2*sizeof(int));
			memcpy(fac_pos, d_tables->d_FAC_B, N_FAC_DRM*2*sizeof(int));
			d_unused_carriers.push_back(0);
			break;
		case 2: // C
			memcpy(freq_pil, d_tables->d_freq_C, 3*2*sizeof(int));
			memcpy(time_pil, d_tables->d_time_C, RMC_NUM_TIME_PIL*2*sizeof(int));
			memcpy(fac_pos, d_tables->d_FAC_C, N_FAC_DRM*2*sizeof(int));
			d_unused_carriers.push_back(0);
			break;
		case 3: // D
			memcpy(freq_pil, d_tables->d_freq_D, 3*2*sizeof(int));
			memcpy(time_pil, d_tables->d_time_D, RMD_NUM_TIME_PIL*2*sizeof(int));
			memcpy(fac_pos, d_tables->d_FAC_D, N_FAC_DRM*2*sizeof(int));
			d_unused_carriers.push_back(0);
			break;
		case 4: //E
			//memcpy(time_pil, d_tables->d_time_E, RME_NUM_TIME_PIL*2*sizeof(int));
			//memcpy(fac_pos, d_tables->d_FAC_E, N_FAC_DRMPLUS*2*sizeof(int));
			break;
		default:
			break;
	}
	
	/* Actual mapping (see chapter 8.4 in the DRM standard for details) */
	for(int tf = 0; tf < d_M_TF; tf++)
	{	
		log << std::endl << "BEGINNING OF FRAME " << tf << std::endl;
		
		/* Pilot cells */
		
		/* Frequency reference cells (not for RM E) */	
		log << "FREQUENCY PILOTS" << std::endl;
		if( d_tp->cfg().RM() != 4)
		{
			for(int s = 0; s < d_N; s++)
			{
				for(int i = 0; i < NUM_FREQ_PILOTS; i++)
				{
					if( (freq_pil[i][0] == 7 || freq_pil[i][0] == 21) && (s%2 == 1) ) // special treatment for carriers 7 and 21 in RM D
					{
						// multiply with -1 for odd values of s
						out[tf*d_N*d_nfft + s*d_nfft + freq_pil[i][0] + k_off] = -sqrt(2) * (cos(2*pi*freq_pil[i][1]/1024) + j*sin(2*pi*freq_pil[i][1]/1024) );
					}
					else
					{
						out[tf*d_N*d_nfft + s*d_nfft + freq_pil[i][0] + k_off] = sqrt(2) * (cos(2*pi*freq_pil[i][1]/1024) + j*sin(2*pi*freq_pil[i][1]/1024) );
					}
					log << "carrier " << freq_pil[i][0] << " ,symbol " << s << std::endl;
				}
			}
		}
	
		/* Time reference cells (only in the first symbol of the transmission frame) */
		log << std::endl << "TIME PILOTS" << std::endl;
		for( int i = 0; i < time_rows; i++)
		{
			out[tf*d_N*d_nfft + time_pil[i][0] + k_off] = sqrt(2) * ( cos(2 * pi * time_pil[i][1] / 1024 ) + j * sin( 2 * pi * time_pil[i][1] / 1024 ) );
			log << "carrier " << time_pil[i][0] << ", symbol 0" << std::endl;
		}
	
		/* Gain reference cells */
		log << std::endl << "GAIN PILOTS" << std::endl;
		for(int s = 0; s < d_N; s++)
		{
			for(int i = 0; i < d_tables->d_gain_pos[s].size(); i++)
			{
				if( abs(out[tf*d_N*d_nfft + s*d_nfft + d_tables->d_gain_pos[s][i] + k_off]) == 0 ) // cell is empty, map gain reference cell to it	 
				{
					out[tf*d_N*d_nfft + s*d_nfft + d_tables->d_gain_pos[s][i] + k_off] = d_tables->d_gain_cells[s][i];
					log << "carrier " << d_tables->d_gain_pos[s][i] << " , symbol " << s << std::endl;
				}
				else // cell is already occupied, do not overwrite phase. check if the cell is an overboosted one and boost amplitude if so.
				{
					if( abs(out[tf*d_N*d_nfft + s*d_nfft + d_tables->d_gain_pos[s][i] + k_off]) > 1.5 ) // normal boost is sqrt(2)
					{
						out[tf*d_N*d_nfft + s*d_nfft + d_tables->d_gain_pos[s][i] + k_off] *= sqrt(2);
						log << "carrier " << d_tables->d_gain_pos[s][i] << " , symbol " << s << " PHASE KEPT, abs: " << abs(out[tf*d_N*d_nfft + s*d_nfft + d_tables->d_gain_pos[s][i] + k_off]) << 
							" real/imag: " << (out[tf*d_N*d_nfft + s*d_nfft + d_tables->d_gain_pos[s][i] + k_off]).real() << "/" << (out[tf*d_N*d_nfft + s*d_nfft + d_tables->d_gain_pos[s][i] + k_off]).imag() << std::endl;
					}
				}
			}
		}
	
		/* AFS reference cells (only RM E) */
	
		/* Channel data (cells are mapped consecutively in ascending order from k_min to k_max) */
		
		/* Map FAC */		
		log << std::endl << "FAC CELLS" << std::endl;
		for( int i = 0; i < d_tp->fac().N(); i++)
		{
			out[tf*d_N*d_nfft + fac_pos[i][0]*d_nfft + fac_pos[i][1] + k_off] = fac_in[tf*d_tp->fac().N() + i];
			log << "carrier: " << fac_pos[i][1] << " , symbol: " << fac_pos[i][0] << ", value: " << fac_in[tf*d_tp->fac().N() + i] << std::endl;
		}
	
		/* Map SDC (only in the first transmission frame, omit DC carrier) */
		if( tf == 0 )
		{
			log << std::endl << "SDC CELLS" << std::endl;
			int n = 0;
			for( int s = 0; s < d_n_sdc_sym; s++)
			{
				for( int i = d_k_min; i <= d_k_max; i++)
				{		
					if( /* cell is empty */ abs(out[tf*d_N*d_nfft + s*d_nfft + k_off + i]) == 0  
						&& /* no unused carrier */ is_used_carrier(i) )
					{
						out[tf*d_N*d_nfft + s*d_nfft + k_off + i] = sdc_in[n];
						n++; // increment SDC cell counter
						log << "carrier: " << i << ", symbol: " << s << ", value: " << out[tf*d_N*d_nfft + s*d_nfft + k_off + i] << std::endl;
					}
				}
			}
		}
	}
	
	/* Map MSC to the rest of the free cells */
	log << std::endl << "MSC CELLS" << std::endl;
	int n = 0;
	// go through the whole super transmission frame and fill in MSC cells where the cell value is still 0 (except DC carrier)
	for( int tf = 0; tf < d_M_TF; tf++)
	{
		for( int s = 0; s < d_N; s++)
		{
			for( int i = d_k_min; i <= d_k_max; i++)
			{
				if( /* cell is empty */ abs(out[tf*d_N*d_nfft + s*d_nfft + k_off + i]) == 0  
						&& /* no unused carrier */ is_used_carrier(i) )
				
				{	log << "msc index: " << n << std::endl;
					if(n < d_tp->msc().N_MUX() * d_M_TF)
					{
						
						out[tf*d_N*d_nfft + s*d_nfft + k_off + i] = msc_in[n];
						n++; // increment MSC cell counter
						log << "carrier: " << i << ", symbol: " << s << ", value: " << out[tf*d_N*d_nfft + s*d_nfft + k_off + i] << ", input: " << msc_in[n-1] << std::endl;
					}
					else // insert dummy cell
					{
						out[tf*d_N*d_nfft + s*d_nfft + k_off + i] = d_dummy_cells[n - d_tp->msc().N_MUX() * d_M_TF];
						n++;
						log << "carrier: " << i << ", symbol: " << s << ", value: " << out[tf*d_N*d_nfft + s*d_nfft + k_off + i]
						<< " from " <<  d_dummy_cells[n - d_tp->msc().N_MUX() * d_M_TF] << " DUMMY CELL" << std::endl;
					}
				}
			}
		}
	}		

	// Tell runtime system how many output items we produced.
	return d_N * d_M_TF;
}

