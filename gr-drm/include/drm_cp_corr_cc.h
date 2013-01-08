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

#ifndef INCLUDED_DRM_CP_CORR_CC_H
#define INCLUDED_DRM_CP_CORR_CC_H

#include <drm_api.h>
#include <gr_block.h>

class drm_cp_corr_cc;
typedef boost::shared_ptr<drm_cp_corr_cc> drm_cp_corr_cc_sptr;

DRM_API drm_cp_corr_cc_sptr drm_make_cp_corr_cc (rx_class);

/*!
 * Performs a cyclic prefix correlation to determine the symbol rate.
 *
 */
class DRM_API drm_cp_corr_cc : public gr_block
{
	friend DRM_API drm_cp_corr_cc_sptr drm_make_cp_corr_cc (rx_class);

	drm_cp_corr_cc (rx_class);

	rx_class* d_rx; // rx_class instance
	std::vector<gr_complex>* d_buf; // intermediate buffer

 public:
	~drm_cp_corr_cc ();


  int general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_CP_CORR_CC_H */

