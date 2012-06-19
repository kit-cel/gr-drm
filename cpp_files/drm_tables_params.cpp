/*
 * drm_tables_params.cpp
 *
 *  Created on: 19.06.2012
 *      Author: felixwunsch
 */

#include "drm_tables.h"

/* MSC */

/* QAM cell related tables */
const unsigned int tables::d_MSC_N_MUX[5][6] = { // row: RM, col: SO
		{1259, 1422, 2632, 2959, 5464, 6118},
		{966, 1110, 2051, 2337, 4249, 4774},
		{0, 0, 0, 1844, 0, 3867},
		{0, 0, 0, 1226, 0, 2606},
		{7460, 0, 0, 0, 0}};

/* Code rates */
const unsigned short tables::d_MSC_CR_4[4][2] = {{1,4},{1,3},{2,5},{1,2}};
const unsigned short tables::d_MSC_16_ABCD[2][5] = {
		{1, 3, 2, 3, 3},
		{1, 2, 3, 4, 4}};
const unsigned short tables::d_MSC_16_E[4][5] = {
		 {1, 6, 1, 2, 6},
		 {1, 4, 4, 7, 28},
		 {1, 3, 2, 3, 3},
		 {1, 2, 3, 4, 4}};
const unsigned short tables::d_MSC_64_SM[4][7] = {
		  {1, 4, 1, 2, 3, 4, 4}, // These code rates are also used for the imaginary part of HMmix (SPP)
		  {1, 3, 2, 3, 4, 5, 15},
	      {1, 2, 3, 4, 7, 8, 8},
		  {2, 3, 4, 5, 8, 9, 45}};

const unsigned short tables::d_MSC_64_HMsym_SPP[4][5] = {
		 {3, 10, 3, 5, 10}, // Also used for real part of HMmix (SPP)
		 {4, 11, 8, 11, 11},
		 {4, 7, 7, 8, 56},
		 {2, 3, 8, 9, 9}};
const unsigned short tables::d_MSC_64_HMsym_VSPP[4][2] = {{1, 2}, {4, 7}, {3, 5}, {2, 3}}; // Also used for real part of HMmix (VSPP)
const unsigned short tables::d_MSC_64_HMmix_SPP[4][11] = {
			{1, 4, 3, 10, 1, 2, 3, 5, 3, 4, 20},
			{1, 3, 4, 11, 2, 3, 8, 11, 4, 5, 165},
			{1, 2, 4, 7, 3, 4, 7, 8, 7, 8, 56},
			{2, 3, 2, 3, 4, 5, 8, 9, 8, 9, 45}};








