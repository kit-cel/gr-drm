/* -*- c++ -*- */
/* 
 * Copyright 2012 Communications Engineering Lab (CEL) / KIT (Karlsruhe Institute of Technology) 
 * Author: Felix Wunsch
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
 
#include <drm/tables.h>

namespace gr {
  namespace drm {
    /* Puncturing patterns */
    // '1's denote bits to be kept, '0's bits to be dropped
    const unsigned char tables::d_PP_1_6[6] = {1,1,1,1,1,1};
    const unsigned char tables::d_PP_1_4[6] = {1,1,1,1,0,0};
    const unsigned char tables::d_PP_3_10[18] = {1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0};
    const unsigned char tables::d_PP_1_3[6] = {1,1,1,0,0,0};
    const unsigned char tables::d_PP_4_11[24] = {1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0};
    const unsigned char tables::d_PP_2_5[12] = {1,1,1,0,0,0,1,1,0,0,0,0};
    const unsigned char tables::d_PP_1_2[6] = {1,1,0,0,0,0};
    const unsigned char tables::d_PP_4_7[24] = {1,1,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0};
    const unsigned char tables::d_PP_3_5[18] = {1,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0};
    const unsigned char tables::d_PP_2_3[12] = {1,1,0,0,0,0,1,0,0,0,0,0};
    const unsigned char tables::d_PP_8_11[48] = {1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0};
    const unsigned char tables::d_PP_3_4[18] = {1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0};
    const unsigned char tables::d_PP_4_5[24] = {1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0};
    const unsigned char tables::d_PP_7_8[42] = {1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0};
    const unsigned char tables::d_PP_8_9[48] = {1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0};

    // puncturing patterns for the tailbits. the row index equals the index r_p (see p. 123 in the DRM standard)
    const unsigned char tables::d_PP_tail[13][N_TAILBITS] = {
            {1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
            {1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
            {1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
            {1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
            {1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0},
            {1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0},
            {1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
            {1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
            {1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
            {1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
            {1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0},
            {1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}; // this row is for handling unused code rates / puncturing patterns

    /* MSC */

    /* QAM cell related tables */
    const unsigned int tables::d_MSC_N_MUX[NUM_RM][NUM_SO] = { // row: RM, col: SO
                    {1259, 1422, 2632, 2959, 5464, 6118},
                    {966, 1110, 2051, 2337, 4249, 4774},
                    {0, 0, 0, 1844, 0, 3867},
                    {0, 0, 0, 1226, 0, 2606},
                    {7460, 0, 0, 0, 0}};

    const unsigned int tables::d_MSC_N_L[NUM_RM][NUM_SO] = {
                    {1,2,1,0,2,0},
                    {2,0,0,2,0,1},
                    {0,0,0,0,0,2},
                    {0,0,0,1,0,1},
                    {2,0,0,0,0,0}};

    /* Code rates
     * SM: {R_0_enum, R_0_denom, R_1_enum, ..., R_Ylcm} */
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

    /* SDC */

    /* Length of SDC data field in bytes for SDC mode 0 and 1 */
    const unsigned int tables::d_SDC_datafield_0[5][6] = {
                            { 37, 43, 85, 97,184,207},
                            { 28, 33, 66, 76,143,161},
                            {  0,  0,  0, 68,  0,147},
                            {  0,  0,  0, 33,  0, 78},
                            {113,  0,  0,  0,  0,  0}};

    const unsigned int tables::d_SDC_datafield_1[5][6] = {
                            { 17, 20, 41, 47, 91,102},
                            { 13, 15, 32, 37, 70, 79},
                            {  0,  0,  0, 32,  0, 72},
                            {  0,  0,  0, 15,  0, 38},
                            { 55,  0,  0,  0,  0,  0}};
  }
}
