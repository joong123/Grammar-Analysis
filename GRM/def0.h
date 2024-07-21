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

#ifndef ISNULLPTR
#define ISNULLPTR(ptr)		(NULL == (ptr))
#endif
#ifndef ISNOTNULLPTR
#define ISNOTNULLPTR(ptr)	(NULL != (ptr))
#endif

#ifndef zeromem
#define zeromem(p, len)		memset((p), 0, (len))
#endif

#ifndef SAFECALL1
#define SAFECALL1(fun, param1)	if (NULL != (fun)) fun(param1)
#endif

#define SET_PTRVAL(PTR, VAL)	if (NULL != (PTR)) *(PTR) = (VAL);

#define RET_ON_NEG(ret)		{ if(0 > (ret)) return (ret); }
#define RET_ON_NPOS(ret)	{ if(0 >= (ret)) return (ret); }

#define OVERFLOW_MUL(a, b, v)		((v) / (b) < (a))
#define OVERFLOW_MUL_sizet(a, b)	OVERFLOW_MUL((a), (b), (size_t)(-1))

#ifndef LIB_API
	#ifdef LIB_IMPORT
		#define LIB_API __declspec(dllimport)// for chain import in .cpp
	#elif defined LIB_EXPORT
		#define LIB_API __declspec(dllexport)
	#else
		#define LIB_API
	#endif
#endif


typedef int32_t				i32;
typedef int64_t				i64;

typedef wchar_t				wch;
typedef char				ch;


enum G_RET
{
	GRET_ERROR		= -1,
	GRET_NULL		= -2,
	GRET_INVOP		= -3,
	GRET_INVARG		= -4,
	GRET_NULLARG	= -5,
	GRET_INVSTATUS	= -6,
	GRET_MALLOC		= -7,
	GRET_OVERFLOW	= -8,
	GRET_INVMETA	= -9,
	GRET_DUPKEY		= -10,
	GRET_INVIDX		= -11,
	GRET_NULLFUNC	= -12,

	GRET_NOTFOUND	= -100,

	GRET_NONE		= 0,

	GRET_SUCCEED	= 1
};

inline void safefree(void* ptr)
{
	if (ptr)
		free(ptr);
}

inline void safefreereset(void** pptr)
{
	if (pptr)
	{
		if (*pptr)
		{
			free(*pptr);
			*pptr = NULL;
		}
	}
}
