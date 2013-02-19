/* this class performs an (I)FFT */

#ifndef INCLUDED_DRMRX_FFT_H
#define INCLUDED_DRMRX_FFT_H

#include <gr_complex.h>
#include <cmath>
#include <fftw3.h>
#include <volk/volk.h>
#include <cstring>
#include <iostream>

class drmrx_fft
{
    gr_complex* d_in; // input pointer, should point to a memory block allocated with fftw_malloc()
    gr_complex* d_out; // output pointer, should point to a memory block allocated with fftw_malloc()
    unsigned int d_len; // FFT length

    // FFTW Planner
    fftwf_plan d_plan_f;
    fftwf_plan d_plan_r;

    void refresh_plans();

    public:
    void set_in(gr_complex* in){ d_in = in; refresh_plans(); }
    void set_out(gr_complex* out){ d_out = out; refresh_plans(); }
    void set_len(unsigned int len){ d_len = len; refresh_plans(); }

    void transform(); // forward fourier transform (time -> frequency)
    void itransform(); // reverse transform

    drmrx_fft(gr_complex* in, gr_complex* out, unsigned int len);
    ~drmrx_fft();
};

#endif
