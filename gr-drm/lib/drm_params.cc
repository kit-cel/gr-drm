/*
 * drm_init_params.cpp
 *
 *  Created on: 11.06.2012
 *      Author: felixwunsch
 */

#include "drm_params.h"
#include <cmath>

// TODO: maybe divide this up into drm_params_ofdm/msc/fac/sdc.cpp

/* OFDM params implementation */
ofdm_params::ofdm_params()
{
	d_nfft = 0;
	d_n_cp = 0;
	d_cp_ratio_enum = 0;
	d_cp_ratio_denom = 0;
	d_K_min = 0;
	d_K_max = 0;
	d_n_unused = 0;
	d_N_S = 0;
	d_M_TF = 0;
	d_fs_soundcard = 0;
}

void
ofdm_params::init(config* cfg)
{
	// TODO: define values for RM E (DRM+)

	/* define tables to take the values for the current configuration from */
	unsigned int tab_nfft[NUM_RM] = {1152, 1024, 704, 448, 0}; //  taken from Dream FIXME: missing value for E
	unsigned int tab_N_S[NUM_RM] = {15, 15, 20, 24, 40}; // see DRM standard Table 82
	unsigned int tab_cp_ratio[NUM_RM][2] = {{1, 9}, {1, 4}, {4, 11}, {11, 14}, {1, 9}}; // denominator
	unsigned short tab_M_TF[NUM_RM] = {3, 3, 3, 3, 4}; // see DRM standard p. 137
	unsigned int tab_K_range[2*NUM_RM][NUM_SO] = {{2, 2, -102, -114, -98, -110}, // see DRM standard Table 84
												  {102, 114, 102, 114, 314, 350},
												  {1, 1, -91, -103, -87, -99},
												  {91, 103, 91, 103, 279, 311},
												  {0, 0, 0, -69, 0, -67},
												  {0, 0, 0, 69, 0, 213},
												  {0, 0, 0, -44, 0, -43},
												  {0, 0, 0, 44, 0, 135},
												  {-106, 0, 0, 0, 0, 0},
												  {106, 0, 0, 0, 0, 0}};
	unsigned short tab_n_unused[NUM_RM] = {3, 1, 1, 1, 0}; // see DRM standard Table 85

	d_nfft = tab_nfft[cfg->RM()];
	d_cp_ratio_enum = tab_cp_ratio[cfg->RM()][0];
	d_cp_ratio_denom = tab_cp_ratio[cfg->RM()][1];
	d_n_cp = d_nfft * d_cp_ratio_enum / d_cp_ratio_denom;
	d_N_S = tab_N_S[cfg->RM()];
	d_M_TF = tab_M_TF[cfg->RM()];
	d_K_min = tab_K_range[2*cfg->RM()][cfg->SO()];
	d_K_max = tab_K_range[2*cfg->RM() + 1][cfg->SO()];
	d_n_unused = tab_n_unused[cfg->RM()];
	d_fs_soundcard = FS_SOUNDCARD;
}

unsigned int
ofdm_params::nfft()
{
	return d_nfft;
}

unsigned short
ofdm_params::cp_ratio_enum()
{
	return d_cp_ratio_enum;
}

unsigned short
ofdm_params::cp_ratio_denom()
{
	return d_cp_ratio_denom;
}

unsigned int
ofdm_params::n_cp()
{
	return d_n_cp;
}

unsigned int
ofdm_params::N_S()
{
	return d_N_S;
}

unsigned short
ofdm_params::M_TF()
{
	return d_M_TF;
}

int
ofdm_params::K_min()
{
	return d_K_min;
}

int
ofdm_params::K_max()
{
	return d_K_max;
}

unsigned int
ofdm_params::n_unused()
{
	return d_n_unused;
}

unsigned int
ofdm_params::fs_soundcard()
{
	return d_fs_soundcard;
}

