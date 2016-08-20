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
 
#include "drm_params.h"
#include "drm_util.h"
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
	unsigned int tab_nfft[NUM_RM] = {1152, 1024, 704, 448, 216}; //  A-D taken from Dream
	unsigned int tab_N_S[NUM_RM] = {15, 15, 20, 24, 40}; // see DRM standard Table 82
	unsigned int tab_cp_ratio[NUM_RM][2] = {{1, 9}, {1, 4}, {4, 11}, {11, 14}, {1, 9}}; // denominator
	unsigned short tab_M_TF[NUM_RM] = {3, 3, 3, 3, 4}; // see DRM standard p. 137
	int tab_K_range[2*NUM_RM][NUM_SO] = {{2, 2, -102, -114, -98, -110}, // see DRM standard Table 84
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

/* channel_params implementation */

channel_params::channel_params()
{
	d_mod_order = 0;
}

int
channel_params::calc_r_p(int N, int RY_p)
{
	if(RY_p > 0)
	{
		return (2*N - 12) - RY_p * std::floor((2 * N - 12) / RY_p);
	}
	else
	{
		return 12; //  this handles unused code rates -> punct_pat_tail_x gets set to zero
	}
}

void
channel_params::set_punct_pat( std::vector< unsigned char >* pp, int e, int d, tables* t )
{
	// simply test for all possible code rates (start with 0/0 -> not used)
	if(e == 0 && d == 0)
	{
		pp->push_back(0);
	}
	else if(e == 1 && d == 6) // 1/6
	{
		pp->assign(t->d_PP_1_6, t->d_PP_1_6 + 6);
	}
	else if(e == 1 && d == 4) // 1/4
	{
		pp->assign(t->d_PP_1_4, t->d_PP_1_4 + 6);
	}
	else if(e == 3 && d == 10) // 3/10
	{
		pp->assign(t->d_PP_3_10, t->d_PP_3_10 + 18);
	}
	else if(e == 1 && d == 3) // 1/3
	{
		pp->assign(t->d_PP_1_3, t->d_PP_1_3 + 6);
	}
	else if(e == 4 && d == 11) // 4/11
	{
		pp->assign(t->d_PP_4_11, t->d_PP_4_11 + 24);
	}
	else if(e == 2 && d == 5) // 2/5
	{
		pp->assign(t->d_PP_2_5, t->d_PP_2_5 + 12);
	}
	else if(e == 1 && d == 2) // 1/2
	{
		pp->assign(t->d_PP_1_2, t->d_PP_1_2 + 6);
	}
	else if(e == 4 && d == 7) // 4/7
	{
		pp->assign(t->d_PP_4_7, t->d_PP_4_7 + 24);
	}
	else if(e == 3 && d == 5) // 3/5
	{
		pp->assign(t->d_PP_3_5, t->d_PP_3_5 + 18);
	}
	else if(e == 2 && d == 3) // 2/3
	{
		pp->assign(t->d_PP_2_3, t->d_PP_2_3 + 12);
	}
	else if(e == 8 && d == 11) // 8/11
	{
		pp->assign(t->d_PP_8_11, t->d_PP_8_11 + 48);
	}
	else if(e == 3 && d == 4) // 3/4
	{
		pp->assign(t->d_PP_3_4, t->d_PP_3_4 + 18);
	}
	else if(e == 4 && d == 5) // 4/5
	{
		pp->assign(t->d_PP_4_5, t->d_PP_4_5 + 24);
	}
	else if(e == 7 && d == 8) // 7/8
	{
		pp->assign(t->d_PP_7_8, t->d_PP_7_8 + 42);
	}
	else if(e == 8 && d == 9) // 8/9
	{
		pp->assign(t->d_PP_8_9, t->d_PP_8_9 + 48);
	}
	else
	{
		std::cout << "Invalid code rate for puncturing pattern assignment!\n";
	}	
}

void
channel_params::set_punct_pat_tail(std::vector< unsigned char >* pp, int r_p, tables* t)
{
	if(r_p < 0 || r_p > 12)
	{
		std::cout << "Invalid tail bit puncturing pattern index!\n";
	}
	else
	{
		for( int i = 0; i < 36;  i++ )
		{
			pp->push_back(t->d_PP_tail[r_p][i]);
		}
	}
}

void
channel_params::set_interl_seq(std::vector< int >* seq, int x_in, int mod_order, int index)
{
	// for details please see the DRM standard chapter 7.3.3
	// special treatment for cell interleaver -> index 3, mod_order 0
	
	/* range checking */
	if( !( mod_order == 0 || mod_order == 2 || mod_order == 4 || mod_order == 6) )
	{
		std::cout << "Invalid modulation order!\n";
	}
	if( !(index == 0 || index == 1 || index == 2 || index == 3) )
	{
		std::cout << "Invalid index!\n";
	}
	
	/* determine t, s and q */
	int t = 0;
	int s = 0;
	int q = 0;
	
	switch(index)
	{
		case 0:
			if(mod_order == 2) // 4-QAM
			{
				t = 21;
			}
			else //  has to be 16-QAM
			{
				t = 13;
			}
			break;
		case 1:
			if(mod_order == 4) // 16-QAM
			{
				t = 21;
			}
			else // has to be 64-QAM
			{
				t = 13;
			}
			break;
		case 2: // 64-QAM
			t = 21;
			break;
		case 3: // Cell interleaving
			t = 5;
			break;
		default:
			// this can't happen as value checking is already done.
			break;
	}
	
	s = std::pow( 2, std::ceil( std::log(x_in) / std::log(2) ) );
	q = s/4 - 1;
	
	/* sequence generation */
	seq->push_back(0);
	
	for( int i = 1; i < x_in; i++)
	{
		seq->push_back( ( t * seq->at(i-1) + q ) % s );
		while(seq->at(i) >= x_in)
		{
			seq->at(i) = ( t * seq->at(i) + q ) % s;
		}
	}			
}

std::vector< int >
channel_params::r_p()
{
	return d_r_p;
}

int
channel_params::mod_order()
{
	return d_mod_order;
}

void
channel_params::set_mod_order(int order)
{
	d_mod_order = order;
}

std::vector< int >
channel_params::bit_interl_seq_0_1()
{
	return d_bit_interl_seq_0_1;
}

std::vector< int >
channel_params::bit_interl_seq_0_2()
{
	return d_bit_interl_seq_0_2;
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

	/* calculate and define code-rate-dependant variables and bit interleaving sequences */
	/* NOTE: There is no need to make a difference here between UEP and EEP as most of the values automatically
	 * 'default' to EEP if the number of bytes in the higher protected part
	 * is defined as zero. */

	if(cfg->msc_mapping() < 3) // Symmetrical mapping (SM)
	{
		// interleaving sequences are defined inside calc_vars_SM
		calc_vars_SM(cfg);
	}
	else if(cfg->msc_mapping() == 3) // Hierarchical symmetrical mapping (HMsym)
	{
		d_n_levels_mlc = 3;
		set_mod_order( 6 );
		
		std::cout << "not yet implemented...\n"; // TODO: implement code rate assignment for hierarchical mapping
	}
	else if(cfg->msc_mapping() == 4) // HMmix
	{
		d_n_levels_mlc = 6;
		set_mod_order( 6 );
		
		std::cout << "not yet implemented...\n";
	}
	else
	{
		std::cout << "Invalid MSC mapping index!\n";
	}
	
	/* set cell interleaver sequence */
	set_interl_seq(&d_cell_interl_seq, d_N_MUX, 0, 3);
}

void
msc_params::calc_vars_SM(config* cfg)
{
	/* define P_max, modulation order and sum of code rates */
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
	
	/* set modulation order */
	set_mod_order(P_max * 2);
	
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
	
	/* set puncturing patterns */
	tables* t = cfg->ptables();
	set_punct_pat(&d_punct_pat_0_2, d_R_0_enum_2, d_R_0_denom_2, t);
	set_punct_pat_tail(&d_punct_pat_tail_0_2, calc_r_p(d_N_2, d_R_0_denom_2), t);
	set_punct_pat(&d_punct_pat_1_2, d_R_1_enum_2, d_R_1_denom_2, t);
	set_punct_pat_tail(&d_punct_pat_tail_1_2, calc_r_p(d_N_2, d_R_1_denom_2), t);
	set_punct_pat(&d_punct_pat_2_2, d_R_2_enum_2, d_R_2_denom_2, t);
	set_punct_pat_tail(&d_punct_pat_tail_2_2, calc_r_p(d_N_2, d_R_2_denom_2), t);
	
	/* set interleaving sequences */
	switch(P_max)
	{
		case 1: // 4-QAM, RM E
			if(cfg->UEP())
			{
				set_interl_seq(&d_bit_interl_seq_0_1, 2*d_N_1, d_mod_order, 0); // the different parts shall not overlap (see 7.3.1.1 in the standard)
				set_interl_seq(&d_bit_interl_seq_0_2, 2*d_N_2, d_mod_order, 0);
			}
			else
			{
				set_interl_seq(&d_bit_interl_seq_0_2, 2*d_N_MUX, d_mod_order, 0);
			}
			break;
		case 2: // 16-QAM
			if(cfg->UEP())
			{
				set_interl_seq(&d_bit_interl_seq_0_1, 2*d_N_1, d_mod_order, 0); // the different parts shall not overlap (see 7.3.1.1 in the standard)
				set_interl_seq(&d_bit_interl_seq_0_2, 2*d_N_2, d_mod_order, 0);
				set_interl_seq(&d_bit_interl_seq_1_1, 2*d_N_1, d_mod_order, 1); 
				set_interl_seq(&d_bit_interl_seq_1_2, 2*d_N_2, d_mod_order, 1);
			}
			else
			{
				set_interl_seq(&d_bit_interl_seq_0_2, 2*d_N_MUX, d_mod_order, 0);
				set_interl_seq(&d_bit_interl_seq_1_2, 2*d_N_MUX, d_mod_order, 1);
			}
			break;
		case 3: // 64-QAM
			if(cfg->UEP())
			{
				set_interl_seq(&d_bit_interl_seq_1_1, 2*d_N_1, d_mod_order, 1); // the different parts shall not overlap (see 7.3.1.1 in the standard)
				set_interl_seq(&d_bit_interl_seq_1_2, 2*d_N_2, d_mod_order, 1);
				set_interl_seq(&d_bit_interl_seq_2_1, 2*d_N_1, d_mod_order, 2); 
				set_interl_seq(&d_bit_interl_seq_2_2, 2*d_N_2, d_mod_order, 2);
			}
			else
			{
				set_interl_seq(&d_bit_interl_seq_1_2, 2*d_N_MUX, d_mod_order, 1);
				set_interl_seq(&d_bit_interl_seq_2_2, 2*d_N_MUX, d_mod_order, 2);
			}
			break;
		default:
			break;
	}		
	
	/* set indexes for partitioning */
	if(P_max >= 1) // 4-QAM
	{
		if(cfg->UEP())
		{
			int M_01 = 2 * d_N_1 * d_R_0_enum_1 / d_R_0_denom_1;
			d_M_index.push_back(M_01);
		}
		else
		{
			d_M_index.push_back(0); // in case of EEP set the index for the higher protected part to 0
		}
		int M_02 = d_R_0_enum_2 * std::floor( (2*d_N_2 - 12) / d_R_0_denom_2 );
		d_M_index.push_back(M_02);
	}
	if(P_max >= 2) // 16-QAM
	{
		if(cfg->UEP())
		{
			int M_11 = 2 * d_N_1 * d_R_1_enum_1 / d_R_1_denom_1;
			d_M_index.push_back(M_11);
		}
		else
		{
			d_M_index.push_back(0); // in case of EEP set the index for the higher protected part to 0
		}
		int M_12 = d_R_1_enum_2 * std::floor( (2*d_N_2 - 12) / d_R_1_denom_2 );
		d_M_index.push_back(M_12);
	}
	if(P_max >= 3) // 64-QAM
	{	
		if(cfg->UEP())
		{
			int M_21 = 2 * d_N_1 * d_R_2_enum_1 / d_R_2_denom_1;
			d_M_index.push_back(M_21);
		}
		else
		{
			d_M_index.push_back(0); // in case of EEP set the index for the higher protected part to 0
		}
		int M_22 = d_R_2_enum_2 * std::floor( (2*d_N_2 - 12) / d_R_2_denom_2 );
		d_M_index.push_back(M_22);
	}	
	
	/* set total number of bits per level in the encoder */
	for(int i = 0; i < d_M_index.size(); i+=2)
	{
		d_M_total.push_back(d_M_index[i] + d_M_index[i+1]);
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
msc_params::M_index()
{
	return d_M_index;
}

std::vector< int >
msc_params::M_total()
{
	return d_M_total;
}

unsigned short
msc_params::n_levels_mlc()
{
	return d_n_levels_mlc;
}

std::vector< unsigned char >
msc_params::punct_pat_0_2()
{
	return d_punct_pat_0_2;
}

std::vector< unsigned char >
msc_params::punct_pat_tail_0_2()
{
	return d_punct_pat_tail_0_2;
}

std::vector< unsigned char >
msc_params::punct_pat_1_2()
{
	return d_punct_pat_1_2;
}

std::vector< unsigned char >
msc_params::punct_pat_tail_1_2()
{
	return d_punct_pat_tail_1_2;
}

std::vector< unsigned char >
msc_params::punct_pat_2_2()
{
	return d_punct_pat_2_2;
}

std::vector< unsigned char >
msc_params::punct_pat_tail_2_2()
{
	return d_punct_pat_tail_2_2;
}

std::vector< int >
msc_params::cell_interl_seq()
{
	return d_cell_interl_seq;
}

std::vector< int >
msc_params::bit_interl_seq_1_1()
{
	return d_bit_interl_seq_1_1;
}

std::vector< int >
msc_params::bit_interl_seq_1_2()
{
	return d_bit_interl_seq_1_2;
}

std::vector< int >
msc_params::bit_interl_seq_2_1()
{
	return d_bit_interl_seq_2_1;
}

std::vector< int >
msc_params::bit_interl_seq_2_2()
{
	return d_bit_interl_seq_2_2;
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

std::vector< int >
control_chan_params::M_total()
{
	return d_M_total;
}

std::vector< unsigned char >
control_chan_params::punct_pat_0()
{
	return d_punct_pat_0;
}

std::vector< unsigned char >
control_chan_params::punct_pat_tail_0()
{
	return d_punct_pat_tail_0;
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


	/* set code rates and modulation order */
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
	
	/* set modulation order */
	set_mod_order( P_max * 2 );
	
	/* set puncturing patterns */
	tables* t = cfg->ptables();
	set_punct_pat(&d_punct_pat_0, d_R_0_enum, d_R_0_denom, t);
	set_punct_pat_tail(&d_punct_pat_tail_0, calc_r_p(d_N, d_R_0_denom), t);
	set_punct_pat(&d_punct_pat_1, d_R_1_enum, d_R_1_denom, t);
	set_punct_pat_tail(&d_punct_pat_tail_1, calc_r_p(d_N, d_R_1_denom), t);
	
	/* set interleaving sequences */
	switch(P_max)
	{
		case 1:
			set_interl_seq(&d_bit_interl_seq_0_2, 2 * d_N, d_mod_order, 0);
			break;
		case 2:
			set_interl_seq(&d_bit_interl_seq_0_2, 2 * d_N, d_mod_order, 0);
			set_interl_seq(&d_bit_interl_seq_1_2, 2 * d_N, d_mod_order, 1);
			break;
		default:
			break;
	}
	
	/* set indexes for partitioning and number of levels in the coding process */
	
	if(P_max >= 1) // 4-QAM
	{
		d_n_levels_mlc = 1;
		int M_02 = d_R_0_enum * std::floor( (2*d_N - 12) / d_R_0_denom );
		d_M_index.push_back(M_02);
	}
	if(P_max >= 2) // 16-QAM
	{
		d_n_levels_mlc = 2;
		int M_12 = d_R_1_enum * std::floor( (2*d_N - 12) / d_R_1_denom );
		d_M_index.push_back(M_12);
	}
	d_M_total = d_M_index; // in SDC mode, there is only SM and EEP -> identical with M_index
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
sdc_params::M_index()
{
	return d_M_index;
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

std::vector< unsigned char >
sdc_params::punct_pat_1()
{
	return d_punct_pat_1;
}

std::vector< unsigned char >
sdc_params::punct_pat_tail_1()
{
	return d_punct_pat_tail_1;
}

std::vector< int > 
sdc_params::bit_interl_seq_1_1()
{
	return d_bit_interl_seq_1_1;
}

std::vector< int > 
sdc_params::bit_interl_seq_1_2()
{
	return d_bit_interl_seq_1_2;
}

/* FAC channel implementation */
void
fac_params::init(config* cfg)
{
	/* set d_N, d_L, modulation order and code rate */
	set_mod_order( 2 );
	if(cfg->RM() == 4) // see DRM standard 7.2.1.2 & Table 74/75
	{
		// RM E
		d_N = N_FAC_DRMPLUS;
		d_L = L_FAC_DRMPLUS;
		d_R_0_enum = 1;
		d_R_0_denom = 4;
		d_R_0 = d_R_0_enum/float(d_R_0_denom);
		set_punct_pat(&d_punct_pat_0, d_R_0_enum, d_R_0_denom, cfg->ptables());
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
	
	d_M_total.push_back(d_L); // this is needed as the partitioner expects an std::vector<>
	
	/* set puncturing patterns */
	tables* t = cfg->ptables();
	set_punct_pat(&d_punct_pat_0, d_R_0_enum, d_R_0_denom, t);
	d_punct_pat_tail_0 = d_punct_pat_0; //  no special tailbit treatment for FAC
	
	/* set interleaving sequence */
	set_interl_seq(&d_bit_interl_seq_0_2, 2 * d_N, d_mod_order, 0);
	
	
}
