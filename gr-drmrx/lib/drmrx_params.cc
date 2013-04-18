#include "drmrx_params.h"
#include <math.h>

drmrx_params::drmrx_params()
{
    // input sampling rate
    d_FS = 48000;

    // robustness modes
    d_RM_A = 0;
    d_RM_B = 1;
    d_RM_C = 2;
    d_RM_D = 3;
    d_RM_E = 4;
    d_RM_NONE = 5;

    // spectrum occupancies
    d_SO_0 = 0;
    d_SO_1 = 1;
    d_SO_2 = 2;
    d_SO_3 = 3;
    d_SO_4 = 4;
    d_SO_5 = 5;
    d_SO_NONE = 6;

    // OFDM parameters

    // durations in seconds, needed for calculation
    float Tu[5] = {0.024, 0.02133, 0.01466, 0.00933, 0.00225};
    float Tg[5] = {0.00266, 0.00533, 0.00533, 0.00733, 0.00025};
    float Ts[5];
    for( int i = 0; i < 5; i++)
    {
        Ts[i] = Tu[i] + Tg[i];
    }

    // now convert time duration in samples by multiplying with FS
    for(int i = 0; i < 5; i++)
    {
        d_nsamp_Ts.push_back(round(Ts[i]*float(d_FS)));
        d_nsamp_Tu.push_back(round(Tu[i]*float(d_FS)));
        d_nsamp_Tg.push_back(round(Tg[i]*float(d_FS)));
    }
}
