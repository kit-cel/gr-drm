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

#ifndef INCLUDED_DRM_ADD_TAILBITS_VBVB_H
#define INCLUDED_DRM_ADD_TAILBITS_VBVB_H

#include <drm_api.h>
#include <gr_sync_block.h>

class drm_add_tailbits_vbvb;
typedef boost::shared_ptr<drm_add_tailbits_vbvb> drm_add_tailbits_vbvb_sptr;

DRM_API drm_add_tailbits_vbvb_sptr drm_make_add_tailbits_vbvb (int vlen_in, int n_tailbits);

/*!
 * \brief Adds n_tailbits zeros to a vector of lenght vlen_in
 *
 */
class DRM_API drm_add_tailbits_vbvb : public gr_sync_block
{
	friend DRM_API drm_add_tailbits_vbvb_sptr drm_make_add_tailbits_vbvb (int vlen_in, int n_tailbits);

	drm_add_tailbits_vbvb (int vlen_in, int n_tailbits);
	
	int d_vlen;
	int d_n_tail;

 public:
	~drm_add_tailbits_vbvb ();


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_ADD_TAILBITS_VBVB_H */

