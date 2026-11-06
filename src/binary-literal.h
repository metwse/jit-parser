/* 32-bit base2 literals */

#ifndef BINARY_LITERAL_H
#define BINARY_LITERAL_H

#include <stdint.h>


/* https://post.bytes.com/forum/topic/c/179179-literal-binary */
#define __hex(n) 0x ## n ## llu
#define __b(x) (((x) & 0x000000000000000Fllu) ? 1 : 0) \
	     + (((x) & 0x00000000000000F0llu) ? 2 : 0) \
	     + (((x) & 0x0000000000000F00llu) ? 4 : 0) \
	     + (((x) & 0x000000000000F000llu) ? 8 : 0) \
	     + (((x) & 0x00000000000F0000llu) ? 16 : 0) \
	     + (((x) & 0x0000000000F00000llu) ? 32 : 0) \
	     + (((x) & 0x000000000F000000llu) ? 64 : 0) \
	     + (((x) & 0x00000000F0000000llu) ? 128 : 0) \
	     + (((x) & 0x0000000F00000000llu) ? 256 : 0) \
	     + (((x) & 0x000000F000000000llu) ? 512 : 0) \
	     + (((x) & 0x00000F0000000000llu) ? 1024 : 0) \
	     + (((x) & 0x0000F00000000000llu) ? 2048 : 0) \
	     + (((x) & 0x000F000000000000llu) ? 4096 : 0) \
	     + (((x) & 0x00F0000000000000llu) ? 8192 : 0) \
	     + (((x) & 0x0F00000000000000llu) ? 16384 : 0) \
	     + (((x) & 0xF000000000000000llu) ? 32768 : 0) \

#define b(d) ((uint8_t) __b(__hex(d)))


#endif
