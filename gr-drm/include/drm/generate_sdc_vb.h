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

#ifndef INCLUDED_drm_generate_sdc_vb_H
#define INCLUDED_drm_generate_sdc_vb_H

#include <drm/api.h>
#include <gnuradio/sync_block.h>
#include <drm/transm_params.h>

namespace gr {
  namespace drm {
    class DRM_API generate_sdc_vb : virtual public sync_block
    {
      typedef boost::shared_ptr <generate_sdc_vb> sptr;
      /*!
       * \brief Generates the Service Description Channel (SDC) data.
       *
       */
      static sptr make(transm_params* tp);
    public:
      virtual void init_data(unsigned char* data) = 0; // set SDC bitstream according to config parameters (see DRM standard chapter 6.4)
    };
  }
}
#endif /* INCLUDED_drm_generate_sdc_vb_H */

