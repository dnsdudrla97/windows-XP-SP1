/**************************************************************************
 * module: UTIL.C
 *
 * author: Louise Pathe
 * date:   Sept 1995
 * Copyright 1990-1997. Microsoft Corporation.
 *
 * utility module 
 *
 **************************************************************************/

/* Inclusions ----------------------------------------------------------- */
#include <stdlib.h>
#ifdef _DEBUG
#include <stdio.h>
#endif

#include "typedefs.h"
#include "util.h"

/* ---------------------------------------------------------------------- */
/* stolen from ffconfig mtxcalc.c */
uint16 log2( uint16 arg )
   {
   if ( arg < 2 )     return( 0 );
   if ( arg < 4 )     return( 1 );
   if ( arg < 8 )     return( 2 );
   if ( arg < 16 )    return( 3 );
   if ( arg < 32 )    return( 4 );
   if ( arg < 64 )    return( 5 );
   if ( arg < 128 )   return( 6 );
   if ( arg < 256 )   return( 7 );
   if ( arg < 512 )   return( 8 );
   if ( arg < 1024 )  return( 9 );
   if ( arg < 2048 )  return( 10 );
   if ( arg < 4096 )  return( 11 );
   if ( arg < 8192 )  return( 12 );
   if ( arg < 16384 ) return( 13 );
   if ( arg < 32768 ) return( 14 );
   return( 15 );
   }

/* ---------------------------------------------------------------------- */
int16 ValueOKForShort(uint32 ulValue)
{
	if (ulValue & 0xFFFF0000) /* any high bits turned on */
		return(0);
	return(1);
}

#ifdef _DEBUG
/* ---------------------------------------------------------------------- */
void DebugMsg( char * CONST msg1, char * CONST msg2, uint16 CONST cLineNumber)
{
char szErrorBuf[256];

	if (cLineNumber == 0)
		sprintf(szErrorBuf,"DEBUG: %s %s.\n", msg1, msg2);
	else
		sprintf(szErrorBuf,"DEBUG: %s %s. Line: %d\n", msg1, msg2, cLineNumber);
	fprintf(stdout, szErrorBuf);

}
/* ---------------------------------------------------------------------- */
#endif