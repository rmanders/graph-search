#ifndef __GS_TYPES__
#define __GS_TYPES__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a,b)	(((a)>=(b)) ? (a) : (b))
#define MIN(a,b)	(((a)<(b)) ? (a) : (b))

#ifdef WINDOWS
typedef __int64            Int64Type;
typedef unsigned __int64   Uint64Type;
#else
typedef long long          Int64Type;
typedef unsigned long long Uint64Type;
#endif

typedef unsigned int       Uint32Type;
typedef int                Int32Type;
typedef unsigned char	   UcharType;
typedef unsigned char      ByteType;
typedef unsigned int       VertexIdType;


typedef struct _AdjListNode {
	VertexIdType mVertexId;
	struct _AdjListNode* mNextNode;
	struct _AdjListNode* mPrevNode;
} AdjListNode;

#endif
