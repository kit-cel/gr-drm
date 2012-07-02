#ifndef _DRM_UTIL_H
#define _DRM_UTIL_H

#include <cstring>

// enqueue bit array arr of lenght len at address ptr
void enqueue_bits(unsigned char* &ptr, unsigned int len, unsigned char arr[]);

// enqueue crc word of order ord after bit sequence of length len at address ptr
void enqueue_crc(unsigned char* ptr, unsigned short rob_mode, unsigned short ord);

#endif
