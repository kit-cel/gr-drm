/*
 * drm_global_constants.h
 *
 *  Created on: 13.06.2012
 *      Author: felixwunsch
 */

#ifndef DRM_GLOBAL_CONSTANTS_H_
#define DRM_GLOBAL_CONSTANTS_H_

#define NUM_RM				5 // number of robustness modes ('A'-'E')
#define NUM_SO				6 // number of available bandwidths (-> spectrum occupancy 0-5)
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
