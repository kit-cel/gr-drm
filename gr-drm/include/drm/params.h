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

#ifndef _DRM_PARAMS_H_
#define _DRM_PARAMS_H_

#include <drm/api.h>
#include <drm/global_constants.h>
#include <drm/tables.h>
#include <drm/config.h>
#include <iostream>
#include <vector>
#include <cmath>

namespace gr
{
  namespace drm
  {
    /* classes holding the derived parameters and init routines (where parameters are calculated)*/
    class DRM_API global_params
    {
    public:
      virtual void
      init(config* cfg) = 0;

      global_params()
      {
      }
      ;
      virtual
      ~global_params()
      {
      }
      ;
    };

    class DRM_API ofdm_params : public global_params
    {
      unsigned int d_nfft; // FFT length
      unsigned int d_n_cp; // length of guard interval (samples)
      unsigned short d_cp_ratio_enum; // enumerator of n_cp / nfft ratio
      unsigned short d_cp_ratio_denom; // denominator of n_cp / nfft ratio
      int d_K_min; // index of lowest carrier
      int d_K_max; // index of highest carrier
      unsigned int d_n_unused; // number of unused carriers (always symmetric to DC)
      unsigned int d_N_S; // number of OFDM symbols per transmission frame
      unsigned short d_M_TF; // number of transmission frames per super transmission frame
      unsigned int d_fs_soundcard; // soundcard sample rate

    public:
      /* accessor methods */
      unsigned int
      nfft();
      unsigned int
      n_cp();
      unsigned short
      cp_ratio_enum();
      unsigned short
      cp_ratio_denom();
      int
      K_min();
      int
      K_max();
      unsigned int
      n_unused();
      unsigned int
      N_S();
      unsigned short
      M_TF();
      unsigned int
      fs_soundcard();

      ofdm_params();
      virtual
      ~ofdm_params()
      {
      }
      ;

      void
      init(config* cfg);
    };

    class DRM_API channel_params : public global_params
    {
    protected:
      std::vector<int> d_r_p; // index for tailbit puncturing pattern
      int d_mod_order; // modulation order (bits per symbol)
      std::vector<int> d_bit_interl_seq_0_1; // sequence for higher protected part ('_1')
      std::vector<int> d_bit_interl_seq_0_2; // bit interleaving sequence for interleaver with index 0, lower protected part ('_2') (see DRM standard, chapter 7.3.3)

    public:
      std::vector<int>
      r_p();

      int
      calc_r_p(int N, int RY_p_);
      void
      set_punct_pat(std::vector<unsigned char>* pp, int e, int d, tables* t);
      void
      set_punct_pat_tail(std::vector<unsigned char>* pp, int r_p, tables* t);
      void
      set_interl_seq(std::vector<int>* seq, int x_in, int mod_order, int index); // calculates the interleaver sequence (same algorithm applies for bit and cell interleaving)
      int
      mod_order();
      void
      set_mod_order(int order);
      std::vector<int>
      bit_interl_seq_0_1();
      std::vector<int>
      bit_interl_seq_0_2();
      channel_params();
      virtual
      ~channel_params()
      {
      }
      ;
    };

    class DRM_API control_chan_params : public channel_params
    {
    protected:
      unsigned int d_L; // length in bits
      unsigned int d_N; // number of QAM symbols

      float d_R_0; // coderate R_0 TODO: is this value necessary?
      unsigned short d_R_0_enum; // enumerator of R_0
      unsigned short d_R_0_denom; // denominator of R_0
      std::vector<int> d_M_total; // total number of samples per coding level

      std::vector<unsigned char> d_punct_pat_0; // puncturing pattern (R_0)
      std::vector<unsigned char> d_punct_pat_tail_0; // puncturing pattern for the tailbits (R_0)

    public:
      unsigned int
      L();
      unsigned int
      N();
      float
      R_0();
      unsigned short
      R_0_enum();
      unsigned short
      R_0_denom();
      std::vector<int>
      M_total();

      std::vector<unsigned char>
      punct_pat_0();
      std::vector<unsigned char>
      punct_pat_tail_0();

      control_chan_params();
      virtual
      ~control_chan_params()
      {
      }
      ;
    };

    class DRM_API sdc_params : public control_chan_params
    {
      float d_R_1; // coderate R_1
      unsigned short d_R_1_enum; // enumerator of R_1
      unsigned short d_R_1_denom; // denominator of R_1
      unsigned int d_n_bytes_datafield; // number of bytes in the SDC data field
      std::vector<int> d_M_index; // indexes needed for partitioning
      unsigned short d_n_levels_mlc; // number of levels in the multi level coding process

      std::vector<unsigned char> d_punct_pat_1; // puncturing pattern (R_1)
      std::vector<unsigned char> d_punct_pat_tail_1; // puncturing pattern for the tailbits (R_1)

      std::vector<int> d_bit_interl_seq_1_1;
      std::vector<int> d_bit_interl_seq_1_2; // bit interleaver sequence for interleaver with index 1, lower protected part

    public:
      float
      R_1();
      unsigned short
      R_1_enum();
      unsigned short
      R_1_denom();
      unsigned int
      n_bytes_datafield();
      std::vector<int>
      M_index();
      unsigned short
      n_levels_mlc();

      std::vector<unsigned char>
      punct_pat_1();
      std::vector<unsigned char>
      punct_pat_tail_1();

