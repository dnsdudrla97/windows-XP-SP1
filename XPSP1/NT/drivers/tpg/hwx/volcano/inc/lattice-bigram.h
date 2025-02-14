#ifndef _LATTICE_BIGRAM_INCLUDED
#define _LATTICE_BIGRAM_INCLUDED

#include "imlang.h"

typedef struct	tag_LATTICE_BIGRAM_INFO
{
	DWORD dwBigram;
	DWORD dwPrevElement;
}	LATTICE_BIGRAM_INFO;

// {11858E48-DE9B-4fe8-BCC8-B35CC1B1E2F0}
DEFINE_GUID( GUID_LATTICE_BIGRAM_INFO_LIST, 0x11858e48, 0xde9b, 0x4fe8, 0xbc, 0xc8, 0xb3, 0x5c, 0xc1, 0xb1, 0xe2, 0xf0 );

typedef struct  tag_LATTICE_BIGRAM_INFO_LIST
{
	DWORD dwTotalBigrams;
	LATTICE_BIGRAM_INFO bigrams[1];
}	LATTICE_BIGRAM_INFO_LIST;

#endif
