#include "drm_config.h"

/* config implementation */
void
config::init(tables* ptr_tables)
{
	std::cout << "init cfg\n";

	/* user defined parameters */
	//d_RM = 1; // B
	//d_SO = 3; // 10 kHz
	d_RM = 4; // maximum values for RM and SO to detect out of bounds in matrices
	d_SO = 5;
	d_UEP = false; // EEP
	d_text = false; // no text message included
	d_long_interl = false; // short interleaving (400ms)
	d_msc_mapping = 0; // 16-QAM SM
	d_sdc_mapping = 0; // 4-QAM

	/* pointer to tables needed for init */
	d_ptables = ptr_tables;
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
config::sdc_mapping()
{
	return d_sdc_mapping;
}
