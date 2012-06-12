/*
 * drm_init_params.h
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#ifndef DRM_INIT_PARAMS_H_
#define DRM_INIT_PARAMS_H_

#include <iostream>

struct config
{
	unsigned short RM;
	unsigned short SO;
	bool UEP;
	bool text;
	unsigned short mapping;
};

class global_params
{
public:
	unsigned short RM;
	unsigned short SO;

	virtual void init(unsigned short rm, unsigned short so) = 0;

	global_params(){};
	virtual ~global_params(){};
};

class ofdm_params : public global_params
{
public:
	unsigned int nfft;
	unsigned int nguard;

	ofdm_params(){};
	virtual ~ofdm_params(){};

	void init(unsigned short rm, unsigned short so);
};

class control_chan_params : public global_params
{
public:
	unsigned int L;
	unsigned int N;

	float coderate;

	control_chan_params(){};
	virtual ~control_chan_params(){};
};

class sdc_params : public control_chan_params
{
public:
	sdc_params(){};
	virtual ~sdc_params(){};

	void init(unsigned short rm, unsigned short so);
};

class fac_params : public control_chan_params
{
public:
	fac_params(){};
	virtual ~fac_params(){};

	void init(unsigned short rm, unsigned short so);
};

class msc_params : public global_params
{
public:
	unsigned int L_MUX;
	unsigned int L_1;
	unsigned int L_2;
	unsigned int N_MUX;
	unsigned int N_1;
	unsigned int N_2;

	msc_params(){};
	virtual ~msc_params(){};

	void init(unsigned short rm, unsigned short so);
};


#endif /* DRM_INIT_PARAMS_H_ */
