#include "drmrx_fft.h"

drmrx_fft::drmrx_fft(gr_complex* in, gr_complex* out, unsigned int len)
{
    d_in = in;
    d_out = out;
    d_len = len;

    refresh_plans();
}

drmrx_fft::~drmrx_fft()
{
    fftwf_destroy_plan(d_plan_f);
    fftwf_destroy_plan(d_plan_r);
}

void
drmrx_fft::refresh_plans()
{
    // FFTW Planner
    d_plan_f = fftwf_plan_dft_1d(d_len, reinterpret_cast<fftwf_complex*>(d_in), reinterpret_cast<fftwf_complex*>(d_out), FFTW_FORWARD, FFTW_ESTIMATE);
    d_plan_r = fftwf_plan_dft_1d(d_len, reinterpret_cast<fftwf_complex*>(d_in), reinterpret_cast<fftwf_complex*>(d_out), FFTW_BACKWARD, FFTW_ESTIMATE);
}

void
drmrx_fft::transform()
{
    fftwf_execute(d_plan_f);
}

void
drmrx_fft::itransform()
{
    fftwf_execute(d_plan_r);
}
