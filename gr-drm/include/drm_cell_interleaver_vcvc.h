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

#ifndef INCLUDED_DRM_CELL_INTERLEAVER_VCVC_H
#define INCLUDED_DRM_CELL_INTERLEAVER_VCVC_H

#include <drm_api.h>
#include <gr_block.h>
#include <iostream>

class drm_cell_interleaver_vcvc;
typedef boost::shared_ptr<drm_cell_interleaver_vcvc> drm_cell_interleaver_vcvc_sptr;

DRM_API drm_cell_interleaver_vcvc_sptr drm_make_cell_interleaver_vcvc (std::vector<int> interl_seq, bool long_interl, int depth = 0);

/*!
 * \brief Performs cell interleaving. The long_interl flag determines if long interleaving is used.
 *
 */
class DRM_API drm_cell_interleaver_vcvc : public gr_block
{
	friend DRM_API drm_cell_interleaver_vcvc_sptr drm_make_cell_interleaver_vcvc (std::vector<int> interl_seq, bool long_interl, int depth);

	drm_cell_interleaver_vcvc (std::vector<int> interl_seq, bool long_interl, int depth);
	
	std::vector<int> d_seq;
	bool d_long_interl;
	int d_depth;
	std::vector< std::vector<gr_complex> > d_buffer;
	std::vector< gr_complex > d_tmp_buffer;

 public:
	~drm_cell_interleaver_vcvc ();


  int general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_CELL_INTERLEAVER_VCVC_H */

