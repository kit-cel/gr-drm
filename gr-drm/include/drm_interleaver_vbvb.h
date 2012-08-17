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

#ifndef INCLUDED_DRM_INTERLEAVER_VBVB_H
#define INCLUDED_DRM_INTERLEAVER_VBVB_H

#include <drm_api.h>
#include <gr_sync_block.h>

class drm_interleaver_vbvb;
typedef boost::shared_ptr<drm_interleaver_vbvb> drm_interleaver_vbvb_sptr;

DRM_API drm_interleaver_vbvb_sptr drm_make_interleaver_vbvb (std::vector<int> interl_seq);

/*!
 * \brief Interleaves a sequence of the same length as the interleaver sequence as specified in the sequence (representing the interleaved indexes)
 *
 */
class DRM_API drm_interleaver_vbvb : public gr_sync_block
{
	friend DRM_API drm_interleaver_vbvb_sptr drm_make_interleaver_vbvb (std::vector<int> interl_seq);

	drm_interleaver_vbvb (std::vector<int> interl_seq);
	
	std::vector< int > d_seq; // interleaver sequence representing the interleaved indices

 public:
	~drm_interleaver_vbvb ();


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_INTERLEAVER_VBVB_H */

