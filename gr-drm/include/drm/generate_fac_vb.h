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

#ifndef INCLUDED_DRM_GENERATE_FAC_VB_H
#define INCLUDED_DRM_GENERATE_FAC_VB_H

#include <drm/api.h>
#include <gnuradio/sync_block.h>
#include <drm/transm_params.h>

namespace gr {
  namespace drm {
    class DRM_API generate_fac_vb : virtual public sync_block
    {
    public:
      typedef boost::shared_ptr <generate_fac_vb> sptr;
      /*!
       * \brief Generates the Fast Access Channel (FAC) data
       *
       */
      static sptr make (transm_params* tp);

      virtual void init_data(unsigned char* data) = 0; //  set FAC bitstream according to config parameters (see DRM standard chapter 6.3)

      virtual void increment_tf_ctr() = 0; // increments tf_ctr and takes account of wraparound;
    };
  }
}
#endif /* INCLUDED_DRM_GENERATE_FAC_VB_H */

