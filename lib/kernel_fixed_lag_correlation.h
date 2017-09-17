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

#ifndef GR_DRM_KERNEL_FIXED_LAG_CORRELATION_H
#define GR_DRM_KERNEL_FIXED_LAG_CORRELATION_H

#include <complex>
#include <vector>


namespace gr {
    namespace drm {

        class flc_kernel {
        private:
            unsigned int d_len;
            unsigned int d_lag;

        public:
            flc_kernel(unsigned int len, unsigned int lag);

            ~flc_kernel();

            void execute(const std::complex<float> *ibuf, unsigned int nsteps, std::complex<float> *obuf);
        };
    }
}

#endif //GR_DRM_KERNEL_FIXED_LAG_CORRELATION_H
