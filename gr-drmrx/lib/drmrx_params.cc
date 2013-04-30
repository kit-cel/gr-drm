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
    int nfft[4] = {1152, 1024, 704, 448};
    d_nfft.assign(nfft, nfft+4);

    /* time reference cells [carrier index][phase_index(0,k)]
 * only in the first symbol of each transmission frame */
    int time_A[21][2] = {
	{17, 973},
	{18, 205},
	{19, 717},
	{21, 264},
	{28, 357},
	{29, 357},
	{32, 952},
	{33, 440},
	{39, 856},
	{40, 88},
	{41, 88},
	{53, 68},
	{54, 836},
	{55, 836},
	{56, 836},
	{60, 1008},
	{61, 1008},
	{63, 752},
	{71, 215},
	{72, 215},
	{73, 727}};

    int time_B[19][2] = {
	{14, 304},
	{16, 331},
	{18, 108},
	{20, 620},
	{24, 192},
	{26, 704},
	{32, 44},
	{36, 432},
	{42, 588},
	{44, 844},
	{48, 651},
	{49, 651},
	{50, 651},
	{54, 460},
	{56, 460},
	{62, 944},
	{64, 555},
	{66, 940},
	{68, 428}};

    int time_C[19][2] = {
	{8, 722},
	{10, 466},
	{11, 214},
	{12, 214},
	{14, 479},
	{16, 516},
	{18, 260},
	{22, 577},
	{24, 662},
	{28, 3},
	{30, 771},
	{32, 392},
	{33, 392},
	{36, 37},
	{38, 37},
	{42, 474},
	{44, 242},
	{45, 242},
	{46, 754}};

    int time_D[21][2] = {
	{5, 636},
	{6, 124},
	{7, 788},
	{8, 788},
	{9, 200},
	{11, 688},
	{12, 152},
	{14, 920},
	{15, 920},
	{17, 644},
	{18, 388},
	{20, 652},
	{21, 1014},
	{23, 176},
	{24, 176},
	{26, 752},
	{27, 496},
	{28, 332},
	{29, 432},
	{30, 964},
	{32, 452}};
    
    int ntime_pil[4] = {21, 19, 19, 21};
    std::vector<int> tmp_phase, tmp_pos;
    for(int i = 0; i < 4; i++)
    {
        tmp_phase.clear();
        tmp_pos.clear();

        switch(i) // lazy solution TODO: find a nice one
        {
            case 0: // RM A
                for(int k = 0; k < ntime_pil[i]; k++){ tmp_phase.push_back(time_A[k][1]); tmp_pos.push_back(time_A[k][0]);}
                break;
            case 1: // RM B
                for(int k = 0; k < ntime_pil[i]; k++){ tmp_phase.push_back(time_B[k][1]); tmp_pos.push_back(time_B[k][0]);}
                break;
            case 2: // ...
                for(int k = 0; k < ntime_pil[i]; k++){ tmp_phase.push_back(time_C[k][1]); tmp_pos.push_back(time_C[k][0]);}
                break;
            case 3:
                for(int k = 0; k < ntime_pil[i]; k++){ tmp_phase.push_back(time_D[k][1]); tmp_pos.push_back(time_D[k][0]);}
                break;
            default: 
                std::cout << "drmrx_params: ERROR! invalid RM?\n";
                break;
        }
        d_time_pil_phase.push_back(tmp_phase);
        d_time_pil_pos.push_back(tmp_pos);        
    }

    // min/max occupied carrier numbers for all RM/SO combinations
    unsigned int tab_K_range[8][6] = {{2, 2, -102, -114, -98, -110}, // see DRM standard Table 84
												  {102, 114, 102, 114, 314, 350},
												  {1, 1, -91, -103, -87, -99},
												  {91, 103, 91, 103, 279, 311},
												  {0, 0, 0, -69, 0, -67},
												  {0, 0, 0, 69, 0, 213},
												  {0, 0, 0, -44, 0, -43},
                                        					  {0, 0, 0, 44, 0, 135}};
    std::vector<int> tmp_kmin, tmp_kmax;
    for(int r = 0; r < 4; r++)
    {
        tmp_kmin.clear();
        tmp_kmax.clear();
        for(int s = 0; s < 6; s++)
        {
            tmp_kmin.push_back(tab_K_range[2*r][s]);
            tmp_kmax.push_back(tab_K_range[2*r+1][s]);
        }
        d_k_min.push_back(tmp_kmin);
        d_k_max.push_back(tmp_kmax);
    }
}
