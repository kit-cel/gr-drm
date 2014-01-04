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

#ifndef INCLUDED_DRM_CELL_INTERLEAVER_VCVC_H
#define INCLUDED_DRM_CELL_INTERLEAVER_VCVC_H

#include <drm/api.h>
#include <gnuradio/block.h>

namespace gr
{
  namespace drm
  {
    class DRM_API cell_interleaver_vcvc : virtual public block
    {
    public:
      typedef boost::shared_ptr<cell_interleaver_vcvc> sptr;
      /*!
       * \brief Performs cell interleaving. The long_interl flag determines if long interleaving is used.
       *
       */
      static sptr
      make(std::vector<int> interl_seq, bool long_interl, int depth);
    };
  }
}

#endif /* INCLUDED_DRM_CELL_INTERLEAVER_VCVC_H */

