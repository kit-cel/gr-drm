/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_DRM_SCRAMBLER_VBVB_H
#define INCLUDED_DRM_SCRAMBLER_VBVB_H

#include <drm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace drm {

    /*!
     * \brief Scrambles the input sequence according to the following polynomial:
     * G(x) = x^9 + x^5 + 1. The scrambler is reset after 'block_len' samples.
     * \ingroup drm
     *
     */
    class DRM_API scrambler_vbvb : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<scrambler_vbvb> sptr;

      /*!
       * \brief Scrambles the input sequence according to the following polynomial:
       * G(x) = x^9 + x^5 + 1. The scrambler is reset after 'block_len' samples.
       */
      static sptr make(unsigned int block_len);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_SCRAMBLER_VBVB_H */

