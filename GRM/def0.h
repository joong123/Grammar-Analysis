#pragma once

#include "stdint.h"
#include <malloc.h>


#ifndef max
#define max(a,b)		(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)		(((a) < (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(n)			(((n) < (0)) ? (-(n)) : (n))
#endif

#ifndef ISNULL
#define ISNULLPTR(ptr)		(NULL == (ptr))
#endif

#define SET_PTRVAL(PTR, VAL)	if (NULL != (PTR)) *(PTR) = (VAL);

#define RET_ON_NEG(ret)		{ if(0 > (ret)) return (ret); }


typedef int32_t				i32;
typedef int64_t				i64;

typedef wchar_t				wch;
typedef char				ch;


enum G_RET
{
	GRET_ERROR		= -1,
	GRET_NULL		= -2,
	GRET_INVARG		= -3,
	GRET_NULLARG	= -4,
	GRET_INVSTATUS	= -5,
	GRET_MALLOC		= -6,
	GRET_OVERFLOW	= -7,
	GRET_INVMETA	= -8,
	GRET_DUPKEY		= -9,
	GRET_INVIDX		= -10,

	GRET_NOTFOUND	= -100,

	GRET_NONE		= 0,

	GRET_SUCCEED	= 1
};

inline void safefree(void* ptr)
{
	if (ptr)
		free(ptr);
}