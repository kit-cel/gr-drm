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
		gr_make_io_signature (1, 1, sizeof (gr_complex) * tp.ofdm().nfft()), tp.ofdm().N_S())
{
	d_tp = tp;
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
	gr_complex *sdc_in = (gr_complex *) input_items[0];
	gr_complex *fac_in = (gr_complex *) input_items[0];
	gr_complex *out = (gr_complex *) output_items[0];

	/* Init empty OFDM frame */
	
	/* Map pilots */
	
	/* Map FAC */
	
	/* Map SDC */
	
	/* Map MSC */
	
	

	// Tell runtime system how many output items we produced.
	return d_tp.ofdm().N_S();
}

