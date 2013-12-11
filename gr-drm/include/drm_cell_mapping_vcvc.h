/* -*- c++ -*- */
/* 
 * Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
 * Author: Felix Wunsch
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

#ifndef INCLUDED_DRM_CELL_mapping_vcvc_H
#define INCLUDED_DRM_CELL_mapping_vcvc_H

#include <drm_api.h>
#include <gnuradio/sync_interpolator.h>
#include <cmath>
#include <complex>
#include "drm_transm_params.h"

class drm_cell_mapping_vcvc;
typedef boost::shared_ptr<drm_cell_mapping_vcvc> drm_cell_mapping_vcvc_sptr;

DRM_API drm_cell_mapping_vcvc_sptr drm_make_cell_mapping_vcvc (transm_params* tp, std::vector< int > input_sizes);

/*!
 * \brief Performs mapping of MSC, SDC, FAC and pilot cells onto the OFDM super transmission frame.
 */
class DRM_API drm_cell_mapping_vcvc : public gr::sync_interpolator
{
	friend DRM_API drm_cell_mapping_vcvc_sptr drm_make_cell_mapping_vcvc (transm_params* tp, std::vector< int > input_sizes);

	drm_cell_mapping_vcvc (transm_params* tp, std::vector< int > input_sizes);

	transm_params* d_tp;
	msc_params d_msc;
	unsigned short d_RM;
	unsigned int d_N_FAC; // number of QAM cells in the FAC
	unsigned int d_N_MSC; // number of QAM cells in the MSC
	tables* d_tables;
	int d_N; // number of symbols per transmission frame
	int d_n_dummy_cells; // number of dummy cells
	gr_complex d_dummy_cells[2]; // array of dummy cells (1+j1, 1-j1)
	double d_boost_coeff;
	int d_nfft;
	int d_time_rows;
	int d_M_TF; // number of transmission frames per super transmission frame
	int d_fac_rows;
	int d_n_sdc_sym; // number of SDC OFDM symbols
	int d_k_min;
	int d_k_max;
	std::vector<int> d_unused_carriers;
	
 public:
	~drm_cell_mapping_vcvc ();
	
	bool is_used_carrier(int k);


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_CELL_mapping_vcvc_H */

