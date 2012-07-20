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

#ifndef INCLUDED_DRM_CELL_MAPPING_VBVB_H
#define INCLUDED_DRM_CELL_MAPPING_VBVB_H

#include <drm_api.h>
#include <gr_sync_interpolator.h>
#include "drm_transm_params.h"

class drm_cell_mapping_vbvb;
typedef boost::shared_ptr<drm_cell_mapping_vbvb> drm_cell_mapping_vbvb_sptr;

DRM_API drm_cell_mapping_vbvb_sptr drm_make_cell_mapping_vbvb (transm_params* tp, std::vector< int > input_sizes);

/*!
 * \brief Performs mapping of MSC, SDC, FAC and pilot cells onto the OFDM frame
 *
 */
class DRM_API drm_cell_mapping_vbvb : public gr_sync_interpolator
{
	friend DRM_API drm_cell_mapping_vbvb_sptr drm_make_cell_mapping_vbvb (transm_params* tp, std::vector< int > input_sizes);

	drm_cell_mapping_vbvb (transm_params* tp, std::vector< int > input_sizes);

	transm_params* d_tp;
	
 public:
	~drm_cell_mapping_vbvb ();


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_CELL_MAPPING_VBVB_H */

