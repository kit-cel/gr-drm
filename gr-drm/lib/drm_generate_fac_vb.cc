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
#include <drm_generate_fac_vb.h>


drm_generate_fac_vb_sptr
drm_make_generate_fac_vb (transm_params* tp)
{
	return drm_generate_fac_vb_sptr (new drm_generate_fac_vb (tp));
}


drm_generate_fac_vb::drm_generate_fac_vb (transm_params* tp)
	: gr_sync_block ("generate_fac_vb",
		gr_make_io_signature (0, 0, 0),
		gr_make_io_signature (1, 1, sizeof (unsigned char) * tp->fac().L() ))
{
	d_tp = tp;
	d_tf_ctr = 0;
}


drm_generate_fac_vb::~drm_generate_fac_vb ()
{
}

unsigned char*
drm_generate_fac_vb::init_data()
{
	std::cout << "entering init_data()" << std::endl;
	/* allocate storage for the FAC bits */
	const unsigned int fac_length = d_tp->fac().L();
	unsigned char* data = new unsigned char[fac_length]; // TODO: call delete[]!
	unsigned char* start_data = data;
	
	/* set vector to zero as unused data shall be set to zero (see DRM standard) */
	for( unsigned int i = 0; i < fac_length; i++)
	{
		*data++ = 0;
	}
	
	data = start_data; //  reset pointer to the beginning
	
	/* Channel parameters */
	
	// Base/Enhancement Flag
	enqueue_bits(data, 1, (unsigned char[]) {0}); // Base layer, decodable by all DRM receivers

	// Identity Flag (currently no AFS is supported, SDC AFS flag is set valid)
	switch(d_tf_ctr)
	{
		case 0:
			enqueue_bits(data, 2, (unsigned char[]) {0,0});
			break;
		case 1:
			enqueue_bits(data, 2, (unsigned char[]) {0,1});
			break;
		case 2:
			if(d_tp->ofdm().M_TF() < 4) // RM A-D
			{
				enqueue_bits(data, 2, (unsigned char[]) {1,0});
			}
			else if(d_tp->ofdm().M_TF() == 4) // RM E
			{
				enqueue_bits(data, 2, (unsigned char[]) {0,1});
			}
			else
			{
				std::cout << "Invalid M_TF value: " << d_tp->ofdm().M_TF() << std::endl;
			}
			break;
		case 3:
			enqueue_bits(data, 2, (unsigned char[]) {1,0});
			break;
		default:
			std::cout << "Invalid d_tf_ctr value!\n";
	}
				
	// RM flag	
	if(d_tp->cfg().RM() < 4) // RM A-D
	{
		enqueue_bits(data, 1, (unsigned char[]) {0});
	}
	else // RM E
	{
		enqueue_bits(data, 1, (unsigned char[]) {1});
	}
	
	// Spectrum Occupancy
	if(d_tp->cfg().RM() < 4)
	{
		switch(d_tp->cfg().RM())
		{
			case 0: // A
				enqueue_bits(data, 3, (unsigned char[]) {0,0,0});
				break;
			case 1: // B
				enqueue_bits(data, 3, (unsigned char[]) {0,0,1});
				break;
			case 2: // C
				enqueue_bits(data, 3, (unsigned char[]) {0,1,0});
				break;
			case 3: // D
				enqueue_bits(data, 3, (unsigned char[]) {0,1,1});
				break;
			default: break; // error checking was done before
		}
	}
	else
	{
		enqueue_bits(data, 3, (unsigned char[]) {0,0,0});
	}
	
	// Interleaver depth flag
	if(d_tp->cfg().RM() < 4 && d_tp->cfg().long_interl() == false)
	{
		enqueue_bits(data, 1, (unsigned char[]) {1});
	}
	else
	{
		enqueue_bits(data, 1, (unsigned char[]) {0});
	}
	
	// MSC mode
	if(d_tp->cfg().RM() < 4) // RM A-D
	{
		switch(d_tp->cfg().msc_mapping())
		{
			case 1: // 16-QAM SM
				enqueue_bits(data, 2, (unsigned char[]) {1,1});
				break;
			case 2: // 64-QAM SM
				enqueue_bits(data, 2, (unsigned char[]) {0,0});
				break;
			case 3: // 64-QAM HMsym
				enqueue_bits(data, 2, (unsigned char[]) {1,0});
				break;
			case 4: // 64-QAM HMmix
				enqueue_bits(data, 2, (unsigned char[]) {0,1});
				break;
			default:
				std::cout << "Invalid MSC mapping index!\n";
				break;
		}
	}
	else // RM E
	{
		switch(d_tp->cfg().msc_mapping())
		{
			case 0: // 4-QAM SM
				enqueue_bits(data, 2, (unsigned char[]) {1,1});
				break;
			case 1: // 16-QAM SM
				enqueue_bits(data, 2, (unsigned char[]) {0,0});
				break;
			default:
				std::cout << "Invalid MSC mapping index!\n";
				break;
		}
	}
	
	// SDC mode
	if(d_tp->cfg().RM() < 4) // RM A-D
	{
		switch(d_tp->cfg().sdc_mapping())
		{
			case 0: // 16-QAM
				enqueue_bits(data, 1, (unsigned char[]) {0});
				break;
			case 1: // 4-QAM
				enqueue_bits(data, 1, (unsigned char[]) {1});
				break;
			default:
				std::cout << "Invalid SDC mapping index!\n";
				break;
		}
	}
	else // RM E
	{
		switch(d_tp->cfg().sdc_prot_level())
		{
			case 0: 
				enqueue_bits(data, 1, (unsigned char[]) {0});
				break;
			case 1:
				enqueue_bits(data, 1, (unsigned char[]) {1});
				break;
			default:
				std::cout << "Invalid SDC protection level index!\n";
				break;
		}
	}
	
	// Number of services
	enqueue_bits(data, 4, (unsigned char[]) {0,1,0,0});
	
	// Reconfiguration index (3 bit field)
	enqueue_bits(data, 3, (unsigned char[]) {0,0,0});
	
	// Toggle flag
	if(d_tp->cfg().RM() < 4) // RM A-D
	{
		enqueue_bits(data, 1, (unsigned char[]) {0});
	}
	else
	{
		if(d_tf_ctr == 1 || d_tf_ctr == 3)
		{
			enqueue_bits(data, 1, (unsigned char[]) {1});
		}
		else
		{
			enqueue_bits(data, 1, (unsigned char[]) {0});
		}
	}
	
	// rfu
	enqueue_bits(data, 1, (unsigned char[]) {0});

	/* Service parameters */
	// Service identifier (arbitrarily chosen)
	enqueue_bits(data, 24, (unsigned char[]) {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
                     						  0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1});
                     						  
    // Short ID
    enqueue_bits(data, 2, (unsigned char[]) {0, 0}); // arbitrarily chosen, has to match the one in the SDC
	
	// Audio CA (conditional access) indication
	enqueue_bits(data, 1, (unsigned char[]) {0}); // no CA indication
	
	// Language
	enqueue_bits(data, 4, (unsigned char[]) {0,1,1,1}); // German
	
	// Audio/Data flag
	enqueue_bits(data, 1, (unsigned char[]) {0}); // Audio
	
	// Service descriptor (5 bit field, depends on Audio/Data flag)
	// NOTE: a binary 31 indicates a test transmission that is skipped by standard receivers
	enqueue_bits(data, 5, (unsigned char[]) {0,1,0,0,0}); // Science
	
	// Data CA indication
	enqueue_bits(data, 1, (unsigned char[]) {0}); // no CA system used / no data present
	
	// rfa
	enqueue_bits(data, 6, (unsigned char[]) {0,0,0,0,0,0});
	
	std::cout << "returning from init_data()" << std::endl;
	return start_data;
}

void
drm_generate_fac_vb::increment_tf_ctr()
{
	// increment by one and wrap if M_TF is reached
	d_tf_ctr = (++d_tf_ctr) % d_tp->ofdm().M_TF();
}

int
drm_generate_fac_vb::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	std::cout << "noutput_items: " << noutput_items << "\t output_items.size(): " << output_items.size() << std::endl;
	for(int i = 0; i < noutput_items; i++)
	{
		unsigned char* fac_data = init_data();
		std::cout << "copying data" << std::endl;
		memcpy( (void*) output_items[i], (void*) fac_data, (size_t) sizeof(char) * d_tp->fac().L() );
		std::cout << "delete data" << std::endl;
		delete[] fac_data;
		std::cout << "increment counter" << std::endl << std::endl;
		increment_tf_ctr();
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

