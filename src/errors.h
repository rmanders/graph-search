#ifndef __GS_ERROR__
#define __GS_ERROR__

#include <exception>
#include "types.h"

enum {
	GSERR_OUT_OF_MEMORY,
	GSERR_OUT_OF_RANGE,
	GSERR_OVER_INDEX_LIMIT,
	GSERR_MAX_GRAPH_SIZE_EXCEEDED,
	GSERR_EDGE_MISMATCH,
	GSERR_NO_SUCH_VERTEX,
	GSERR_EDGE_ALREADY_EXISTS
};

struct GSERRORS {
	int id;
	const char* msg;
} GSErrors[] = {
	{ 0,                             "No Error" },
	{ GSERR_OUT_OF_MEMORY,           "Unable to allocate any more memory" },
	{ GSERR_OUT_OF_RANGE,            "Index was out of range" },
	{ GSERR_OVER_INDEX_LIMIT,        "Size exceeded index limit" },
	{ GSERR_MAX_GRAPH_SIZE_EXCEEDED, "The graph size exceeded the maxmimum size allowable" },
	{ GSERR_EDGE_MISMATCH,           "Edge mismatch between assignment of two graph structures" },
	{ GSERR_NO_SUCH_VERTEX,         "Refereced none-existent Vertex Id" },
	{ GSERR_EDGE_ALREADY_EXISTS,     "Operation would overwrite already exiting edge" },
	{ -1,                            "Undefined error" }
};


class GsException : public exception {
private:
	int mErrorNum;
public:
	GsException( void ) : exception() {
		mErrorNum = -1;
	}

	GsException( Uint32Type iErrorNum ) : exception() {
		mErrorNum = iErrorNum;
	}
	
	virtual const char* what() const throw() {
		int i;
		for(i=0; GSErrors[i].id != -1; i++) {
			if(mErrorNum == GSErrors[i].id)
				break;
		}
		return GSErrors[i].msg;
	}
};

#endif
