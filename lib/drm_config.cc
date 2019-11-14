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

#include <stdexcept>
#include <drm/drm_config.h>

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
	d_RM = RM;
	d_SO = SO;
	d_UEP = false; // EEP
	d_n_bytes_A = 0; // EEP
	if(text_message.size() > 0) // set flag if a text message is available
	{
		d_text = true;
	}
	d_long_interl = long_interl;
	d_msc_mapping = msc_mapping;
	if(RM == 4 && msc_mapping != 0) // no other mapping tahn 4QAM is possible for DRM+
	{
		std::cout << "config:: MSC mapping has been reset to 4-QAM. This is the only mapping scheme used for DRM+.\n";
		d_msc_mapping = 0;
	}
	d_msc_prot_level_1 = 0; // not used because UEP==0
	d_msc_prot_level_2 = msc_prot_level_2;
	d_sdc_mapping = sdc_mapping;
	d_sdc_prot_level = 0; // R = 0.5, takes only effect if RM E is chosen
	d_audio_samp_rate = audio_samp_rate; // 12 or 24 kHz audio
	d_station_label = station_label; // station label (up to 16 characters)
	d_text_message = text_message; // text message (up to 8*16 bytes of UTF-8 encoded characters)

	/* pointer to tables needed for init */
	d_ptables = ptr_tables;

	/* make some range and combination checks */
	check_arguments();
}

void
config::check_arguments()
{
	bool valid = true;

	/* check for unsupported values and combinations */

	/* some range checks */
	if (d_RM < 0 || d_RM > 4)
	{
		throw std::runtime_error("SO out of range!\n");
	}
	if (d_SO < 0 || d_SO > 5)
	{
		throw std::runtime_error("SO out of range!\n");
	}
	if (d_msc_mapping < 0 || d_msc_mapping > 4)
	{
		throw std::runtime_error("MSC mapping out of range!\n");
	}
	if (d_sdc_mapping < 0 || d_sdc_mapping > 1)
	{
		throw std::runtime_error("SDC mapping out of range!\n");
	}
	if (d_sdc_prot_level < 0 || d_sdc_prot_level > 1)
	{
		throw std::runtime_error("Protection level out of range!\n");
	}

	/* some combination checks */
	if (d_RM == 4 && d_SO != 0)
	{
		throw std::runtime_error("SO has to be 0 in RM E!\n");
	}
	if ((d_RM == 2 || d_RM == 3 ) && (d_SO < 3 || d_SO == 4))
	{
		throw std::runtime_error("Invalid RM/SO combination! See Annex J in the standard.\n");
	}
	if (d_RM < 4 && d_sdc_prot_level != 0)
	{
		throw std::runtime_error("Invalid SDC protection level (only for RM E, otherwise set it to zero)!\n");
	}
	if (d_msc_mapping > 2 && d_sdc_mapping == 0)
	{
		throw std::runtime_error("If hierarchical mapping is used, SDC has to use 4-QAM!\n");
	}
	if(!(d_audio_samp_rate == 12000 || d_audio_samp_rate == 24000 || d_audio_samp_rate == 48000))
	{
		throw std::runtime_error("Invalid audio sample rate!\n");
	}
	if(d_RM < 4 && d_audio_samp_rate > 24000)
	{
		throw std::runtime_error("48 kHz sample rate is only valid for RM E!\n");
	}
	if(d_RM == 4 && d_audio_samp_rate < 24000)
	{
		throw std::runtime_error("12 kHz sample rate is only valid for RM A-E!\n");
	}
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
