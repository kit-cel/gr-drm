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
#include <volk/volk.h>
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
              d_state(STATE_RM_SEARCH),
              d_RM(0)
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
        d_state = STATE_RM_SEARCH;
    }
    
    int synchronization_cc_impl::detect_threshold_crossing(gr_complex *ptr, unsigned int len)
    {
      unsigned int offset = 0;
      unsigned short max_index = 0;
      unsigned int bufsize = 0;
      while(true)
      {
        bufsize = std::min(d_bufsize, len - offset);
        volk_32fc_magnitude_squared_32f(d_buf, ptr + offset, bufsize);
        volk_32f_index_max_16u(&max_index, d_buf, bufsize);
        std::cout << max_index << std::endl;
        if(d_buf[max_index] > d_threshold){
          return offset + max_index;
        }
        offset += d_bufsize;
        if(offset > len){
          return -1;
        }
      }
    }

    int
    synchronization_cc_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *insig = (const gr_complex *) input_items[0];
      gr_complex *outsig = (gr_complex *) output_items[0];
      gr_complex* flc[4] = {(gr_complex *) output_items[RM_A+1],
                            (gr_complex *) output_items[RM_B+1],
                            (gr_complex *) output_items[RM_C+1],
                            (gr_complex *) output_items[RM_D+1]
                            };

      // perform fixed lag correlations for all different modes
      int nsteps = noutput_items - (1152 + 128);
      for (int i = RM_A; i <= RM_D; i++) {
            d_flc_kernels[i].execute(insig, nsteps, flc[i]);
      }
      if (d_state == STATE_RM_SEARCH) {
        int threshold_crossed_index = -1;
        for (int i = RM_A; i <= RM_D; i++) {
            threshold_crossed_index = detect_threshold_crossing(flc[i], nsteps);
            if(threshold_crossed_index >= 0){
              d_state = STATE_RM_TRACKING; // set state
              d_RM = i; // set RM
              d_nfft = d_tp->cfg().ptables()->d_nfft[d_RM]; // set FFT length
              d_ncp = d_tp->cfg().ptables()->d_ncp[d_RM]; // set CP length
              GR_LOG_INFO(d_logger, "Signal detected: set to STATE_RM_TRACKING");
              
              // set symbol start index delayed by half the CP length to ensure it's in 
              // the second half of the CP. The (true) maximum of the FLC corresponds 
              // to the beginning of the CP. The threshold is usually crossed
              // too early, so a delay by ncp/4 should be fine.
              unsigned int symbol_start_index = threshold_crossed_index + d_ncp / 4;
              + d_tp->cfg().ptables()->d_ncp[d_RM]/2;
              add_item_tag(0, nitems_written(0) + symbol_start_index, pmt::mp("RM_detected"), 
              pmt::from_long(d_RM));
              // consume all samples up to the threshold crossing so that the tracking 
              // is as simple as possible
              memcpy(outsig, insig, threshold_crossed_index * sizeof(gr_complex));
              return threshold_crossed_index;
              //d_samples_since_symbol_start = (nsteps - symbol_start_index) % (d_nfft + d_ncp);
            }
        }
        // threshold was not crossed, so return the entire buffer
        memcpy(outsig, insig, nsteps * sizeof(gr_complex));
        return nsteps;
      }
      else if (d_state == STATE_RM_TRACKING)
      {
        unsigned int offset = 0;
        while(d_nfft + d_ncp - offset <= nsteps)
        {
          gr_complex flc_val;
          d_flc_kernels[d_RM].execute(insig + offset, 1, &flc_val);
          offset += d_nfft + d_ncp;
          if(std::abs(flc_val) < d_threshold){ 
            GR_LOG_INFO(d_logger, "sync lost: reset to STATE_RM_SEARCH");
            reset();
            break;
          }
        }
        memcpy(outsig, insig, offset * sizeof(gr_complex));
        return offset;
      }
      else {
        throw std::runtime_error("This should not be reached.");
      }

      
    }

  } /* namespace drm */
} /* namespace gr */

