/* This file collects global constants */

#ifndef INCLUDED_DRMRX_CONSTANTS_H
#define INCLUDED_DRMRX_CONSTANTS_H

#define FS				50000	// sampling rate in Hz, chosen to make resampling from 250 kHz efficient
#define T_O				0.4		// duration of an OFDM symbol in seconds

#define RM_A			0 // 'best' channel (DRM30)
#define RM_B			1
#define RM_C			2
#define RM_D			3 // 'worst' channel (DRM30)
#define RM_E			4 // DRM+
#define RM_NONE			5 // invalid RM 

#define SO_0			0 // 4.5 kHz
#define SO_1			1 // 5 kHz
#define SO_2			2 // 9 kHz
#define SO_3			3 // 10 kHz
#define SO_4			4 // 18 kHz
#define SO_5			5 // 20 kHz
#define SO_NONE			6 // invalid SO

#endif
