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

    /************ OFDM parameters **********************/

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
            fprintf(stderr,"drmrx_params: ERROR! invalid RM?\n");
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
                fprintf(stderr,"drmrx_params: ERROR! invalid RM?\n");
                break;
        }
        d_time_pil_phase.push_back(tmp_phase);
        d_time_pil_pos.push_back(tmp_pos);
    }

    /* gain reference cells */
    // edge pilots' power gets boosted by a factor of 2

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
                        gr_complexd tmp_double( boost * boost *( cos(2 * pi * gain_phase_index[s][n] / 1024) + j * sin(2 * pi * gain_phase_index[s][n] / 1024) ) );
                        gr_complex tmp_float(float(tmp_double.real()), float(tmp_double.imag()));
                        gain_cells_tmp.push_back(tmp_float);
                    }
                    else // normal gain
                    {
                        gr_complexd tmp_double( boost * ( cos(2 * pi * gain_phase_index[s][n] / 1024) + j * sin(2 * pi * gain_phase_index[s][n] / 1024) ) );
                        gr_complex tmp_float(float(tmp_double.real()), float(tmp_double.imag()));
                        gain_cells_tmp.push_back(tmp_float);
                    }
                }
                tmp_frame_rmso_fixed.push_back(gain_cells_tmp); // push tmp vector to actual class member
            }
            tmp_frame_rm_fixed.push_back(tmp_frame_rmso_fixed);
        }
        d_gain_pil_values.push_back(tmp_frame_rm_fixed);
    }

    /* FAC positions. The two numbers denote {symbol no, carrier no} */
    const int FAC_A[65][2] = {
            {2, 26}, {2, 46}, {2, 66}, {2, 86},
            {3, 10}, {3, 30}, {3, 50}, {3, 70}, {3, 90},
            {4, 14}, {4, 22}, {4, 34}, {4, 62}, {4, 74}, {4, 94},
            {5, 26}, {5, 38}, {5, 58}, {5, 66}, {5, 78},
            {6, 22}, {6, 30}, {6, 42}, {6, 62}, {6, 70}, {6, 82},
            {7, 26}, {7, 34}, {7, 46}, {7, 66}, {7, 74}, {7, 86},
            {8, 10}, {8, 30}, {8, 38}, {8, 50}, {8, 58}, {8, 70}, {8, 78}, {8, 90},
            {9, 14}, {9, 22}, {9, 34}, {9, 42}, {9, 62}, {9, 74}, {9, 82}, {9, 94},
            {10, 26}, {10, 38}, {10, 46}, {10, 66}, {10, 86},
            {11, 10}, {11, 30}, {11, 50}, {11, 70}, {11, 90},
            {12, 14}, {12, 34}, {12, 74}, {12, 94},
            {13, 38}, {13, 58}, {13, 78}};

    const int FAC_B[65][2] = {
            {2, 13}, {2, 25}, {2, 43}, {2, 55}, {2, 67},
            {3, 15}, {3, 27}, {3, 45}, {3, 57}, {3, 69},
            {4, 17}, {4, 29}, {4, 47}, {4, 59}, {4, 71},
            {5, 19}, {5, 31}, {5, 49}, {5, 61}, {5, 73},
            {6, 9}, {6, 21}, {6, 33}, {6, 51}, {6, 63}, {6, 75},
            {7, 11}, {7, 23}, {7, 35}, {7, 53}, {7, 65}, {7, 77},
            {8, 13}, {8, 25}, {8, 37}, {8, 55}, {8, 67}, {8, 79},
            {9, 15}, {9, 27}, {9, 39}, {9, 57}, {9, 69}, {9, 81},
            {10, 17}, {10, 29}, {10, 41}, {10, 59}, {10, 71}, {10, 83},
            {11, 19}, {11, 31}, {11, 43}, {11, 61}, {11, 73},
            {12, 21}, {12, 33}, {12, 45}, {12, 63}, {12, 75},
            {13, 23}, {13, 35}, {13, 47}, {13, 65}, {13, 77},};

    const int FAC_C[65][2] = {
            {3, 9}, {3, 21}, {3, 45}, {3, 57},
            {4, 23}, {4, 35}, {4, 47},
            {5, 13}, {5, 25}, {5, 37}, {5, 49},
            {6, 15}, {6, 27}, {6, 39}, {6, 51},
            {7, 5}, {7, 17}, {7, 29}, {7, 41}, {7, 53},
            {8, 7}, {8, 19}, {8, 31}, {8, 43}, {8, 55},
            {9, 9}, {9, 21}, {9, 45}, {9, 57},
            {10, 23}, {10, 35}, {10, 47},
            {11, 13}, {11, 25}, {11, 37}, {11, 49},
            {12, 15}, {12, 27}, {12, 39}, {12, 51},
            {13, 5}, {13, 17}, {13, 29}, {13, 41}, {13, 53},
            {14, 7}, {14, 19}, {14, 31}, {14, 43}, {14, 55},
            {15, 9}, {15, 21}, {15, 45}, {15, 57},
            {16, 23}, {16, 35}, {16, 47},
            {17, 13}, {17, 25}, {17, 37}, {17, 49},
            {18, 15}, {18, 27}, {18, 39}, {18, 51}};

    const int FAC_D[65][2] = {
            {3, 9}, {3, 18}, {3, 27},
            {4, 10}, {4, 19},
            {5, 11}, {5, 20}, {5, 29},
            {6, 12}, {6, 30},
            {7, 13}, {7, 22}, {7, 31},
            {8, 5}, {8, 14}, {8, 23}, {8, 32},
            {9, 6}, {9, 15}, {9, 24}, {9, 33},
            {10, 16}, {10, 25}, {10, 34},
            {11, 8}, {11, 17}, {11, 26}, {11, 35},
            {12, 9}, {12, 18}, {12, 27}, {12, 36},
            {13, 10}, {13, 19}, {13, 37},
            {14, 11}, {14, 20}, {14, 29},
            {15, 12}, {15, 30},
            {16, 13}, {16, 22}, {16, 31},
            {17, 5}, {17, 14}, {17, 23}, {17, 32},
            {18, 6}, {18, 15}, {18, 24}, {18, 33},
            {19, 16}, {19, 25}, {19, 34},
            {20, 8}, {20, 17}, {20, 26}, {20, 35},
            {21, 9}, {21, 18}, {21, 27}, {21, 36},
            {22, 10}, {22, 19}, {22, 37}};

    /*const int FAC_E[244][2] = {
            {5, -78}, {5, -62}, {5, -46}, {5, -30}, {5, -14}, {5, 2}, {5, 18}, {5, 34}, {5, 50}, {5, 66}, {5, 82},
            {6, -90}, {6, -74}, {6, -58}, {6, -42}, {6, -26}, {6, -10}, {6, 6}, {6, 22}, {6, 38}, {6, 54}, {6, 70}, {6, 86},
            {7, -86}, {7, -70}, {7, -54}, {7, -38}, {7, -22}, {7, -6}, {7, 10}, {7, 26}, {7, 42}, {7, 58}, {7, 74}, {7, 90},
            {8, -82}, {8, -66}, {8, -50}, {8, -34}, {8, -18}, {8, -2}, {8, 14}, {8, 30}, {8, 46}, {8, 62}, {8, 78},
            {9, -78}, {9, -62}, {9, -46}, {9, -30}, {9, -14}, {9, 2}, {9, 18}, {9, 34}, {9, 50}, {9, 66}, {9, 82},
            {10, -90}, {10, -74}, {10, -58}, {10, -42}, {10, -26}, {10, -10}, {10, 6}, {10, 22}, {10, 38}, {10, 54}, {10, 70}, {10, 86},
            {11, -86}, {11, -70}, {11, -54}, {11, -38}, {11, -22}, {11, -6}, {11, 10}, {11, 26}, {11, 42}, {11, 58}, {11, 74}, {11, 90},
            {12, -82}, {12, -66}, {12, -50}, {12, -34}, {12, -18}, {12, -2}, {12, 14}, {12, 30}, {12, 46}, {12, 62}, {12, 78},
            {13, -78}, {13, -62}, {13, -46}, {13, -30}, {13, -14}, {13, 2}, {13, 18}, {13, 34}, {13, 50}, {13, 66}, {13, 82},
            {14, -90}, {14, -74}, {14, -58}, {14, -42}, {14, -26}, {14, -10}, {14, 6}, {14, 22}, {14, 38}, {14, 54}, {14, 70}, {14, 86},
            {15, -86}, {15, -70}, {15, -54}, {15, -38}, {15, -22}, {15, -6}, {15, 10}, {15, 26}, {15, 42}, {15, 58}, {15, 74}, {15, 90},
            {16, -82}, {16, -66}, {16, -50}, {16, -34}, {16, -18}, {16, -2}, {16, 14}, {16, 30}, {16, 46}, {16, 62}, {16, 78},
            {17, -78}, {17, -62}, {17, -46}, {17, -30}, {17, -14}, {17, 2}, {17, 18}, {17, 34}, {17, 50}, {17, 66}, {17, 82},
            {18, -90}, {18, -74}, {18, -58}, {18, -42}, {18, -26}, {18, -10}, {18, 6}, {18, 22}, {18, 38}, {18, 54}, {18, 70}, {18, 86},
            {19, -86}, {19, -70}, {19, -54}, {19, -38}, {19, -22}, {19, -6}, {19, 10}, {19, 26}, {19, 42}, {19, 58}, {19, 74}, {19, 90},
            {20, -82}, {20, -66}, {20, -50}, {20, -34}, {20, -18}, {20, -2}, {20, 14}, {20, 30}, {20, 46}, {20, 62}, {20, 78},
            {21, -78}, {21, -62}, {21, -46}, {21, -30}, {21, -14}, {21, 2}, {21, 18}, {21, 34}, {21, 50}, {21, 66}, {21, 82},
            {22, -90}, {22, -74}, {22, -58}, {22, -42}, {22, -26}, {22, -10}, {22, 6}, {22, 22}, {22, 38}, {22, 54}, {22, 70}, {22, 86},
            {23, -86}, {23, -70}, {23, -54}, {23, -38}, {23, -22}, {23, -6}, {23, 10}, {23, 26}, {23, 42}, {23, 58}, {23, 74}, {23, 90},
            {24, -82}, {24, -66}, {24, -50}, {24, -34}, {24, -18}, {24, -2}, {24, 14}, {24, 30}, {24, 46}, {24, 62}, {24, 78},
            {25, -78}, {25, -62}, {25, -46}, {25, -30}, {25, -14}, {25, 2}, {25, 18}, {25, 34}, {25, 50}, {25, 66}, {25, 82},
            {26, -90}, {26, -74}, {26, -58}};*/

    // only RM B is used here
    std::vector< std::vector<int> > tmp_fac_rmfixed;
    for(int rm = 0; rm < 4; rm++)
    {
        std::vector<int> tmp_fac_rmsymfixed;
        for(int i = 0; i< 65; i++)
        {
            switch(rm)
            {
                case 0:// RM A
                tmp_fac_rmsymfixed.assign(&FAC_A[i][0], &FAC_A[i][0]+2);
                break;
                case 1:// RM B
                tmp_fac_rmsymfixed.assign(&FAC_B[i][0], &FAC_B[i][0]+2);
                break;
                case 2:// ...
                tmp_fac_rmsymfixed.assign(&FAC_C[i][0], &FAC_C[i][0]+2);
                break;
                case 3:
                tmp_fac_rmsymfixed.assign(&FAC_D[i][0], &FAC_D[i][0]+2);
                break;
                default:
                fprintf(stderr, "invalid RM. index out of bounds!\n");
            }

            tmp_fac_rmfixed.push_back(tmp_fac_rmsymfixed);
        }
        d_fac_pos.push_back(tmp_fac_rmfixed);
        tmp_fac_rmfixed.clear();
    }
}
