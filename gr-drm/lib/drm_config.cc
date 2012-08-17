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
 
#include "drm_config.h"

/* config implementation */
config::config()
{
	d_RM = 0;
	d_SO = 0;
	d_UEP = 0;
	d_n_bytes_A = 0;
	d_text = false;
	d_long_interl =	0;
    d_msc_mapping = 0;
    d_msc_prot_level_1 = 0;
	d_msc_prot_level_2 = 0;
	d_sdc_mapping = 0;
	d_sdc_prot_level = 0;
	d_audio_samp_rate = 0;
	d_ptables = NULL;
}

void
config::init(tables* ptr_tables, 
			  unsigned short RM, 
			  unsigned short SO, 
			  bool UEP,
			  unsigned int n_bytes_A, 
			  bool text,
			  unsigned short msc_mapping,
			  unsigned short msc_prot_level_1, 
			  unsigned short msc_prot_level_2,
			  unsigned short sdc_mapping,
			  unsigned short sdc_prot_level,
			  bool long_interl,
			  unsigned int audio_samp_rate,
			  std::string station_label,
			  std::string text_message)
{
	/* user defined parameters */
	//d_RM = 1; // B
	d_RM = RM;
	//d_SO = 3; // 10 kHz
	d_SO = SO;
	d_UEP = false; // EEP
	d_n_bytes_A = 0; // EEP
	if(text_message.size() > 0) // set text message flag
	{
		d_text = true; // this is false by default, so only change it when there is a message
	}
	d_long_interl = long_interl;
	//d_long_interl = false; // short interleaving (400ms)
	d_msc_mapping = msc_mapping;
	if(RM == 4 && msc_mapping != 0) // no other mapping tahn 4QAM is possible for DRM+
	{
		std::cout << "config:: MSC mapping has been reset to 4-QAM. This is the only mapping scheme used for DRM+.\n";
		d_msc_mapping = 0;
	}
	//d_msc_mapping = 1; // 16-QAM SM
	d_msc_prot_level_1 = 0; // not used because UEP==0
	//d_msc_prot_level_2 = 1; // R_all = 0.62
	d_msc_prot_level_2 = msc_prot_level_2;
	d_sdc_mapping = 1; // 4-QAM
	d_sdc_prot_level = 0; // R = 0.5, takes only effect if RM E is chosen
	d_audio_samp_rate = audio_samp_rate; // 12 or 24 kHz audio
	d_station_label = station_label; // station label (up to 16 characters)
	d_text_message = text_message; // text message (up to 8*16 bytes of UTF-8 encoded character data)

	/* pointer to tables needed for init */
	d_ptables = ptr_tables;

	/* make some range and combination checks */
	if(!check_arguments())
	{
		std::cout << "INVALID CONFIGURATION!\n";
	}
}

bool
config::check_arguments()
{
	bool valid = true;

	/* check for unsupported values and combinations */

	/* some range checks */
	if (d_RM < 0 || d_RM > 4)
	{
		std::cout << "RM out of range!\n";
		valid = false;
	}
	if (d_SO < 0 || d_SO > 5)
	{
		std::cout << "SO out of range!\n";
		valid = false;
	}
	if (d_msc_mapping < 0 || d_msc_mapping > 4)
	{
		std::cout << "MSC mapping out of range!\n";
		valid = false;
	}
	if (d_sdc_mapping < 0 || d_sdc_mapping > 1)
	{
		std::cout << "SDC mapping out of range!\n";
		valid = false;
	}
	if (d_sdc_prot_level < 0 || d_sdc_prot_level > 1)
	{
		std::cout << "Protection level out of range!\n";
		valid = false;
	}

	/* some combination checks */
	if (d_RM == 4 && d_SO != 0)
	{
		std::cout << "SO has to be 0 in RM E!\n";
		valid = false;
	}
	if ((d_RM == 2 || d_RM == 3 ) && (d_SO < 3 || d_SO == 4))
	{
		std::cout << "Invalid RM/SO combination! See Annex J in the standard.\n";
		valid = false;
	}
	if (d_RM < 4 && d_sdc_prot_level != 0)
	{
		std::cout << "Invalid SDC protection level (only for RM E, otherwise set it to zero)!\n";
		valid = false;
	}
	if (d_msc_mapping > 2 && d_sdc_mapping == 0)
	{
		std::cout << "If hierarchical mapping is used, SDC has to use 4-QAM!\n";
		valid = false;
	}
	if(!(d_audio_samp_rate == 12000 || d_audio_samp_rate == 24000 || d_audio_samp_rate == 48000))
	{
		std::cout << "Invalid audio sample rate!\n";
		valid = false;
	}
	if(d_RM < 4 && d_audio_samp_rate > 24000)
	{
		std::cout << "48 kHz sample rate is only valid for RM E!\n";
		valid = false;
	}
	if(d_RM == 4 && d_audio_samp_rate < 24000)
	{
		std::cout << "12 kHz sample rate is only valid for RM A-E!\n";
		valid = false;
	}

	return valid;
}

unsigned short
config::RM()
{
	return d_RM;
}

unsigned short
config::SO()
{
	return d_SO;
}

bool
config::UEP()
{
	return d_UEP;
}

unsigned int
config::n_bytes_A()
{
	return d_n_bytes_A;
}

bool
config::text()
{
	return d_text;
}

bool
config::long_interl()
{
	return d_long_interl;
}

unsigned short
config::msc_mapping()
{
	return d_msc_mapping;
}

unsigned short
config::msc_prot_level_1()
{
	return d_msc_prot_level_1;
}

unsigned short
config::msc_prot_level_2()
{
	return d_msc_prot_level_2;
}

unsigned short
config::sdc_mapping()
{
	return d_sdc_mapping;
}

unsigned short
config::sdc_prot_level()
{
	return d_sdc_prot_level;
}

unsigned int
config::audio_samp_rate()
{
	return d_audio_samp_rate;
}

std::string
config::station_label()
{
	return d_station_label;
}

std::string
config::text_message()
{
	return d_text_message;
}

tables*
config::ptables()
{
	return d_ptables;
}
