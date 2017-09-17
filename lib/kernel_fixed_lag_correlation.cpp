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

#include "kernel_fixed_lag_correlation.h"
#include <volk/volk.h>

namespace gr {
    namespace drm {
        flc_kernel::flc_kernel(unsigned int len, unsigned int lag)
        {
            d_len = len;
            d_lag = lag;
        }
        flc_kernel::~flc_kernel(){};

        void flc_kernel::execute(const std::complex<float> *ibuf, unsigned int nsteps, std::complex<float> *obuf)
        {
            // possible improvements: 1) calculate the sum iteratively (-> can't use VOLK any longer and needs to have state)
            //                        2) introduce an option to skip steps to make it more efficient, possibly with linear interpolation or zero-order-hold
            std::complex<float> dotprod, p1, p2;
            for(int i = 0; i < nsteps; i++) {
                volk_32fc_x2_conjugate_dot_prod_32fc(&dotprod, ibuf + i, ibuf + d_lag + i, d_len);
                volk_32fc_x2_conjugate_dot_prod_32fc(&p1, ibuf + i, ibuf + i, d_len);
                volk_32fc_x2_conjugate_dot_prod_32fc(&p2, ibuf + d_lag + i, ibuf + d_lag + i, d_len);
                obuf[i] = dotprod / std::sqrt( std::real(p1 * p2));
            }
        }
    }
}

