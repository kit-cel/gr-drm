/* -*- c++ -*- */
/*
 * Copyright 2014 Felix Wunsch, Communications Engineering Lab (CEL) / Karlsruhe Institute of Technology (KIT).
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


#ifndef INCLUDED_DRM_ADD_TAILBITS_VBVB_H
#define INCLUDED_DRM_ADD_TAILBITS_VBVB_H

#include <drm/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace drm {

    /*!
     * \brief <+description of block+>
     * \ingroup drm
     *
     */
    class DRM_API add_tailbits_vbvb : virtual public block
    {
     public:
      typedef boost::shared_ptr<add_tailbits_vbvb> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of drm::add_tailbits_vbvb.
       *
       * To avoid accidental use of raw pointers, drm::add_tailbits_vbvb's
       * constructor is in a private implementation
       * class. drm::add_tailbits_vbvb::make is the public interface for
       * creating new instances.
       */
      static sptr make(int vlen_in, int n_tailbits);

    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_ADD_TAILBITS_VBVB_H */

