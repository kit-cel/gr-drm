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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "synchronization_cc_impl.h"

namespace gr {
  namespace drm {

    synchronization_cc::sptr
    synchronization_cc::make(transm_params* tp)
    {
      return gnuradio::get_initial_sptr
        (new synchronization_cc_impl(tp));
    }

    /*
     * The private constructor
     */
    synchronization_cc_impl::synchronization_cc_impl(transm_params* tp)
      : gr::sync_block("synchronization_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 5, sizeof(gr_complex))),
              d_tp(tp),
              d_tracking(false)
    {
        d_nfft = d_tp->ofdm().nfft();
        d_ncp = d_tp->ofdm().n_cp();
        d_threshold = 0.8; // hardcoded for now
        for(int i=RM_A; i <= RM_D; i++)
        {
            int nfft = d_tp->cfg().ptables()->d_nfft[i];
            int ncp = d_tp->cfg().ptables()->d_ncp[i];
            std::cout << "i:" << i << "//" << nfft << ',' <<  ncp << std::endl;
            d_flc_kernels.push_back(flc_kernel(ncp, nfft));
        }
        set_output_multiple(2048); // this is larger than the largest combination of nfft + ncp, but still reasonable
    }

    /*
     * Our virtual destructor.
     */
    synchronization_cc_impl::~synchronization_cc_impl()
    {
    }

    void synchronization_cc_impl::reset()
    {
        d_tracking = false;
    }

    int
    synchronization_cc_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *isig = (const gr_complex *) input_items[0];
      gr_complex *osig = (gr_complex *) output_items[0];
      gr_complex* flc[4] = {(gr_complex *) output_items[RM_A+1],
                            (gr_complex *) output_items[RM_B+1],
                            (gr_complex *) output_items[RM_C+1],
                            (gr_complex *) output_items[RM_D+1]
                            };

      if (!d_tracking) {
          // perform fixed lag correlations for all different modes
          int nsteps = noutput_items - (1152 + 128);
          for (int i = RM_A; i <= RM_D; i++) {
              d_flc_kernels[i].execute(isig, nsteps, flc[i]);
          }
          memcpy(osig, isig, nsteps * sizeof(gr_complex));
      }

      return nsteps;
    }

  } /* namespace drm */
} /* namespace gr */

