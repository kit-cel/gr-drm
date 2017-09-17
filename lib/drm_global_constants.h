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
 
#ifndef DRM_GLOBAL_CONSTANTS_H_
#define DRM_GLOBAL_CONSTANTS_H_

#define NUM_RM				5 // number of robustness modes ('A'-'E')
#define RM_A                0
#define RM_B                1
#define RM_C                2
#define RM_D                3
#define RM_E                4

#define NUM_SO				6 // number of available bandwidths (-> spectrum occupancy 0-5)
#define SO_0                0
#define SO_1                1
#define SO_2                2
#define SO_3                3
#define SO_4                4
#define SO_5                5

#define FS_SOUNDCARD		48000 // sound card sample rate (48 kHz)

#define N_FAC_DRM			65 // number of FAC QAM cells in DRM
#define N_FAC_DRMPLUS		244 // number of FAC QAM cells in DRM+
#define L_FAC_DRM			72 // number of FAC bits in DRM
#define L_FAC_DRMPLUS		116 // number of FAC bits in DRM+

#define INTL_DEPTH_DRM		5 // interleaving depth for RM A-D for long interleaving (multiple of transmission frames)
#define INTL_DEPTH_DRMPLUS	6 // ... same for DRM+ (RM E)

#define NUM_FREQ_PILOTS		3 // only for RM A-D, no freq pilots in DRM+
#define NUM_AFS_PILOTS		54 // only for RM E

#define RMA_NUM_TIME_PIL	21
#define RMB_NUM_TIME_PIL	19
#define RMC_NUM_TIME_PIL	19
#define RMD_NUM_TIME_PIL	21
#define RME_NUM_TIME_PIL	21

#define LEN_PP				 6 // lenght of puncturing patterns
#define DENOM_MOTHER_CODE	 6 // denominator for mother code rate (1/6) used for channel encoding
#define N_TAILBITS			36 // number of tailbits (of mother code) after channel encoding

#endif /* DRM_GLOBAL_CONSTANTS_H_ */
