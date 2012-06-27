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

#ifndef INCLUDED_DRM_SCRAMBLER_BB_H
#define INCLUDED_DRM_SCRAMBLER_BB_H

#include <drm_api.h>
#include <gr_sync_block.h>
#include <iostream>

class drm_scrambler_bb;
typedef boost::shared_ptr<drm_scrambler_bb> drm_scrambler_bb_sptr;

DRM_API drm_scrambler_bb_sptr 
drm_make_scrambler_bb (unsigned int block_len);

/*!
 * \brief Scrambles the input sequence according to the following polynomial:
 * G(x) = x^9 + x^5 + 1. The scrambler gets reset after block_len samples.
 */
class DRM_API drm_scrambler_bb : public gr_sync_block
{
	unsigned int d_block_len;
	
	friend DRM_API drm_scrambler_bb_sptr drm_make_scrambler_bb (unsigned int block_len);

	drm_scrambler_bb (unsigned int block_len);

 public:
	~drm_scrambler_bb ();
	
	unsigned int block_len();

	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DRM_SCRAMBLER_BB_H */

