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
	/* FAC positions. The two numbers denote {symbol no, carrier no} */
	int d_FAC_A[N_FAC_DRM][2];
	int d_FAC_B[N_FAC_DRM][2];
	int d_FAC_C[N_FAC_DRM][2];
	int d_FAC_D[N_FAC_DRM][2];
	int d_FAC_E[N_FAC_DRMPLUS][2]; // TODO: define values

	/* Frequency pilot positions and phases (normalized to 1024)
	 * The two colums denote {carrier index, phase index} */
	int d_freq_A[NUM_FREQ_PILOTS][2];
	int d_freq_b[NUM_FREQ_PILOTS][2];
	int d_freq_C[NUM_FREQ_PILOTS][2];
	int d_freq_D[NUM_FREQ_PILOTS][2]; // TODO: special care has to be taken to ensure continuous tones in RM D.
	// there are no frequency pilots in RM E!

	/* Time reference positions and phases (normalized) */
	int d_time_A[RMA_NUM_TIME_PIL][2];
	int d_time_B[RMB_NUM_TIME_PIL][2];
	int d_time_C[RMC_NUM_TIME_PIL][2];
	int d_time_D[RMD_NUM_TIME_PIL][2];
	int d_time_E[RME_NUM_TIME_PIL][2]; // TODO: define values

	/* Gain reference cells */

	/* AFS reference cells */

	/* QAM mapping */
	float d_QAM64SM[8][2];
	float d_QAM64HMsym[8][2];
	float d_QAM64HMmix[8][2];
	float d_QAM16[4][2];
	float d_QAM4[2][2];

public:
	/* accessor methods */

	/* init routine */
	void init();

	tables(){};
	~tables(){};

};

#endif /* DRM_TABLES_H_ */
