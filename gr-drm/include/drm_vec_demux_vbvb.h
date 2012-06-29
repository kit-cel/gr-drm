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

#ifndef INCLUDED_DRM_VEC_DEMUX_VBVB_H
#define INCLUDED_DRM_VEC_DEMUX_VBVB_H

#include <drm_api.h>
#include <gr_sync_block.h>
#include <iostream>

class drm_vec_demux_vbvb;
typedef boost::shared_ptr<drm_vec_demux_vbvb> drm_vec_demux_vbvb_sptr;

DRM_API drm_vec_demux_vbvb_sptr 
drm_make_vec_demux_vbvb (unsigned int vlen_in, unsigned int n_out, const std::vector<unsigned int> &vlen_out);

/*!
 * \brief Demultiplexes a vector (uchar) of N items to m streams of N1...Nm items.
 *
 */
class DRM_API drm_vec_demux_vbvb : public gr_sync_block
{
	std::vector<int> d_vlen_out;
	unsigned int d_vlen_in;
	unsigned int d_n_out;
	
	friend DRM_API drm_vec_demux_vbvb_sptr drm_make_vec_demux_vbvb (unsigned int vlen_in, unsigned int n_out, const std::vector<int> &vlen_out);

	drm_vec_demux_vbvb (unsigned int vlen_in, unsigned int n_out, const std::vector<int> &vlen_out);

 public:
 	~drm_vec_demux_vbvb ();
 
 	std::vector<int> vlen_out () { return d_vlen_out; }
 	void set_vlen_out (const std::vector<int> &vlen_out) { d_vlen_out = vlen_out; }
 	unsigned int vlen_in () { return d_vlen_in; }
 	void set_vlen_in ( unsigned int vlen_in ) { d_vlen_in = vlen_in; }
 	unsigned int n_out () { return d_n_out; }
 	void set_n_out ( unsigned int n_out ) { d_n_out = n_out; }

	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_VEC_DEMUX_VBVB_H */

