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

    /* OFDM parameters */

    // number of symbols per frame (NOT superframe) depending on RM
    int nsym_frame[5] = {15, 15, 20, 24, 40};
    d_nsym_frame.assign(nsym_frame, nsym_frame + 5);

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

    /* frequency reference cells [carrier index][phase_index(0,k)]
     * corresponds to 750 Hz, 2250 Hz and 3000 Hz */
    int freq_A[3][2] = {
    	{18, 205},
    	{54, 836},
    	{72, 215}};

    int freq_B[3][2] = {
    	{16, 331},
    	{48, 651},
    	{64, 555}};

    int freq_C[3][2] = {
    	{11, 214},
    	{33, 392},
    	{44, 242}};

    int freq_D[3][2] = {
    	{7,	788},
    	{21, 1014},
    	{28, 332}};

    std::vector<int> tmp_pos, tmp_phase;
    for(int i = 0; i < 4; i++)
    {
        switch(i)
        {
        case 0:
            for(int k = 0; k<3; k++){tmp_pos.push_back(freq_A[k][0]);}
            break;
        case 1:
            for(int k = 0; k<3; k++){tmp_pos.push_back(freq_B[k][0]);}
            break;
        case 2:
            for(int k = 0; k<3; k++){tmp_pos.push_back(freq_C[k][0]);}
            break;
        case 3:
            for(int k = 0; k<3; k++){tmp_pos.push_back(freq_D[k][0]);}
            break;
        default:
            std::cout << "drmrx_params: ERROR! invalid RM?\n";
            break;
        }
        d_freq_pil_pos.push_back(tmp_pos);
        tmp_pos.clear();
    }

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
    tmp_phase.clear(); // these vectors are reused, so make sure they are empty
    tmp_pos.clear();
    for(int i = 0; i < 4; i++)
    {
        tmp_phase.clear();
        tmp_pos.clear();

        switch(i) // lazy solution. TODO: find a nice one
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

    /* gain reference cells */
    // edge pilots' power gets boosted by a factor of 2

    printf("start calculating gain cells");
    int gain_boost[5][24] = { // [NUM_RM][NUM_SO*4]
		{2,6,98,102,  2,6,110,114,  -102,-98,98,102,  -114,-110,110,114,  -98,-94,310,314,  -110,-106,346,350},
		{1,3,89,91,  1,3,101,103,  -91,-89,89,91,  -103,-101,101,103,  -87,-85,277,279,  -99,-97,309,311},
		{0,0,0,0,  0,0,0,0,  0,0,0,0,  -69,-67,67,69,  0,0,0,0,  -67,-65,211,213},
		{0,0,0,0,  0,0,0,0,  0,0,0,0,  -44,-43,43,44,  0,0,0,0,  -43,-42,134,135},
		{-106,-102,102,106,  0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0}};

    for(int rm = 0; rm < 5; rm++)
    {
        tmp_pos.clear();
        for(int i = 0; i < 24; i++)
        {
            tmp_pos.push_back(gain_boost[rm][i]);
        }
        d_gain_boosted.push_back(tmp_pos);
    }

    int gain_W_A[5][3] = {
        {228, 341, 455},
        {455, 569, 683},
        {683, 796, 910},
        {910,   0, 114},
        {114, 228, 341}
    };
    int gain_Z_A[5][3] = {
        {0,    81, 248},
        {18,  106, 106},
        {122, 116,  31},
        {129, 129,  39},
        {33,   32, 111}
    };
    int gain_Q_A = 36;

    int gain_W_B[3][5] = {
        {512,   0, 512,   0, 512},
        {0,   512,   0, 512,   0},
        {512,   0, 512,   0, 512}
    };
    int gain_Z_B[3][5] = {
        {0,    57, 164,  64,  12},
        {168, 255, 161, 106, 118},
        {25,  232, 132, 233,  38}
    };
    int gain_Q_B = 12;

    int gain_W_C[2][10] = {
        {465, 372, 279, 186,  93,   0, 931, 838, 745, 652},
        {931, 838, 745, 652, 559, 465, 372, 279, 186,  93}
    };
    int gain_Z_C[2][10] = {
        {0,    76, 29,  76,   9, 190, 161, 248,  33, 108},
        {179, 178, 83, 253, 127, 105, 101, 198, 250, 145}
    };
    int gain_Q_C = 12;

    int gain_W_D[3][8] = {
        {366, 439, 512, 585, 658, 731, 805, 878},
        {731, 805, 878, 951,   0,  73, 146, 219},
        {73,  146, 219, 293, 366, 439, 512, 585}
    };
    int gain_Z_D[3][8] = {
        {0,   240,  17,  60, 220,  38, 151, 101},
        {110,   7,  78,  82, 175, 150, 106,  25},
        {165,   7, 252, 124, 253, 177, 197, 142}
    };
    int gain_Q_D = 14;

    int gain_R_E[4][10] = {
        {  39, 118, 197, 276, 354, 433,  39, 118, 197, 276},
        {  37, 183, 402,  37, 183, 402,  37, 183, 402,  37},
        { 110, 329, 475, 110, 329, 475, 110, 329, 475, 110},
        {  79, 158, 236, 315, 394, 473,  79, 158, 236, 315}
    };
    int gain_Z_E[4][10] = {
        { 473, 394, 315, 236, 158,  79,   0,   0,   0,   0},
        { 183, 914, 402,  37, 475, 841, 768, 768, 987, 183},
        { 549, 622, 475, 110,  37, 622, 256, 768, 329, 549},
        {  79, 158, 236, 315, 394, 473, 158, 315, 473, 630}
    };
    int gain_Q_E[4][10] = {
        { 329, 489, 894, 419, 607, 519,1020, 942, 817, 939},
        { 824,1023,  74, 319, 225, 207, 348, 422, 395,  92},
        { 959, 379,   7, 738, 500, 920, 440, 727, 263, 733},
        { 907, 946, 924,  91, 189, 133, 910, 804,1022, 433}
    };

    //void tables::calc_gain_cell_params(unsigned short rm, int so, unsigned int n_sym, int k_min, int k_max)
    // calculate all possible gain cell positions and phases
    printf("enter pos calc");
    // gain pilot carrier positions
    for(int rm = 0; rm < 4; rm++) // no RM E here
    {
        // tmp vectors, also used in phase calculation
        std::vector< int > tmp_sym_rmso_fixed;
        std::vector< std::vector<int> > tmp_frame_rmso_fixed;
        std::vector< std::vector< std::vector<int> > > tmp_frame_rm_fixed;
        tmp_frame_rm_fixed.clear();
        for(int so = 0; so < 6; so++)
        {
            // for calculation details see DRM standard chapter 8.4.4

            int c1, c2, c3, c4; //  constants needed for index calculation
            int p_min, p_max;
            switch(rm) //  determine constants
            {
                case 0: // A
                    c1 = 2;
                    c2 = 4;
                    c3 = 5;
                    c4 = 20;
                    break;
                case 1: // B
                    c1 = 1;
                    c2 = 2;
                    c3 = 3;
                    c4 = 6;
                    break;
                case 2: // C
                    c1 = 1;
                    c2 = 2;
                    c3 = 2;
                    c4 = 4;
                    break;
                case 3: // D
                    c1 = 1;
                    c2 = 1;
                    c3 = 3;
                    c4 = 3;
                    break;
                case 4: // E
                    c1 = 2;
                    c2 = 4;
                    c3 = 4;
                    c4 = 16;
                    break;
                default:
                    break;
            }

            p_min = (d_k_min[rm][so] - c1 - c2 * c3) / c4 - 5; // calculate p_min that ensures we reach k_min
            p_max = (d_k_max[rm][so] - c1 - c2 * c3) / c4 + 5; // calculate p_max that ensures we reach k_max
            int cur_k;
            tmp_frame_rmso_fixed.clear();
            for(int s = 0; s < d_nsym_frame[rm]; s++) // the pattern has a shorter periodicity than s but s is an integer multiple of the pattern length
            {
                tmp_sym_rmso_fixed.clear();

                for(int p = p_min; p < p_max; p++)
                {
                    cur_k = c1 + c2 * (s % c3) + c4 * p; //  actual calculation

                    if(cur_k <= d_k_max[rm][so]&& cur_k >= d_k_min[rm][so]) // keep value if it's a valid index
                    {
                        tmp_sym_rmso_fixed.push_back(cur_k);
                    }
                }
                tmp_frame_rmso_fixed.push_back(tmp_sym_rmso_fixed);
            }
            tmp_frame_rm_fixed.push_back(tmp_frame_rmso_fixed);
        }
        d_gain_pil_pos.push_back(tmp_frame_rm_fixed);
    }

    // gain pilot phases
    printf("enter phase calc");
    for(int rm = 0; rm < 4; rm++) // no RM E here
    {
        std::vector< std::vector<gr_complex > > tmp_frame_rmso_fixed;
        std::vector< std::vector< std::vector<gr_complex > > > tmp_frame_rm_fixed;
        tmp_frame_rm_fixed.clear();
        for(int so = 0; so < 6; so++)
        {
            /* cell phases (if the locations of gain cells coincide with other reference cells, the other phases take precedence) */
            std::vector< std::vector< int > > gain_phase_index; // keeps phase values for 1 frame
            int x, y, k0; // constants needed for phase calculation
            int n, m , p; // variables needed for phase calculation
            std::vector< int > v_tmp; // this vector is used for phase calculation and for pushing to d_gain_phase.
            switch(rm)
            {
                case 0: // A
                    x = 4;
                    y = 5;
                    k0 = 2;
                    break;
                case 1: // B
                {
                    x = 2;
                    y = 3;
                    k0 = 1;
                    break;
                }
                case 2: // C
                    x = 2;
                    y = 2;
                    k0 = 1;
                    break;
                case 3: // D
                    x = 1;
                    y = 3;
                    k0 = 1;
                    break;
                case 4: // E
                    x = 4;
                    y = 4;
                    k0 = 2;
                    break;
                default:
                    break;
            }
            int v; // phase index
            for( int s = 0; s < d_nsym_frame[rm]; s++)
            {
                v_tmp.clear();
                n = s % y;
                m = floor( s/y );
                for( int i = 0; i < (d_gain_pil_pos[rm][so][s]).size(); i++)
                {
                    p = ((d_gain_pil_pos[rm][so][s])[i] - k0 - n*x) / (x * y);
                    if(rm < 4) // DRM30
                    {
                        // use formula for DRM30
                        switch(rm)
                        {
                            case 0: // A
                                v = ( 4*gain_Z_A[n][m] + p*gain_W_A[n][m] + (int) pow(p,2) * (1 + s) * gain_Q_A ) % 1024;
                                break;
                            case 1: // B
                                v = ( 4*gain_Z_B[n][m] + p*gain_W_B[n][m] + (int) pow(p,2) * (1 + s) * gain_Q_B ) % 1024;
                                break;
                            case 2: // B
                                v = ( 4*gain_Z_C[n][m] + p*gain_W_C[n][m] + (int) pow(p,2) * (1 + s) * gain_Q_C ) % 1024;
                                break;
                            case 3: // B
                                v = ( 4*gain_Z_D[n][m] + p*gain_W_D[n][m] + (int) pow(p,2) * (1 + s) * gain_Q_D ) % 1024;
                                break;
                            default:
                                break;
                        }
                        if(v<0) // wrap around
                        {
                            v = 1024+v;
                        }
                        v_tmp.push_back(v);
                    }
                    else
                    {
                        // use formula for DRM+
                        v = ( (int) pow(p,2) * gain_R_E[n][m] + p * gain_Z_E[n][m] + gain_Q_E[n][m] ) % 1024;
                        v_tmp.push_back(v);
                    }
                }
                gain_phase_index.push_back(v_tmp); // push indices for one symbol to d_gain_phase
            }

            /* calculate actual complex cell values
             cell value is sqrt(2) * exp( j*2*pi*phase_index/1024 ) */
            printf("enter value calc");
            std::vector< gr_complex > gain_cells_tmp;
            double boost = sqrt(2);
            const double pi = M_PI;
            gr_complexd j(0,1); // imaginary unit
            tmp_frame_rmso_fixed.clear();
            for(int s = 0; s < d_nsym_frame[rm]; s++)
            {
                gain_cells_tmp.clear();
                for(int n = 0; n < (d_gain_pil_pos[rm][so][s]).size(); n++)
                {
                    //std::cout << "check if " << d_gain_pil_pos[rm][so][s][n] << " equals " << d_gain_boost[rm][4*so] << " or " << d_gain_boost[rm][4*so + 1] << " or " << d_gain_boost[rm][4*so + 2] << " or " << d_gain_boost[rm][4*so + 3] << std::endl;
                    if(d_gain_pil_pos[rm][so][s][n] == d_gain_boosted[rm][4*so] || // check if cell shall be overboosted
                       d_gain_pil_pos[rm][so][s][n] == d_gain_boosted[rm][4*so + 1] ||
                       d_gain_pil_pos[rm][so][s][n] == d_gain_boosted[rm][4*so + 2] ||
                       d_gain_pil_pos[rm][so][s][n] == d_gain_boosted[rm][4*so + 3])
                    {
                        gain_cells_tmp.push_back( boost * boost *( cos(2 * pi * gain_phase_index[s][n] / 1024) + j * sin(2 * pi * gain_phase_index[s][n] / 1024) ) );
                    }
                    else // normal gain
                    {
                        gain_cells_tmp.push_back( boost * ( cos(2 * pi * gain_phase_index[s][n] / 1024) + j * sin(2 * pi * gain_phase_index[s][n] / 1024) ) );
                    }
                }
                tmp_frame_rmso_fixed.push_back(gain_cells_tmp); // push tmp vector to actual class member
            }
            tmp_frame_rm_fixed.push_back(tmp_frame_rmso_fixed);
        }
        d_gain_pil_values.push_back(tmp_frame_rm_fixed);
    }
}
