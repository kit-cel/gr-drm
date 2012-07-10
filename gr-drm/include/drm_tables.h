/*
 * drm_tables.h
 *
 *  Created on: 13.06.2012
 *      Author: felixwunsch
 */

#ifndef DRM_TABLES_H_
#define DRM_TABLES_H_

#include "drm_global_constants.h"

class tables
{
	// TODO: find a way to access the tables flexibly, then make them private.
	public:

	/****************************************************************
	 ******************* (Cell) mapping *****************************
	 ***************************************************************/

	/* FAC positions. The two numbers denote {symbol no, carrier no} */
	static const int d_FAC_A[N_FAC_DRM][2];
	static const int d_FAC_B[N_FAC_DRM][2];
	static const int d_FAC_C[N_FAC_DRM][2];
	static const int d_FAC_D[N_FAC_DRM][2];
	//static const int d_FAC_E[N_FAC_DRMPLUS][2]; // TODO: define values

	/* Frequency pilot positions and phases (normalized to 1024)
	 * The two colums denote {carrier index, phase index} */
	static const int d_freq_A[NUM_FREQ_PILOTS][2];
	static const int d_freq_B[NUM_FREQ_PILOTS][2];
	static const int d_freq_C[NUM_FREQ_PILOTS][2];
	static const int d_freq_D[NUM_FREQ_PILOTS][2]; // TODO: special care has to be taken to ensure continuous tones in RM D.
	// there are no frequency pilots in RM E!

	/* Time reference positions and phases (normalized) */
	static const int d_time_A[RMA_NUM_TIME_PIL][2];
	static const int d_time_B[RMB_NUM_TIME_PIL][2];
	static const int d_time_C[RMC_NUM_TIME_PIL][2];
	static const int d_time_D[RMD_NUM_TIME_PIL][2];
	//static const int d_time_E[RME_NUM_TIME_PIL][2]; // TODO: define values

	/* Gain reference cells */
	//TODO: add gain reference cells

	/* AFS reference cells */
	//TODO: add AFS reference cells

	/* QAM mapping */
	static const float d_QAM64SM[8][2];
	static const float d_QAM64HMsym[8][2];
	static const float d_QAM64HMmix[8][2];
	static const float d_QAM16[4][2];
	static const float d_QAM4[2][2];

	/****************************************************************
	 *************** Puncturing patterns ****************************
	 ****************************************************************/

	static const unsigned char d_PP1[LEN_PP];
	static const unsigned char d_PP2[LEN_PP];
	static const unsigned char d_PP3[LEN_PP];
	static const unsigned char d_PP4[LEN_PP];
	static const unsigned char d_PP5[LEN_PP];
	static const unsigned char d_PP6[LEN_PP];
	
	/****************************************************************
	 ********* OFDM/MSC/SDC/FAC parameters **************************
	 ****************************************************************/

	/* MSC */
	static const unsigned int d_MSC_N_MUX[5][6]; // row: RM, col: SO
	static const unsigned short d_MSC_CR_4[4][2];
	static const unsigned short d_MSC_16_ABCD[2][5];
	static const unsigned short d_MSC_16_E[4][5];
	static const unsigned short d_MSC_64_SM[4][7];
	static const unsigned short d_MSC_64_HMsym_SPP[4][5];
	static const unsigned short d_MSC_64_HMsym_VSPP[4][2];
	static const unsigned short d_MSC_64_HMmix_SPP[4][11];
	
	/* SDC */
	static const unsigned int d_SDC_datafield_0[5][6]; // row: RM, col: SO
	static const unsigned int d_SDC_datafield_1[5][6];

	tables(){};
	~tables(){};
};

#endif /* DRM_TABLES_H_ */