      std::vector<int>
      bit_interl_seq_1_1();
      std::vector<int>
      bit_interl_seq_1_2();

      sdc_params();
      virtual
      ~sdc_params()
      {
      }
      ;

      void
      init(config* cfg);
    };

    class DRM_API fac_params : public control_chan_params
    {
    public:
      fac_params()
      {
      }
      ;
      virtual
      ~fac_params()
      {
      }
      ;

      void
      init(config* cfg);
    };

    class DRM_API msc_params : public channel_params
    {
      /* NOTE: so far, only SM is implemented! TODO: implement hierarchical mapping */
      unsigned int d_L_MUX; // number of bits per multiplex frame
      unsigned int d_L_1; // higher protected bits
      unsigned int d_L_2; // lower protected bits
      unsigned int d_L_VSPP; // very strongly protected bits
      unsigned int d_N_MUX; // number of QAM cells per multiplex frame
      unsigned int d_N_1; // higher protected cells
      unsigned int d_N_2; // lower protected cells

      unsigned short d_MSC_prot_1; // MSC protection level for higher protected part (see 7.1 and tables 64-70 in the standard)
      unsigned short d_MSC_prot_2; // protection level for lower protected part

      unsigned short d_R_0_enum_1; // enumerator of R0 for the higher protected part
      unsigned short d_R_0_denom_1; // denominator of ...
      unsigned short d_R_1_enum_1;
      unsigned short d_R_1_denom_1;
      unsigned short d_R_2_enum_1;
      unsigned short d_R_2_denom_1;
      unsigned short d_R_Ylcm_1;
      unsigned short d_R_0_enum_2; // enumerator of R0 for the lower protected part
      unsigned short d_R_0_denom_2;
      unsigned short d_R_1_enum_2;
      unsigned short d_R_1_denom_2;
      unsigned short d_R_2_enum_2;
      unsigned short d_R_2_denom_2;
      unsigned short d_R_Ylcm_2;
      std::vector<int> d_M_index; // indexes needed for partitioning
      std::vector<int> d_M_total; // total number of bits on each encoder level
      unsigned short d_n_levels_mlc; // number of levels in the multi level coding process

      // TODO: add patterns for UEP (suffix _1)
      std::vector<unsigned char> d_punct_pat_0_2; // puncturing pattern (R_0)
      std::vector<unsigned char> d_punct_pat_tail_0_2; // puncturing pattern for the tailbits (R_0)
      std::vector<unsigned char> d_punct_pat_1_2; // puncturing pattern (R_1)
      std::vector<unsigned char> d_punct_pat_tail_1_2; // puncturing pattern for the tailbits (R_1)
      std::vector<unsigned char> d_punct_pat_2_2; // puncturing pattern (R_2)
      std::vector<unsigned char> d_punct_pat_tail_2_2; // puncturing pattern for the tailbits (R_2)

      std::vector<int> d_bit_interl_seq_1_1; // sequence for interleaver 1, higher protected part
      std::vector<int> d_bit_interl_seq_1_2; // sequence for interleaver 1, lower protected part
      std::vector<int> d_bit_interl_seq_2_1;
      std::vector<int> d_bit_interl_seq_2_2; // ... interleaver 2
      std::vector<int> d_cell_interl_seq; // Cell (symbols) interleaving sequence

    public:
      unsigned int
      L_MUX();
      unsigned int
      L_1();
      unsigned int
      L_2();
      unsigned int
      L_VSPP();
      unsigned int
      N_MUX();
      unsigned int
      N_1();
      unsigned int
      N_2();

      unsigned short
      MSC_prot_1();
      unsigned short
      MSC_prot_2();
      unsigned short
      R_0_enum_1();
      unsigned short
      R_0_denom_1();
      unsigned short
      R_1_enum_1();
      unsigned short
      R_1_denom_1();
      unsigned short
      R_2_enum_1();
      unsigned short
      R_2_denom_1();
      unsigned short
      R_Ylcm_1();
      unsigned short
      R_0_enum_2();
      unsigned short
      R_0_denom_2();
      unsigned short
      R_1_enum_2();
      unsigned short
      R_1_denom_2();
      unsigned short
      R_2_enum_2();
      unsigned short
      R_2_denom_2();
      unsigned short
      R_Ylcm_2();
      std::vector<int>
      M_index();
      std::vector<int>
      M_total();
      unsigned short
      n_levels_mlc();
      std::vector<unsigned char>
      punct_pat_0_2();
      std::vector<unsigned char>
      punct_pat_tail_0_2();
      std::vector<unsigned char>
      punct_pat_1_2();
      std::vector<unsigned char>
      punct_pat_tail_1_2();
      std::vector<unsigned char>
      punct_pat_2_2();
      std::vector<unsigned char>
      punct_pat_tail_2_2();

      std::vector<int>
      bit_interl_seq_1_1();
      std::vector<int>
      bit_interl_seq_1_2();
      std::vector<int>
      bit_interl_seq_2_1();
      std::vector<int>
      bit_interl_seq_2_2();
      std::vector<int>
      cell_interl_seq();

      void
      calc_vars_SM(config* cfg);

      msc_params();
      virtual
      ~msc_params()
      {
      }
      ;

      void
      init(config* cfg);
    };

  }
}
#endif /* DRM_INIT_PARAMS_H_ */
