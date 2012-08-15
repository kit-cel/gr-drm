/*
 * drm_tables.h
 *
 *  Created on: 13.06.2012
 *      Author: felixwunsch
 */

#ifndef DRM_TABLES_H_
#define DRM_TABLES_H_

#include "drm_global_constants.h"
#include <vector>
#include <cmath>
#include <complex>

class tables
{
	public:

	/****************************************************************
	 ******************* (Cell) mapping *****************************
	 ***************************************************************/

	/* FAC positions. The two numbers denote {symbol no, carrier no} */
	static const int d_FAC_A[N_FAC_DRM][2];
	static const int d_FAC_B[N_FAC_DRM][2];
	static const int d_FAC_C[N_FAC_DRM][2];
	static const int d_FAC_D[N_FAC_DRM][2];
	static const int d_FAC_E[N_FAC_DRMPLUS][2];

	/* Frequency pilot positions and phases (normalized to 1024)
	 * The two colums denote {carrier index, phase index} */
	static const int d_freq_A[NUM_FREQ_PILOTS][2];
	static const int d_freq_B[NUM_FREQ_PILOTS][2];
	static const int d_freq_C[NUM_FREQ_PILOTS][2];
	static const int d_freq_D[NUM_FREQ_PILOTS][2];
	// there are no frequency pilots in RM E!
	
	/* AFS reference cells (only RM E) */
	static const int d_AFS[NUM_AFS_PILOTS][3];

	/* Time reference positions and phases (normalized) */
	static const int d_time_A[RMA_NUM_TIME_PIL][2];
	static const int d_time_B[RMB_NUM_TIME_PIL][2];
	static const int d_time_C[RMC_NUM_TIME_PIL][2];
	static const int d_time_D[RMD_NUM_TIME_PIL][2];
	static const int d_time_E[RME_NUM_TIME_PIL][2];

	/* Gain reference cells */
	static const int d_gain_W_A[5][3];
	static const int d_gain_Z_A[5][3];
	static const int d_gain_Q_A;
	static const int d_gain_W_B[3][5];
	static const int d_gain_Z_B[3][5];
	static const int d_gain_Q_B;
	static const int d_gain_W_C[2][10];
	static const int d_gain_Z_C[2][10];
	static const int d_gain_Q_C;
	static const int d_gain_W_D[3][8];
	static const int d_gain_Z_D[3][8];
	static const int d_gain_Q_D;
	static const int d_gain_R_E[4][10];
	static const int d_gain_Z_E[4][10];
	static const int d_gain_Q_E[4][10];
	
	std::vector< std::vector< int > > d_gain_pos;
	std::vector< std::vector< std::complex<double> > > d_gain_cells; // gain cells
	static const int d_gain_boost[NUM_RM][NUM_SO * 4]; // 4 indices per RM/SO
	void calc_gain_cell_params(unsigned short rob_mode, int so, unsigned int n_sym, int k_min, int k_max); // this function is NOT called in init as RM is defined in config and N_S in ofdm_params!

	/* QAM mapping */
	static const float d_QAM64SM[8][2];
	static const float d_QAM64HMsym[8][2];
	static const float d_QAM64HMmix[8][2];
	static const float d_QAM16[4][2];
	static const float d_QAM4[2][2];


	/****************************************************************
	 *************** Puncturing patterns ****************************
	 ****************************************************************/

	static const unsigned char d_PP_1_6[6];
	static const unsigned char d_PP_1_4[6];
	static const unsigned char d_PP_3_10[18];
	static const unsigned char d_PP_1_3[6];
	static const unsigned char d_PP_4_11[24];
	static const unsigned char d_PP_2_5[12];
	static const unsigned char d_PP_1_2[6];
	static const unsigned char d_PP_4_7[24];
	static const unsigned char d_PP_3_5[18];
	static const unsigned char d_PP_2_3[12];
	static const unsigned char d_PP_8_11[48];
	static const unsigned char d_PP_3_4[18];
	static const unsigned char d_PP_4_5[24];
	static const unsigned char d_PP_7_8[42];
	static const unsigned char d_PP_8_9[48];
	static const unsigned char d_PP_tail[13][N_TAILBITS]; // the row index equals the index r_p (see p. 123 in the DRM standard)
	
	
	/****************************************************************
	 ********* OFDM/MSC/SDC/FAC parameters **************************
	 ****************************************************************/

	/* MSC */
	static const unsigned int d_MSC_N_MUX[NUM_RM][NUM_SO]; // row: RM, col: SO
	static const unsigned int d_MSC_N_L[NUM_RM][NUM_SO]; // row: RM, col: SO
	static const unsigned short d_MSC_CR_4[4][2];
	static const unsigned short d_MSC_16_ABCD[2][5];
	static const unsigned short d_MSC_16_E[4][5];
	static const unsigned short d_MSC_64_SM[4][7];
	static const unsigned short d_MSC_64_HMsym_SPP[4][5];
	static const unsigned short d_MSC_64_HMsym_VSPP[4][2];
	static const unsigned short d_MSC_64_HMmix_SPP[4][11];
	
	/* SDC */
	static const unsigned int d_SDC_datafield_0[NUM_RM][NUM_SO]; // row: RM, col: SO
	static const unsigned int d_SDC_datafield_1[NUM_RM][NUM_SO];

	tables(){};
	~tables(){};
};

#endif /* DRM_TABLES_H_ */
