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
 
#ifndef _DRM_UTIL_H
#define _DRM_UTIL_H

#include <cstring>
#include <cmath>
#include <vector>
#include <drm/transm_params.h>

namespace gr {
  namespace drm {
    // enqueue bit array arr of lenght len at address ptr
    void enqueue_bits(unsigned char* &ptr, unsigned int len, unsigned char arr[]);

    // enqueue bit array of length len with decimal representation val (max->unsigned int) at address ptr
    void enqueue_bits_dec(unsigned char* &ptr, unsigned int len, unsigned int val);

    // enqueue crc word of order ord after bit sequence of length len at address ptr
    void enqueue_crc(unsigned char* ptr, transm_params* tp, int len, unsigned short ord);

    // enqueue array arr of lenght len to vector vec
    void enqueue_array_to_vector(std::vector< unsigned char >* vec, const unsigned char* arr, int len);

    // dequeue 8 bits from array ptr and interpret them as unsigned char
    unsigned char dequeue_char(unsigned char* &ptr);

    // dequeue array of length len from array ptr (unsigned char) and interpret as unsigned int
    unsigned int dequeue_dec(unsigned char* &ptr, unsigned int len);
  }
}
#endif
