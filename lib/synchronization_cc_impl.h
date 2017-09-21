/* -*- c++ -*- */
/* 
 * Copyright 2017 Felix Wunsch.
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

#ifndef INCLUDED_DRM_SYNCHRONIZATION_CC_IMPL_H
#define INCLUDED_DRM_SYNCHRONIZATION_CC_IMPL_H

#include <drm/synchronization_cc.h>
#include "kernel_fixed_lag_correlation.h"

namespace gr {
  namespace drm {
    unsigned int STATE_RM_SEARCH = 1;
    unsigned int STATE_RM_TRACKING = 2;

    class synchronization_cc_impl : public synchronization_cc
    {
     private:
      transm_params* d_tp;
      unsigned int d_nfft;
      unsigned int d_max_nfft;
      unsigned int d_ncp;
      float d_threshold;
      std::vector<flc_kernel> d_flc_kernels; // one kernel per possible configuration
      unsigned int d_state;
      unsigned int d_RM;
      int detect_threshold_crossing(gr_complex *ptr, unsigned int len);
      static const unsigned int d_bufsize = 2048;
      float d_buf[d_bufsize];
      int d_samples_since_symbol_start;

     public:
      void reset(); // resets the synchronization
      synchronization_cc_impl(transm_params* tp);
      ~synchronization_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_SYNCHRONIZATION_CC_IMPL_H */