/* MSC channel implementation */
msc_params::msc_params()
{
	d_L_MUX = 0;
	d_L_1 = 0;
	d_L_2 = 0;
	d_L_VSPP = 0;
	d_N_MUX = 0;
	d_N_1 = 0;
	d_N_2 = 0;
	d_MSC_prot_1 = 0;
	d_MSC_prot_2 = 0;
	d_R_0_enum_1 = 0;
	d_R_0_denom_1 = 0;
	d_R_1_enum_1 = 0;
	d_R_1_denom_1 = 0;
	d_R_2_enum_1 = 0;
	d_R_2_denom_1 = 0;
	d_R_Ylcm_1 = 0;
	d_R_0_enum_2 = 0;
	d_R_0_denom_2 = 0;
	d_R_1_enum_2 = 0;
	d_R_1_denom_2 = 0;
	d_R_2_enum_2 = 0;
	d_R_2_denom_2 = 0;
	d_R_Ylcm_2 = 0;
	d_n_levels_mlc = 0;
}

void
msc_params::init(config* cfg)
{
	/* Assign code rates */

	if(cfg->msc_mapping() > 2) // Hierarchical mapping
	{
		std::cout << "Hierarchical mapping is not yet implemented...\n";
	}
	else // Symmetrical mapping
	{
		/* set code rates according to protection level(s) */
		if(cfg->RM() < 4) // RM A-D
		{
			switch(cfg->msc_mapping())
			{
			case 1: // 16-QAM
				if(cfg->UEP())
				{
					d_R_0_enum_1 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_1()][0];
					d_R_0_denom_1 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_1()][1];
					d_R_1_enum_1 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_1()][2];
					d_R_1_denom_1 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_1()][3];
					d_R_Ylcm_1 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_1()][4];
				}

				d_R_0_enum_2 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_2()][0];
				d_R_0_denom_2 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_2()][1];
				d_R_1_enum_2 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_2()][2];
				d_R_1_denom_2 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_2()][3];
				d_R_Ylcm_2 = cfg->ptables()->d_MSC_16_ABCD[cfg->msc_prot_level_2()][4];
				break;

			case 2: // 64-QAM SM
				if(cfg->UEP())
				{
					d_R_0_enum_1 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_1()][0];
					d_R_0_denom_1 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_1()][1];
					d_R_1_enum_1 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_1()][2];
					d_R_1_denom_1 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_1()][3];
					d_R_2_enum_1 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_1()][4];
					d_R_2_denom_1 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_1()][5];
					d_R_Ylcm_1 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_1()][6];
				}

				d_R_0_enum_2 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_2()][0];
				d_R_0_denom_2 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_2()][1];
				d_R_1_enum_2 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_2()][2];
				d_R_1_denom_2 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_2()][3];
				d_R_2_enum_2 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_2()][4];
				d_R_2_denom_2 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_2()][5];
				d_R_Ylcm_2 = cfg->ptables()->d_MSC_64_SM[cfg->msc_prot_level_2()][6];
				break;

			default:
				std::cout << "Undefined state (RM A-D)! Check configuration!\n";
				break;
			}
		}
		else // RM E
		{
			switch(cfg->msc_mapping())
			{
			case 0: // 4-QAM
				d_R_0_enum_2 = cfg->ptables()->d_MSC_CR_4[cfg->msc_prot_level_2()][0];
				d_R_0_denom_2 = cfg->ptables()->d_MSC_CR_4[cfg->msc_prot_level_2()][1];
				break;

			case 1: // 16-QAM
				if(cfg->UEP())
				{
					d_R_0_enum_1 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_1()][0];
					d_R_0_denom_1 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_1()][1];
					d_R_1_enum_1 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_1()][2];
					d_R_1_denom_1 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_1()][3];
					d_R_Ylcm_1 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_1()][4];
				}

				d_R_0_enum_2 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_2()][0];
				d_R_0_denom_2 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_2()][1];
				d_R_1_enum_2 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_2()][2];
				d_R_1_denom_2 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_2()][3];
				d_R_Ylcm_2 = cfg->ptables()->d_MSC_16_E[cfg->msc_prot_level_2()][4];
				break;

			default:
				std::cout << "Undefined state (RM E)! Check configuration!\n";
				break;
			}
		}
	}

	/* calculate and define code-rate-dependant variables */
	/* NOTE: There is no need to make a difference here between UEP and EEP as most of the values automatically
	 * 'default' to EEP if the number of bytes in the higher protected part
	 * is defined as zero. */

	if(cfg->msc_mapping() < 3) // Symmetrical mapping (SM)
	{
		calc_vars_SM(cfg);
		// TODO: calc rest of the variables
	}
	else if(cfg->msc_mapping() == 3) // Hierarchical symmetrical mapping (HMsym)
	{
		d_n_levels_mlc = 3;
		std::cout << "not yet implemented...\n"; // TODO: implement code rate assignment for hierarchical mapping
	}
	else if(cfg->msc_mapping() == 4) // HMmix
	{
		d_n_levels_mlc = 6;
		std::cout << "not yet implemented...\n";
	}
	else
	{
		std::cout << "Invalid MSC mapping number!\n";
	}
}

