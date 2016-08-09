/* -*- c++ -*- */
/*
 * Copyright 2016 Free Software Foundation, Inc.
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

////////////////////////////////////////////////////////////////////////
// Enable math definitions like M_PI on MSVC
////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES //define before including math header
#endif

////////////////////////////////////////////////////////////////////////
// Provide VLA macro for cross platform dynamic array
////////////////////////////////////////////////////////////////////////

//include this header so it cant be re-included and re-define this __GR_VLA
#include <gnuradio/attributes.h>

//first undefine __GR_VLA for bug C99 macro issue in v3.7.10
#ifdef __GR_VLA
#  undef __GR_VLA
#endif

//provide __GR_VLA with ifdef fix for non MSVC
#ifndef _MSC_VER
#  define __GR_VLA(TYPE, buf, size) TYPE buf[size]
#else
#  define __GR_VLA(TYPE, buf, size) TYPE * buf = (TYPE *) alloca(sizeof(TYPE) * (size))
#endif
