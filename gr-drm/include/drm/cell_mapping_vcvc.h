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

#ifndef INCLUDED_DRM_CELL_mapping_vcvc_H
#define INCLUDED_DRM_CELL_mapping_vcvc_H

#include <drm/api.h>
#include <gnuradio/sync_interpolator.h>
//#include <cmath>
//#include <complex>
#include <drm/transm_params.h>

namespace gr
{
  namespace drm
  {
    class DRM_API cell_mapping_vcvc : virtual public sync_interpolator
    {
    public:
      typedef boost::shared_ptr<cell_mapping_vcvc> sptr;
      /*!
       * \brief Performs mapping of MSC, SDC, FAC and pilot cells onto the OFDM super transmission frame.
       */
      static sptr
      make(transm_params* tp, std::vector<int> input_sizes);

      virtual bool
      is_used_carrier(int k) = 0;
    };
  }
}
#endif /* INCLUDED_DRM_CELL_mapping_vcvc_H */

