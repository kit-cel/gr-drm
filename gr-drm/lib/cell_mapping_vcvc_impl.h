/* -*- c++ -*- */
/*
 * Copyright 2014 Felix Wunsch, Communications Engineering Lab (CEL) / Karlsruhe Institute of Technology (KIT).
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

#ifndef INCLUDED_DRM_CELL_MAPPING_VCVC_IMPL_H
#define INCLUDED_DRM_CELL_MAPPING_VCVC_IMPL_H

#include <drm/cell_mapping_vcvc.h>

namespace gr {
  namespace drm {

    class cell_mapping_vcvc_impl : public cell_mapping_vcvc
    {
    private:
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
		cell_mapping_vcvc_impl(transm_params* tp, std::vector< int > input_sizes);
		~cell_mapping_vcvc_impl();

		bool is_used_carrier(int k);

		// Where all the action really happens
		int work(int noutput_items,
		   	gr_vector_const_void_star &input_items,
		   	gr_vector_void_star &output_items);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_CELL_MAPPING_VCVC_IMPL_H */

