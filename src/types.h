#ifndef __GS_TYPES__
#define __GS_TYPES__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WINDOWS
typedef __int64            int64;
typedef unsigned __int64   uint64;
#else
typedef long long          int64;
typedef unsigned long long uint64;
#endif

typedef unsigned int     uint32;
typedef int              int32;

typedef unsigned char	 uchar;

#endif
