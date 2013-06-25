/* This class contains all possible parameters for a DRM receiver. */

#ifndef DRMRX_PARAMS_H
#define DRMRX_PARAMS_H

#include <vector>
#include <iostream>
#include <cstdio>
#include <gr_complex.h>

class drmrx_params
{
    // input sampling rate
    int d_FS;

    // robustness modes
    int d_RM_A; // 0 -> best channel
    int d_RM_B;
    int d_RM_C;
    int d_RM_D; // 3 -> worst channel
    int d_RM_E; // 4 -> DRM+
    int d_RM_NONE; // 5 -> invalid

    // spectrum occupancies
    int d_SO_0; // 0 -> 4.5 kHz, if RM==RM_E this corresponds to 100 kHz
    int d_SO_1; // 1 -> 5 kHz
    int d_SO_2; // 2 -> 9 kHz
    int d_SO_3; // 3 -> 10 kHz
    int d_SO_4; // 4 -> 18 kHz
    int d_SO_5; // 5 -> 20 kHz
    int d_SO_NONE; // 6 -> invalid

    // OFDM parameters. Lenghts are in samples

    // number of samples per frame (NOT superframe)
    std::vector<int> d_nsym_frame;

    // symbol length (with cyclic prefix)
    std::vector<int> d_nsamp_Ts;

    // cyclic prefix length
    std::vector<int> d_nsamp_Tg;

    // symbol length (without cyclic prefix)
    std::vector<int> d_nsamp_Tu;

    // FFT lengths for different RMs
    std::vector<int> d_nfft;

    // time pilots (position and phase index)
    std::vector< std::vector< int > > d_time_pil_phase;
    std::vector< std::vector< int > > d_time_pil_pos;

    // frequency pilots
    std::vector< std::vector< int > > d_freq_pil_pos;

    // gain pilots
    std::vector< std::vector< std::vector< std::vector< int > > > > d_gain_pil_pos; //[RM][SO][symbol][carrier]
    std::vector< std::vector<int> > d_gain_boosted; // boosted gain pilot cells
    std::vector< std::vector< std::vector< std::vector< gr_complex > > > > d_gain_pil_values; //[RM][SO][symbol][carrier] actual complex pilot symbols

    // edge carrier indexes
    std::vector< std::vector< int > > d_k_min;
    std::vector< std::vector< int > > d_k_max;


 public:
    // get methods for python access through SWIG

    // sampling rate
    int FS(){ return d_FS; }

    // robustness modes
    int RM_A(){ return d_RM_A; }
    int RM_B(){ return d_RM_B; }
    int RM_C(){ return d_RM_C; }
    int RM_D(){ return d_RM_D; }
    int RM_E(){ return d_RM_E; }
    int RM_NONE(){ return d_RM_NONE; }

    // spectrum occupancies
    int SO_0(){ return d_SO_0; }
    int SO_1(){ return d_SO_1; }
    int SO_2(){ return d_SO_2; }
    int SO_3(){ return d_SO_3; }
    int SO_4(){ return d_SO_4; }
    int SO_5(){ return d_SO_5; }
    int SO_NONE(){ return d_SO_NONE; }

    // OFDM parameters
    std::vector<int> nsym_frame(){ return d_nsym_frame; }
    std::vector<int> nsamp_Ts(){ return d_nsamp_Ts; }
    std::vector<int> nsamp_Tg(){ return d_nsamp_Tg; }
    std::vector<int> nsamp_Tu(){ return d_nsamp_Tu; }
    std::vector<int> nfft(){ return d_nfft; }

    // pilot cell positions and phase indexes
    std::vector<std::vector<int> > time_pil_phase(){ return d_time_pil_phase; }
    std::vector<std::vector<int> > time_pil_pos(){ return d_time_pil_pos; }
    std::vector<std::vector<int> > freq_pil_pos(){ return d_freq_pil_pos; }
    std::vector<std::vector<std::vector< std::vector<int> > > > gain_pil_pos(){ return d_gain_pil_pos; }
    std::vector<std::vector<std::vector< std::vector<gr_complex > > > > gain_pil_values(){ return d_gain_pil_values; }
    std::vector<std::vector<int> > gain_boosted(){ return d_gain_boosted; }

    // edge carrier indexes
    std::vector<std::vector<int> > k_min(){ return d_k_min; }
    std::vector<std::vector<int> > k_max(){ return d_k_max; }

    // constructor and destructor
    drmrx_params(); // constructor, initializes all the values
    ~drmrx_params(){} // destructor

};

#endif
