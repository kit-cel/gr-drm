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

#ifndef INCLUDED_drm_generate_sdc_vb_IMPL_H
#define INCLUDED_drm_generate_sdc_vb_IMPL_H

#include <drm/generate_sdc_vb.h>

namespace gr {
  namespace drm {
    class DRM_API generate_sdc_vb_impl : public generate_sdc_vb
    {
    private:
            transm_params* d_tp;

     public:
            /*!
             * \brief Generates the Service Description Channel (SDC) data.
             *
             */
            generate_sdc_vb_impl (transm_params* tp);

            ~generate_sdc_vb_impl ();

            void init_data(unsigned char* data); // set SDC bitstream according to config parameters (see DRM standard chapter 6.4)

            int work (int noutput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items);
    };
  }
}
#endif /* INCLUDED_drm_generate_sdc_vb_IMPL_H */

