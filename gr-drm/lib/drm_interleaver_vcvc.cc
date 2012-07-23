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
#include <drm_interleaver_vcvc.h>
#include <fstream>


drm_interleaver_vcvc_sptr
drm_make_interleaver_vcvc (std::vector<int> interl_seq)
{
	return drm_interleaver_vcvc_sptr (new drm_interleaver_vcvc (interl_seq));
}


drm_interleaver_vcvc::drm_interleaver_vcvc (std::vector<int> interl_seq)
	: gr_sync_block ("interleaver_vcvc",
		gr_make_io_signature (1, 1, sizeof (gr_complex) * interl_seq.size()),
		gr_make_io_signature (1, 1, sizeof (gr_complex) * interl_seq.size()))
{
	d_seq = interl_seq;
}


drm_interleaver_vcvc::~drm_interleaver_vcvc ()
{
}


int
drm_interleaver_vcvc::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const gr_complex *in = (const  gr_complex *) input_items[0];
	gr_complex *out = ( gr_complex *) output_items[0];
	
	std::ofstream log;
	log.open("cell_interleaver_log.txt");
	log << "CELL INTERLEADER LOG" << std::endl;
	log << "interleaver size: " << d_seq.size() << std::endl;

	// Interleave array entries according to the interleaver sequence
	for(int i = 0; i < d_seq.size(); i++)
	{
		out[i] = in[d_seq[i]];
		log << "index: " << d_seq[i] << ", input: "<< in[d_seq[i]] << ", output: " << out[i] << std::endl;
	}

	// Tell runtime system how many output items we produced.
	return 1;
}

