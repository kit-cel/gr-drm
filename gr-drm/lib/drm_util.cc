#include "drm_util.h"
#include <iostream>

void enqueue_bits(unsigned char* &ptr, unsigned int len, unsigned char arr[])
{
	for(unsigned int i = 0; i<len; i++)
	{
		if(arr[i] > 1)
		{
			std::cout << "Error! Received non-binary value: " << arr[i] << std::endl;
		}
		else
		{
			*ptr++ = arr[i];
			//std::cout << " wrote " << (int) *(ptr-1) << " previously wrote " << (int) *(ptr-2) << std::endl;
		}
	}
}

void enqueue_bits_dec(unsigned char* &ptr, unsigned int len, unsigned int val)
{
	if(sizeof(val) <= sizeof(unsigned int) && (val <= std::pow(2,len)-1 || (val == 0 && len > 0)))
	{
		unsigned char bits[sizeof(unsigned int)*8];
		unsigned int mask = 0x1;
		for(unsigned int i = 0; i < sizeof(bits); i++)
		{
			bits[i] = val & mask;
			val >>= 1; // this ends up LSb first
		}
		for(unsigned int i = 0; i < len; i++)
		{
			*ptr++ = bits[len-i-1]; // invert order to MSb first
		}
	}
	else
	{
		std::cout << "Value cannot be converted because it is too large!\n";
	}
}

void enqueue_crc(unsigned char* ptr, unsigned short rob_mode, const unsigned short ord)
{
	unsigned int len; // length of input bitstream
	unsigned char shift_reg[ord]; // shift register of length ord
	unsigned char shift_reg_prev[ord]; // state of register before last shift
	unsigned char next_lsb; // the bit that results out of XORing the MSb with the input
	unsigned char crc_word[ord]; // CRC word
	memset(shift_reg, 1, ord); // set all stages to one (see DRM standard, annex D)
	memset(shift_reg_prev, 0, ord); // just for debugging purposes
	switch(ord)
	{
		case 8: // FAC: G(x) = x^8 + x^4 + x^3 + x^2 + 1			
			if(rob_mode < 4) // standard DRM
			{
				len = 64; // 20 bits of channel parameters + 44 bits of service parameters
			}
			else // DRM+
			{
				len = 112; // 20 bits  + 2 * 44 bits + 4 bits (zeros, only for CRC calculation) 
			}
			for(int i = 0; i < len; i++)
			{
				memcpy(shift_reg_prev, shift_reg, ord);						
				next_lsb = ( ptr[i] + shift_reg_prev[0] ) % 2;
				shift_reg[7] = next_lsb;
				shift_reg[6] = shift_reg_prev[7];
				shift_reg[5] = ( shift_reg_prev[6] + next_lsb ) % 2;
				shift_reg[4] = ( shift_reg_prev[5] + next_lsb ) % 2;
				shift_reg[3] = ( shift_reg_prev[4] + next_lsb ) % 2;
				shift_reg[2] = shift_reg_prev[3];
				shift_reg[1] = shift_reg_prev[2];
				shift_reg[0] = shift_reg_prev[1];				
			}
			break;
			
		case 16: // SDC: G(x) = x^16 + x^12 + x^5 + 1
			// TODO: add a table with the lenght of the SDC data field to tables
			// TODO: calc crc
			
			break;
			
		default:
			std::cout << "Invalid order of CRC polynomial!\n";
			break;
	}
	
	/* read and append CRC word */
	for(unsigned int i = 0; i < ord; i++)
	{
		crc_word[i] = ( shift_reg[i] + 1 ) % 2; // CRC word shall be inverted prior to transmission
		ptr[i + len] = crc_word[i]; // append CRC word
	}
}
