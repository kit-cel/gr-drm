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

void
drm_generate_fac_vb::init_data(unsigned char* data)
{
	unsigned char* data_start = data; // pointer to the beginning of the FAC data
	
	/* set vector to zero as unused data shall be set to zero (see DRM standard) */
	memset(data, 0, d_tp->fac().L());
	
	/* Channel parameters */
	
	// Base/Enhancement Flag
	enqueue_bits(data, 1, (unsigned char[]) {0}); // Base layer, decodable by all DRM receivers

	// Identity Flag (currently no AFS is supported, SDC AFS flag is set invalid)
	switch(d_tf_ctr)
	{
		case 0:
			enqueue_bits(data, 2, (unsigned char[]) {1,1});
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
		switch(d_tp->cfg().SO())
		{
			case 0: // 4.5 kHz
				enqueue_bits(data, 3, (unsigned char[]) {0,0,0});
				break;
			case 1: // 5 kHz
				enqueue_bits(data, 3, (unsigned char[]) {0,0,1});
				break;
			case 2: // 9 kHz
				enqueue_bits(data, 3, (unsigned char[]) {0,1,0});
				break;
			case 3: // 10 kHz
				enqueue_bits(data, 3, (unsigned char[]) {0,1,1});
				break;
			case 4: // 18 kHz
				enqueue_bits(data, 3, (unsigned char[]) {1,0,0});
				break;
			case 5: // 20 kHz
				enqueue_bits(data, 3, (unsigned char[]) {1,0,1});
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
	
	unsigned char* serv_params_start = data; //  marker for the start of the service parameters

	/*std::cout << "FAC channel parameters: ";
	for(int i = 0; i < 20; i++)
		std::cout << (int) data_start[i];
	std::cout << std::endl;*/
	
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
	enqueue_bits(data, 5, (unsigned char[]) {0,0,0,1,1}); // Science
	
	// Data CA indication
	enqueue_bits(data, 1, (unsigned char[]) {0}); // no CA system used / no data present
	
	// rfa
	enqueue_bits(data, 6, (unsigned char[]) {0,0,0,0,0,0});
	
	/* if RM E is chosen, two sets of service params are transmitted FIXME: here, it is just a copy of the first set */
	if(d_tp->cfg().RM() == 4)
	{
		memcpy(data, serv_params_start, data - serv_params_start); // copy the part between data and serv_params_start
	}
	
	/*std::cout << "FAC service parameters: ";
	for(int i = 0; i < 44; i++)
		std::cout << (int) serv_params_start[i];
	std::cout << std::endl;*/
	
	/* enqueue CRC word */
	int len;
	if(d_tp->cfg().RM() < 4) // standard DRM
	{
		len = 64; // 20 bits of channel parameters + 44 bits of service parameters
	}
	else // DRM+
	{
		len = 112; // 20 bits  + 2 * 44 bits + 4 bits (zeros, only for CRC calculation) 
	}
	enqueue_crc(data_start, d_tp, len, 8); //  The channel type is implicitly detected by the choice of the polynomial
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
	const unsigned int fac_length = d_tp->fac().L();
	unsigned char* data = new unsigned char[fac_length];
	
	init_data(data);
	memcpy( (void*) output_items[0], (void*) data, (size_t) sizeof(char) * fac_length );
	increment_tf_ctr();
	
	delete[] data;

	// Tell runtime system how many output items we produced.
	return 1;
}

