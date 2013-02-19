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

#ifndef INCLUDED_DRMRX_CP_SYNC_CC_H
#define INCLUDED_DRMRX_CP_SYNC_CC_H

#include <drmrx_api.h>
#include <gr_block.h>
#include "drmrx_conf.h"
#include "drmrx_constants.h"
#include "drmrx_corr.h"

class drmrx_cp_sync_cc;
typedef boost::shared_ptr<drmrx_cp_sync_cc> drmrx_cp_sync_cc_sptr;

DRMRX_API drmrx_cp_sync_cc_sptr drmrx_make_cp_sync_cc (drmrx_conf* rx);

/*!
 * \brief Performs a cyclic prefix correlation to determine the symbol rate and the fractional frequency offset
 *
 */
class DRMRX_API drmrx_cp_sync_cc : public gr_block
{
	friend DRMRX_API drmrx_cp_sync_cc_sptr drmrx_make_cp_sync_cc (drmrx_conf* rx);

	drmrx_cp_sync_cc (drmrx_conf* rx);
	
	drmrx_conf* d_rx; // receiver configuration
	bool d_rm_detected; // this flag indicates whether a RM was detected and the detection process can be skipped
	double d_symboltime[4][2]; // matrix holding values for T_u and T_g, rows correspond to RM, column 1 is T_u, column 2 T_g (in seconds)
    int d_len[4]; // lengths of correlator inputs (must be the same)
    int d_off[4]; // distance in samples between cyclic prefix and its equivalent in the useful part of the OFDM symbol
    gr_complex* d_corr_out[4]; // array holding pointers to complex arrays
    int d_pos[4]; // array holding position of the maximum in d_corr_out[i]
    float d_max[4]; // maximum value of d_corr_out[i] at d_pos[i];
    float d_avg[4]; // average of d_corr_out;

 public:
	~drmrx_cp_sync_cc ();


  int general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRMRX_CP_SYNC_CC_H */