void
msc_params::calc_vars_SM(config* cfg)
{
	/* define P_max and sum */
	unsigned short P_max = 0; // number of levels in the MLC encoder, depends on mapping
	float sum = 0; // sum of code rates (higher protected part) as in the formula on p. 112
	if(cfg->msc_mapping() == 0)
	{
		P_max = 1; // 4-QAM
		if(cfg->UEP())
		{
			sum = d_R_0_enum_1 / float(d_R_0_denom_1);
		}
	}
	else if(cfg->msc_mapping() == 1)
	{
		P_max = 2; // 16-QAM
		if(cfg->UEP())
		{
			sum = d_R_0_enum_1 / float(d_R_0_denom_1) +
				  d_R_1_enum_1 / float(d_R_1_denom_1);
		}
	}
	else if(cfg->msc_mapping() > 1)
	{
		P_max = 3; // 64-QAM
		if(cfg->UEP())
		{
			sum = d_R_0_enum_1 / float(d_R_0_denom_1) +
				  d_R_1_enum_1 / float(d_R_1_denom_1) +
				  d_R_2_enum_1 / float(d_R_2_denom_1);
		}
	}
	
	/* set number of levels in the encoder */
	d_n_levels_mlc = P_max;

	/* set number of QAM cells */
	d_N_MUX = cfg->ptables()->d_MSC_N_MUX[cfg->RM()][cfg->SO()];
	d_N_1 = ceil((8 * cfg->n_bytes_A()) / (2 * d_R_Ylcm_1 * sum)) * d_R_Ylcm_1;
	d_N_2 = d_N_MUX - d_N_1;

	if((d_N_2 < 20) || (d_N_1 > d_N_MUX - 20))
	{
		std::cout << "N_2 or N_1 out of bounds!\n";
	}

	/* set number of bits */
	d_L_VSPP = 0; // no hierarchical mapping is used
	d_L_1 = 2 * d_N_1 * sum;
	switch(P_max)
	{
	case 1:
		d_L_2 = d_R_0_enum_2 * floor((2*d_N_2 - 12) / d_R_0_denom_2);
		break;
	case 2:
		d_L_2 = d_R_0_enum_2 * floor((2*d_N_2 - 12) / d_R_0_denom_2) +
				d_R_1_enum_2 * floor((2*d_N_2 - 12) / d_R_1_denom_2);
		break;
	case 3:
		d_L_2 = d_R_0_enum_2 * floor((2*d_N_2 - 12) / d_R_0_denom_2) +
				d_R_1_enum_2 * floor((2*d_N_2 - 12) / d_R_1_denom_2) +
				d_R_2_enum_2 * floor((2*d_N_2 - 12) / d_R_2_denom_2);
		break;
	default:
		std::cout << "P_max invalid!\n";
		break;
	}

	d_L_MUX = d_L_1 + d_L_2;
	
	/* set indexes for partitioning */
	if(P_max >= 1) // 4-QAM
	{
		if(cfg->UEP())
		{
			int M_01 = 2 * d_N_1 * d_R_0_enum_1 / d_R_0_denom_1;
			d_M.push_back(M_01);
		}
		int M_02 = d_R_0_enum_2 * std::floor( (2*d_N_2 - 12) / d_R_0_denom_2 );
		d_M.push_back(M_02);
	}
	if(P_max >= 2) // 16-QAM
	{
		if(cfg->UEP())
		{
			int M_11 = 2 * d_N_1 * d_R_1_enum_1 / d_R_1_denom_1;
			d_M.push_back(M_11);
		}
		int M_12 = d_R_1_enum_2 * std::floor( (2*d_N_2 - 12) / d_R_1_denom_2 );
		d_M.push_back(M_12);
	}
	if(P_max >= 3) // 64-QAM
	{	
		if(cfg->UEP())
		{
			int M_21 = 2 * d_N_1 * d_R_2_enum_1 / d_R_2_denom_1;
			d_M.push_back(M_21);
		}
		int M_22 = d_R_2_enum_2 * std::floor( (2*d_N_2 - 12) / d_R_2_denom_2 );
		d_M.push_back(M_22);
	}	
}

