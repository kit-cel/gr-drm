#include "drm_config.h"

/* config implementation */
config::config()
{
	d_RM = 0;
	d_SO = 0;
	d_UEP = 0;
	d_n_bytes_A = 0;
	d_text = 0;
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
			  unsigned int audio_samp_rate)
{
	/* user defined parameters */
	//d_RM = 1; // B
	d_RM = RM;
	//d_SO = 3; // 10 kHz
	d_SO = SO;
	d_UEP = false; // EEP
	d_n_bytes_A = 0; // EEP
	d_text = false; // no text message included
	d_long_interl = false; // short interleaving (400ms)
	d_msc_mapping = 1; // 16-QAM SM
	d_msc_prot_level_1 = 0; // not used because UEP==0
	//d_msc_prot_level_2 = 1; // R_all = 0.62
	d_msc_prot_level_2 = msc_prot_level_2;
	d_sdc_mapping = 1; // 4-QAM
	d_sdc_prot_level = 0; // R = 0.5, takes only effect if RM E is chosen
	d_audio_samp_rate = audio_samp_rate; // 24 kHz audio

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

tables*
config::ptables()
{
	return d_ptables;
}
