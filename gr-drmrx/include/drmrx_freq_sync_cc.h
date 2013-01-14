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

#ifndef INCLUDED_DRMRX_FREQ_SYNC_CC_H
#define INCLUDED_DRMRX_FREQ_SYNC_CC_H

#include <drmrx_api.h>
#include <gr_block.h>
#include <drmrx_conf.h>
#include <drmrx_constants.h>

class drmrx_freq_sync_cc;
typedef boost::shared_ptr<drmrx_freq_sync_cc> drmrx_freq_sync_cc_sptr;

DRMRX_API drmrx_freq_sync_cc_sptr drmrx_make_freq_sync_cc (drmrx_conf* rx);

/*!
 * \brief Detects the presence of a DRM30 signal by correlating with the known frequency pilot pattern. 
 *
 */
class DRMRX_API drmrx_freq_sync_cc : public gr_block
{
	friend DRMRX_API drmrx_freq_sync_cc_sptr drmrx_make_freq_sync_cc (drmrx_conf* rx);

	drmrx_freq_sync_cc (drmrx_conf* rx);
	drmrx_conf* d_rx; // drmrx_conf instance
	unsigned int d_nsamp_sym; // number of samples per symbol
	std::vector<gr_complex>* d_buf; // intermediate buffer

 public:
	~drmrx_freq_sync_cc ();


  int general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRMRX_FREQ_SYNC_CC_H */