unsigned int
msc_params::L_MUX()
{
	return d_L_MUX;
}

unsigned int
msc_params::L_1()
{
	return d_L_1;
}

unsigned int
msc_params::L_2()
{
	return d_L_2;
}

unsigned int
msc_params::L_VSPP()
{
	return d_L_VSPP;
}

unsigned int
msc_params::N_MUX()
{
	return d_N_MUX;
}

unsigned int
msc_params::N_1()
{
	return d_N_1;
}

unsigned int
msc_params::N_2()
{
	return d_N_2;
}

unsigned short
msc_params::MSC_prot_1()
{
	return d_MSC_prot_1;
}

unsigned short
msc_params::MSC_prot_2()
{
	return d_MSC_prot_2;
}

unsigned short
msc_params::R_0_enum_1()
{
	return d_R_0_enum_1;
}

unsigned short
msc_params::R_0_denom_1()
{
	return d_R_0_denom_1;
}

unsigned short
msc_params::R_1_enum_1()
{
	return d_R_1_enum_1;
}

unsigned short
msc_params::R_1_denom_1()
{
	return d_R_1_denom_1;
}

unsigned short
msc_params::R_2_enum_1()
{
	return d_R_2_enum_1;
}

unsigned short
msc_params::R_2_denom_1()
{
	return d_R_2_denom_1;
}

unsigned short
msc_params::R_Ylcm_1()
{
	return d_R_Ylcm_1;
}

unsigned short
msc_params::R_0_enum_2()
{
	return d_R_0_enum_2;
}

unsigned short
msc_params::R_0_denom_2()
{
	return d_R_0_denom_2;
}

unsigned short
msc_params::R_1_enum_2()
{
	return d_R_1_enum_2;
}

unsigned short
msc_params::R_1_denom_2()
{
	return d_R_1_denom_2;
}

unsigned short
msc_params::R_2_enum_2()
{
	return d_R_2_enum_2;
}

unsigned short
msc_params::R_2_denom_2()
{
	return d_R_2_denom_2;
}

unsigned short
msc_params::R_Ylcm_2()
{
	return d_R_Ylcm_2;
}

std::vector< int >
msc_params::M()
{
	return d_M;
}

unsigned short
msc_params::n_levels_mlc()
{
	return d_n_levels_mlc;
}

/* Control channel implementation */
control_chan_params::control_chan_params()
{
	d_L = 0;
	d_N = 0;
	d_R_0 = 0;
	d_R_0_enum = 0;
	d_R_0_denom = 0;
}

unsigned int
control_chan_params::L()
{
	return d_L;
}

unsigned int
control_chan_params::N()
{
	return d_N;
}

float
control_chan_params::R_0()
{
	return d_R_0;
}

unsigned short
control_chan_params::R_0_enum()
{
	return d_R_0_enum;
}

unsigned short
control_chan_params::R_0_denom()
{
	return d_R_0_denom;
}

/* SDC channel implementation */
sdc_params::sdc_params()
{
	d_R_1 = 0;
	d_R_1_enum = 0;
	d_R_1_denom = 0;
}

