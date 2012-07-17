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

#ifndef INCLUDED_DRM_QAM_MAP_VBVB_H
#define INCLUDED_DRM_QAM_MAP_VBVB_H

#include <drm_api.h>
#include <gr_sync_block.h>
#include <iostream>

class drm_qam_map_vbvb;
typedef boost::shared_ptr<drm_qam_map_vbvb> drm_qam_map_vbvb_sptr;

DRM_API drm_qam_map_vbvb_sptr drm_make_qam_map_vbvb (const float map_table[][2], int bits_per_symbol, int vlen_out, int n_inputs);

/*!
 * \brief General QAM mapping. Input is a matrix with bits_per_symbol (indexing through decimal interpretation of the bits) rows and 2 columns (I/Q).
 * Works only for symmetrical signal constellations.
 *
 */
class DRM_API drm_qam_map_vbvb : public gr_sync_block
{
	friend DRM_API drm_qam_map_vbvb_sptr drm_make_qam_map_vbvb (const float map_table[][2], int bits_per_symbol, int vlen_out, int n_inputs);

	drm_qam_map_vbvb (const float map_table[][2], int bits_per_symbol, int vlen_out, int n_inputs);
	
	float d_map_table[8][2]; // this table always has the maximum size, but only the relevant fields will be filled
	int d_bits_per_symbol;
	int d_vlen_out;
	int d_n_inputs;

 public:
	~drm_qam_map_vbvb ();


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_QAM_MAP_VBVB_H */