void
sdc_params::init(config* cfg)
{
	// see DRM standard 7.2.1.3
	unsigned int tab_N_SDC[5][6] = {{167, 190, 359, 405, 754, 846},
								    {130, 150, 282, 322, 588, 662},
									{  0,   0,   0, 288,   0, 607},
									{  0,   0,   0, 152,   0, 332},
									{936,   0,   0,   0,   0,   0}};

	d_N = tab_N_SDC[cfg->RM()][cfg->SO()];

	//see DRM standard Annex J
	unsigned int tab_L_SDC[2*NUM_RM][NUM_SO] = {{321, 366, 705, 798, 1494, 1680},
												{161, 184, 353, 399, 748, 840},
												{246, 288, 552, 630, 1164, 1311},
												{124, 144, 276, 316, 582, 656},
												{0, 0, 0, 584, 0, 1200},
											    {0, 0, 0, 282, 0, 601},
											    {0, 0, 0, 291, 0, 651},
											    {0, 0, 0, 146, 0, 326},
											    {930, 0, 0, 0, 0, 0},
											    {465, 0, 0, 0, 0, 0}};


	/* set code rates */
	int P_max = 0;
	if(cfg->RM() < 4) // RM A-D
	{
		d_L = tab_L_SDC[2*cfg->RM() + cfg->sdc_mapping()][cfg->SO()];		
		switch(cfg->sdc_mapping()) // FIXME: SDC prot level index should be the same as in the standard
		{
		case 0: // 16-QAM, R_all = 0.5
			P_max = 2;
			d_n_bytes_datafield = cfg->ptables()->d_SDC_datafield_0[cfg->RM()][cfg->SO()];
			d_R_0 = float(1/2);
			d_R_0_enum = 1;
			d_R_0_denom = 3;
			d_R_1 = 2/3;
			d_R_1_enum = 2;
			d_R_1_denom = 3;
			break;
		case 1: // 4-QAM
			P_max = 1;
			d_n_bytes_datafield = cfg->ptables()->d_SDC_datafield_1[cfg->RM()][cfg->SO()];
			d_R_0_enum = 1;
			d_R_0_denom = 2;
			d_R_0 = d_R_0_enum/float(d_R_0_denom); // typecast necessary to get a rational value
			break;
		default: // error checking is done before
			break;

		}
	}
	else // RM E (always 4-QAM)
	{
		d_L = tab_L_SDC[2*cfg->RM() + cfg->sdc_prot_level()][cfg->SO()];
		P_max = 1;
		switch(cfg->sdc_prot_level())
		{
		case 0:
			d_n_bytes_datafield = cfg->ptables()->d_SDC_datafield_0[cfg->RM()][cfg->SO()];
			d_R_0 = 1/2;
			d_R_0_enum = 1;
			d_R_0_denom = 2;
			break;
		case 1:
			d_n_bytes_datafield = cfg->ptables()->d_SDC_datafield_1[cfg->RM()][cfg->SO()];
			d_R_0 = 1/4;
			d_R_0_enum = 1;
			d_R_0_denom = 4;
			break;
		default:
			break;
		}
	}
	
	/* set indexes for partitioning and number of levels in the coding process */
	
	if(P_max >= 1) // 4-QAM
	{
		d_n_levels_mlc = 1;
		int M_02 = d_R_0_enum * std::floor( (2*d_N - 12) / d_R_0_denom );
		d_M.push_back(M_02);
	}
	if(P_max >= 2) // 16-QAM
	{
		d_n_levels_mlc = 2;
		int M_12 = d_R_1_enum * std::floor( (2*d_N - 12) / d_R_1_denom );
		d_M.push_back(M_12);
	}
}

float
sdc_params::R_1()
{
	return d_R_1;
}

unsigned short
sdc_params::R_1_enum()
{
	return d_R_1_enum;
}

unsigned short
sdc_params::R_1_denom()
{
	return d_R_1_denom;
}

std::vector< int >
sdc_params::M()
{
	return d_M;
}

unsigned short
sdc_params::n_levels_mlc()
{
	return d_n_levels_mlc;
}

unsigned int
sdc_params::n_bytes_datafield()
{
	return d_n_bytes_datafield;
}

/* FAC channel implementation */
void
fac_params::init(config* cfg)
{
	if(cfg->RM() == 4) // see DRM standard 7.2.1.2 & Table 74/75
	{
		// RM E
		d_N = N_FAC_DRMPLUS;
		d_L = L_FAC_DRMPLUS;
		d_R_0_enum = 1;
		d_R_0_denom = 4;
		d_R_0 = d_R_0_enum/float(d_R_0_denom);
	}
	else
	{
		// RM A-D
		d_N = N_FAC_DRM;
		d_L = L_FAC_DRM;
		d_R_0_enum = 3;
		d_R_0_denom = 5;
		d_R_0 = d_R_0_enum/float(d_R_0_denom);
	}
}
