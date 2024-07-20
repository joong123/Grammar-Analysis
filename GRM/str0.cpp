#include "pch.h"
#define LIB_EXPORT
#include "str0.h"

#include <malloc.h>
#include <wchar.h>

#ifdef __cplusplus
#include <type_traits>
#endif



bool _PtrOverlap(const void* const p1, size_t len1, const void* const p2, size_t len2)
{
	return (p2 < (char*)p1 + len1) && (p1 < (char*)p2 + len2);
}

bool _PtrOverlapWCH(const wch* p1, size_t len1, const wch* p2, size_t len2)
{
	return (p2 < p1 + len1) && (p1 < p2 + len2);
}

int32_t _WCSEqual(const wch* p1, size_t len, const wch* p2, size_t len2)
{
	if (len != len2)
		return 0;
	for (size_t i = 0; i < len; ++i)
	{
		if (p1[i] != p2[i])
			return 0;
	}
	return 1;
}

int32_t _WCSEqualCI(const wch* p1, size_t len, const wch* p2, size_t len2)
{
	if (len != len2)
		return 0;
	for (size_t i = 0; i < len; ++i)
	{
		if (p1[i] != p2[i])
		{
			const wch c1 = p1[i];
			const wch c2 = p2[i];
			const wch dc = (L'a' - L'A');
			if (!(c1 >= L'a' && c1 <= 'z' && c1 - dc == c2) &&
				!(c1 >= 'A' && c1 <= 'Z' && c1 + dc == c2))
				return 0;
		}
	}
	return 1;
}



#pragma region STR1

//
// WStr1MakeCS			-> WStr1MakeCSBy2
// WStr1MakeCSBy		-> WStr1MakeCSBy2
// WStr1MakeCSBy2		-> WStr1PureMakeCSBy2
// 
// WStr1Make			-> WStr1MakeCSBy2
// WStr1MakeBy		-> WStr1MakeBy2
// WStr1MakeBy2		-> WStr1MakeCSBy2
// 
// WStr1PureMakeCS		-> WStr1PureMakeCSBy2
// WStr1PureMakeCSBy	-> WStr1PureMakeCSBy2
// WStr1PureMakeCSBy2
// 
// WStr1PureMake		-> WStr1PureMakeCSBy2
// WStr1PureMakeBy	-> WStr1PureMakeBy2
// WStr1PureMakeBy2	-> WStr1PureMakeCSBy2
// 
// WStr1Copy			-> WStr1MakeCSBy2
// WStr1CopyBy			-> WStr1CopyBy2
// WStr1CopyBy2			-> WStr1MakeCSBy2
//

static const size_t szStr1 = sizeof(str1);
static const size_t szWStr1 = sizeof(wstr1);
static const size_t szStr2 = sizeof(str2);
static const size_t szWStr2 = sizeof(wstr2);
static const size_t szWStr1Local = (sizeof(wch*) + sizeof(size_t)) / sizeof(wch);
const wstr1 wstr1Def = WStr1Default();

#ifdef __cplusplus
static const bool bIsPODStr1 = std::is_pod<str1>();
static const bool bIsPODWStr1 = std::is_pod<str1>();
static const bool bIsPODHashWStr1 = std::is_pod<str1>();
#endif


wstr1 WStr1Default()
{
	wstr1 str = WStr1Inited();
	WStr1MakeDefault(&str);
	return str;
}

wstr1 WStr1Inited()
{
	wstr1 str;
	_InitWStr1(&str);
	return str;
}

void InitWStr1(wstr1* str)
{
	if (NULL != str)
		_InitWStr1(str);
}

wstr1 MakeWStr1CH(const wch c, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeCH(&str, c);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1CS(const wch* pcs, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeCS(&str, pcs);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1CSBy(const wch* pcs, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeCSBy(&str, pcs, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1CSBy2(const wch* pcs, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeCSBy2(&str, pcs, s, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1CSFromPos(const wch* pcs, size_t s, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeCSFromPos(&str, pcs, s);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1Tmp(const wstr1 tmp, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeTmp(&str, tmp);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1(const wstr1* other, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1Make(&str, other);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1By(const wstr1* other, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeBy(&str, other, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1By2(const wstr1* other, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeBy2(&str, other, s, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1FromPos(const wstr1* other, size_t s, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1MakeFromPos(&str, other, s);
	SET_PTRVAL(pRet, ret);
	return str;
}

int32_t WStr1MakeDefault(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1Release(str);
}

int32_t _WStr1MakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	wstr1 strTmp;
	const int32_t ret = _WStr1PureMakeCSBy2(&strTmp, pcs, s, len);
	RET_ON_NEG(ret);
	return _WStr1Move(str, &strTmp);
}

int32_t _WStr1MakeCSFromPos(wstr1* str, const wch* pcs, size_t s, size_t te)
{
	wstr1 strTmp;
	const int32_t ret = _WStr1PureMakeCSFromPos(&strTmp, pcs, s, te);
	RET_ON_NEG(ret);
	return _WStr1Move(str, &strTmp);
}

int32_t WStr1MakeCH(wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1MakeCSBy2(str, &c, 0, 1);
}

int32_t WStr1MakeCS(wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1MakeCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1MakeCSBy(wstr1* str, const wch* pcs, size_t len)
{
	return WStr1MakeCSBy2(str, pcs, 0, len);
}

int32_t WStr1MakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1MakeCSBy2(str, pcs, s, len);
}

int32_t WStr1MakeCSFromPos(wstr1* str, const wch* pcs, size_t s)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1MakeCSFromPos(str, pcs, s, wcslen(pcs));
}

int32_t WStr1MakeTmp(wstr1* str, const wstr1 tmp)
{
	return WStr1Make(str, &tmp);
}

int32_t WStr1Make(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return _WStr1Release(str);
	return _WStr1MakeCSBy2(str, WSTR1_STR(other), 0, WSTR1_LEN(other));
}

int32_t WStr1MakeBy(wstr1* str, const wstr1* other, size_t len)
{
	return WStr1MakeBy2(str, other, 0, len);
}

int32_t WStr1MakeBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	const size_t lenOther = WSTR1_LEN(other);
	if (s >= lenOther || len > lenOther - s)
		return GRET_INVIDX;
	return _WStr1MakeCSBy2(str, WSTR1_STR(other), s, len);
}

int32_t WStr1MakeFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;
	return _WStr1MakeCSFromPos(str, WSTR1_STR(other), s, WSTR1_LEN(other));
}

int32_t WStr1PureMakeDefault(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1InitLen(str, 0);
}

int32_t _WStr1PureMakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	const int32_t ret = _WStr1InitLen(str, len);
	RET_ON_NEG(ret);
	memmove(WSTR1_STR(str), pcs + s, len * sizeof(wch));
	return GRET_SUCCEED;
}

int32_t _WStr1PureMakeCSFromPos(wstr1* str, const wch* pcs, size_t s, size_t te)
{
	if (s >= te)
		return GRET_INVIDX;
	const size_t lenMake = te - s;
	return _WStr1PureMakeCSBy2(str, pcs, s, lenMake);
}

int32_t WStr1PureMakeCH(wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1PureMakeCSBy2(str, &c, 0, 1);
}

int32_t WStr1PureMakeCS(wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureMakeCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1PureMakeCSBy(wstr1* str, const wch* pcs, size_t len)
{
	return WStr1PureMakeCSBy2(str, pcs, 0, len);
}

int32_t WStr1PureMakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureMakeCSBy2(str, pcs, s, len);
}

int32_t WStr1PureMakeCSFromPos(wstr1* str, const wch* pcs, size_t s)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureMakeCSFromPos(str, pcs, s, wcslen(pcs));
}

int32_t WStr1PureMakeTmp(wstr1* str, const wstr1 tmp)
{
	return WStr1PureMake(str, &tmp);
}

int32_t WStr1PureMake(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
	{
		_InitWStr1(str);
		return GRET_SUCCEED;
	}
	return _WStr1PureMakeCSBy2(str, WSTR1_STR(other), 0, WSTR1_LEN(other));
}

int32_t WStr1PureMakeBy(wstr1* str, const wstr1* other, size_t len)
{
	return WStr1PureMakeBy2(str, other, 0, len);
}

int32_t WStr1PureMakeBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	const size_t lenOther = WSTR1_LEN(other);
	if (s >= lenOther || len > lenOther - s)
		return GRET_INVIDX;
	return _WStr1PureMakeCSBy2(str, WSTR1_STR(other), s, len);
}

int32_t WStr1PureMakeFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	return _WStr1PureMakeCSFromPos(str, WSTR1_STR(other), s, WSTR1_LEN(other));
}

wstr1 CopyWStr1Tmp(const wstr1 tmp, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1CopyTmp(&str, tmp);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1(const wstr1* other, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1Copy(&str, other);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1By(const wstr1* other, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1CopyBy(&str, other, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1By2(const wstr1* other, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1CopyBy2(&str, other, s, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1FromPos(const wstr1* other, size_t s, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1CopyFromPos(&str, other, s);
	SET_PTRVAL(pRet, ret);
	return str;
}

int32_t _WStr1CopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	wstr1 strTmp;
	const int32_t ret = _WStr1PureCopyBy2(&strTmp, other, s, len);
	RET_ON_NEG(ret);
	return _WStr1Move(str, &strTmp);
}

int32_t WStr1CopyTmp(wstr1* str, const wstr1 tmp)
{
	return WStr1Copy(str, &tmp);
}

int32_t WStr1Copy(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return _WStr1Release(str);
	return _WStr1CopyBy2(str, other, 0, WSTR1_LEN(other));
}

int32_t WStr1CopyBy(wstr1* str, const wstr1* other, size_t len)
{
	return WStr1CopyBy2(str, other, 0, len);
}

int32_t WStr1CopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;
	return _WStr1CopyBy2(str, other, s, len);
}

int32_t WStr1CopyFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	const size_t lenOther = WSTR1_LEN(other);
	if (s >= lenOther)
		return GRET_INVIDX;

	const size_t lenCopy = lenOther - s;
	return _WStr1CopyBy2(str, other, s, lenCopy);
}

int32_t _WStr1PureCopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	const size_t lenOther = WSTR1_LEN(other);
	if (s >= lenOther || len > lenOther - s)
		return GRET_INVIDX;

	const STR0_STATUS status = (STR0_STATUS)(other->status);
	int32_t ret = 0;
	switch (status)
	{
	case STR0S_OWN:
		ret = _WStr1PureMakeCSBy2(str, WSTR1_STR(other), s, len);
		RET_ON_NEG(ret);
		if (!WSTR1_ISOWN(str))
			ret = _WStr1ToOwn(str);
		return ret;
	case STR0S_LOCAL:
		_WStr1ShallowCopy(str, other);
		WSTR1_MOVE(str, s, 0, len);
		WSTR1_STRAT(str, WSTR1_LEN(str)) = WSTRENDCHAR;
		return GRET_SUCCEED;
	case STR0S_REF:
		_WStr1ShallowCopy(str, other);
		str->s1 += s;
		str->len = len;
		return GRET_SUCCEED;
	case STR0S_NULL:
	default:
		return GRET_INVSTATUS;
	}
}

int32_t _WStr1PureCopyFromPos(wstr1* str, const wstr1* other, size_t s, size_t te)
{
	if (s >= te)
		return GRET_INVIDX;
	const size_t lenCopy = te - s;
	return _WStr1PureCopyBy2(str, other, s, lenCopy);
}

int32_t WStr1PureCopyTmp(wstr1* str, const wstr1 tmp)
{
	return WStr1PureCopy(str, &tmp);
}

int32_t WStr1PureCopy(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
	{
		_InitWStr1(str);
		return GRET_SUCCEED;
	}
	return _WStr1PureCopyBy2(str, other, 0, WSTR1_LEN(other));
}

int32_t WStr1PureCopyBy(wstr1* str, const wstr1* other, size_t len)
{
	return WStr1PureCopyBy2(str, other, 0, len);
}

int32_t WStr1PureCopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;
	return _WStr1PureCopyBy2(str, other, s, len);
}

int32_t WStr1PureCopyFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;
	return _WStr1PureCopyFromPos(str, other, s, WSTR1_LEN(other));
}

int32_t WStr1Move(wstr1* str, wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	return _WStr1Move(str, other);
}

wstr1 RefWStr1CS(wch* pcs, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1RefCS(&str, pcs);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1CSBy(wch* pcs, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1RefCSBy(&str, pcs, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1CSBy2(wch* pcs, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1RefCSBy2(&str, pcs, s, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1CSFromPos(wch* pcs, size_t s, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1RefCSFromPos(&str, pcs, s);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1(const wstr1* other, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1Ref(&str, other);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1By(const wstr1* other, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1RefBy(&str, other, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1By2(const wstr1* other, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1RefBy2(&str, other, s, len);
	SET_PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1FromPos(const wstr1* other, size_t s, int32_t* pRet)
{
	wstr1 str = WStr1Inited();
	const int32_t ret = WStr1RefFromPos(&str, other, s);
	SET_PTRVAL(pRet, ret);
	return str;
}

int32_t _WStr1RefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len)
{
	if (!WSTR1_ISREF(str)
		&& _PtrOverlapWCH(WSTR1_STR(str), WSTR1_LEN(str), pcs + s, len))
		return GRET_INVOP;

	const int32_t ret = _WStr1Release(str);
	RET_ON_NEG(ret);
	return _WStr1PureRefCSBy2(str, pcs, s, len);
}

int32_t _WStr1RefCSFromPos(wstr1* str, wch* pcs, size_t s, size_t te)
{
	if (s >= te)
		return GRET_INVIDX;
	const size_t lenRef = te - s;
	return _WStr1RefCSBy2(str, pcs, s, lenRef);
}

int32_t WStr1RefCS(wstr1* str, wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1RefCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1RefCSBy(wstr1* str, wch* pcs, size_t len)
{
	return WStr1RefCSBy2(str, pcs, 0, len);
}

int32_t WStr1RefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1RefCSBy2(str, pcs, s, len);
}

int32_t WStr1RefCSFromPos(wstr1* str, wch* pcs, size_t s)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1RefCSFromPos(str, pcs, s, wcslen(pcs));
}

int32_t WStr1Ref(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	return _WStr1RefCSBy2(str, (wch*)WSTR1_STR(other), 0, WSTR1_LEN(other));
}

int32_t WStr1RefBy(wstr1* str, const wstr1* other, size_t len)
{
	return WStr1RefBy2(str, other, 0, len);
}

int32_t WStr1RefBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	const size_t lenOther = WSTR1_LEN(other);
	if (s >= lenOther || len > lenOther - s)
		return GRET_INVIDX;
	return _WStr1RefCSBy2(str, (wch*)WSTR1_STR(other), s, len);
}

int32_t WStr1RefFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	const size_t lenOther = WSTR1_LEN(other);
	return _WStr1RefCSFromPos(str, (wch*)WSTR1_STR(other), s, lenOther);
}

int32_t _WStr1PureRefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len)
{
	str->s1 = pcs + s;
	str->len = len;
	str->status = STR0S_REF;
	return GRET_SUCCEED;
}

int32_t _WStr1PureRefCSFromPos(wstr1* str, wch* pcs, size_t s, size_t te)
{
	if (s >= te)
		return GRET_INVIDX;
	const size_t lenRef = te - s;
	return _WStr1PureRefCSBy2(str, pcs, s, lenRef);
}

int32_t WStr1Deref(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (!WSTR1_ISREF(str))
		return GRET_INVSTATUS;
	return _WStr1MakeCSBy2(str, WSTR1_STR(str), 0, WSTR1_LEN(str));
}

int32_t WStr1PureRefCS(wstr1* str, wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureRefCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1PureRefCSBy(wstr1* str, wch* pcs, size_t len)
{
	return WStr1PureRefCSBy2(str, pcs, 0, len);
}

int32_t WStr1PureRefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureRefCSBy2(str, pcs, s, len);
}

int32_t WStr1PureRefCSFromPos(wstr1* str, wch* pcs, size_t s)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureRefCSFromPos(str, pcs, s, wcslen(pcs));
}

int32_t WStr1PureRef(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	return _WStr1PureRefCSBy2(str, (wch*)WSTR1_STR(other), 0, WSTR1_LEN(other));
}

int32_t WStr1PureRefBy(wstr1* str, const wstr1* other, size_t len)
{
	return WStr1PureRefBy2(str, other, 0, len);
}

int32_t WStr1PureRefBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	return _WStr1PureRefCSBy2(str, (wch*)WSTR1_STR(other), s, len);
}

int32_t WStr1PureRefFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;
	return _WStr1PureRefCSFromPos(str, (wch*)WSTR1_STR(other), s, WSTR1_LEN(other));
}

int32_t WStr1Clear(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_NONE;
	return _WStr1Relen(str, 0);
}

int32_t WStr1ClearLazy(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_NONE;
	return _WStr1RelenLazy(str, 0);
}

int32_t WStr1Attach(wstr1* str, wch* pcs)
{
	if (NULL == str)
		return GRET_NULL;
	const int32_t ret = _WStr1Release(str);
	if (NULL == pcs)
		return GRET_SUCCEED;
	str->s1 = pcs;
	str->len = wcslen(pcs);
	str->status = STR0S_OWN;
	return GRET_SUCCEED;
}

int32_t WStr1Detach(wstr1* str, wch** ppcs)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
	{
		SET_PTRVAL(ppcs, NULL);
		return GRET_SUCCEED;
	}
	if (!WSTR1_ISOWN(str))
	{
		int ret = _WStr1ToOwn(str);
		RET_ON_NEG(ret);
	}
	SET_PTRVAL(ppcs, str->s1);
	const int32_t ret = _WStr1Abandon(str);
	return GRET_SUCCEED;
}

int32_t WStr1ReAttach(wstr1* str, wch* pcs, wch** ppcsOld)
{
	if (NULL == str)
		return GRET_NULL;
	int ret = WStr1Detach(str, ppcsOld);
	RET_ON_NEG(ret);
	return WStr1Attach(str, pcs);
}

int32_t WStr1Release(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1Release(str);
}

int32_t WStr1Abandon(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1Abandon(str);
}

int32_t WStr1ToOwn(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1ToOwn(str);
}

int32_t WStr1ToLocal(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1ToLocal(str);
}

int32_t WStr1Fit(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	if (!WSTR1_ISOWN(str))
		return GRET_NONE;

	return _WStr1Recap(str, WSTR1_LEN(str) + 1);
}

int32_t WStr1Relen(wstr1* str, size_t len, bool bLazy)
{
	if (NULL == str)
		return GRET_NULL;
	return bLazy ? _WStr1RelenLazy(str, len) : _WStr1Relen(str, len);
}

void _InitWStr1(wstr1* str)
{
	str->status = STR0S_NULL;
	//str->len = 0;
	//str->s1 = NULL;
}

void _WStr1ShallowCopy(wstr1* str, const wstr1* other)
{
	const int32_t ret = _WStr1Release(str);
	memmove(str, other, sizeof(wstr1));
}

int32_t _WStr1Move(wstr1* str, wstr1* other)
{
	// CHECK SAME OBJ
	if (str == other)
		return GRET_NONE;

	const int32_t ret = _WStr1Release(str);
	RET_ON_NEG(ret);
	memmove(str, other, sizeof(wstr1));
	_InitWStr1(other);
	return GRET_SUCCEED;
}

int32_t _WStr1InitCap(wstr1* str, size_t cap)
{
	if (cap > WSTR1_CAP0)
	{
		wch* const pn = (wch*)malloc(cap * sizeof(wch));
		if (NULL == pn)
			return GRET_MALLOC;
		str->s1 = pn;
		str->status = STR0S_OWN;
	}
	else
	{
		str->status = STR0S_LOCAL;
	}
	return GRET_SUCCEED;
}

int32_t _WStr1Recap(wstr1* str, size_t cap)
{
	if (WSTR1_ISREF(str))
		return GRET_INVSTATUS;
	if (cap > WSTR1_CAP0 && !WSTR1_ISOWN(str))
	{
		return _WStr1ToOwn(str, cap);
	}
	//else if (cap <= WSTR1_CAP0 && !WSTR1_ISLOCAL(str))
	//{
	//	return _WStr1ToLocal(str);
	//}
	else if (WSTR1_ISOWN(str))
	{
		wch* const pn = (wch*)realloc(str->s1, cap * sizeof(wch));
		if (NULL == pn)
			return GRET_MALLOC;
		str->s1 = pn;
		return GRET_SUCCEED;
	}
	else// LOCAL
		return GRET_INVSTATUS;
	return GRET_NONE;
}

int32_t _WStr1InitLen(wstr1* str, size_t len)
{
	const size_t cap = len + 1;
	if (cap < len)
		return GRET_OVERFLOW;

	const int32_t ret = _WStr1InitCap(str, cap);
	RET_ON_NEG(ret);
	if (WSTR1_ISLOCAL(str))
	{
		str->len0 = (int8_t)len;
	}
	else
	{
		str->len = len;
		WSTR1_STR(str)[len] = WSTRENDCHAR;
	}
	return GRET_SUCCEED;
}

int32_t _WStr1Relen(wstr1* str, size_t len)
{
	if (WSTR1_ISREF(str))
		return GRET_INVSTATUS;
	if (len > WSTR1_LEN0 && !WSTR1_ISOWN(str))
	{
		const size_t cap = len + 1;
		if (cap < len)
			return GRET_OVERFLOW;
		return _WStr1ToOwn(str, cap);
	}
	//else if (len <= WSTR1_LEN0 && !WSTR1_ISLOCAL(str))
	//{
	//	return _WStr1ToLocal(str);
	//}
	else if (WSTR1_ISOWN(str))
	{
		const size_t cap = len + 1;
		if (cap < len)
			return GRET_OVERFLOW;
		wch* const pn = (wch*)realloc(str->s1, cap * sizeof(wch));
		if (NULL == pn)
			return GRET_MALLOC;
		str->s1 = pn;
		str->len = len;
		return GRET_SUCCEED;
	}
	else if (WSTR1_ISLOCAL(str))
	{
		str->len0 = (int8_t)len;
		return GRET_SUCCEED;
	}
	return GRET_NONE;
}

int32_t _WStr1RelenLazy(wstr1* str, size_t len)
{
	const size_t lenOld = WSTR1_LEN(str);
	if (len > lenOld)
		return _WStr1Relen(str, len);
	if (!WSTR1_ISLOCAL(str))
		str -> len = len;
	WSTR1_STR(str)[len] = WSTRENDCHAR;
	return GRET_SUCCEED;
}

int32_t _WStr1ExtendBy(wstr1* str, size_t dLen)
{
	const size_t lenOld = WSTR1_LEN(str);
	const size_t lenNew = lenOld + dLen;
	if (lenNew < lenOld)
		return GRET_OVERFLOW;
	return _WStr1Relen(str, lenNew);
}

int32_t _WStr1ShrinkBy(wstr1* str, size_t dLen)
{
	const size_t lenOld = WSTR1_LEN(str);
	if (dLen > lenOld)
		return GRET_INVIDX;

	const size_t lenNew = lenOld - dLen;
	return _WStr1Relen(str, lenNew);
}

int32_t _WStr1ShrinkByLazy(wstr1* str, size_t dLen)
{
	const size_t lenOld = WSTR1_LEN(str);
	if (dLen > lenOld)
		return GRET_INVIDX;

	const size_t lenNew = lenOld - dLen;
	return _WStr1RelenLazy(str, lenNew);
}

int32_t _WStr1ToOwn(wstr1* str, size_t recap)
{
	if (WSTR1_ISNULL(str))
		return GRET_NULL;
	if (WSTR1_ISOWN(str))
		return GRET_NONE;

	const size_t lenOld = WSTR1_LEN(str);
	const size_t cap = (0 == recap) ? lenOld + 1 : recap;
	wch* const pn = (wch*)malloc(cap * sizeof(wch));
	if (NULL == pn)
		return GRET_MALLOC;
	const size_t relen = min(recap - 1, lenOld);
	memmove(pn, WSTR1_STR(str), relen * sizeof(wch));
	pn[relen] = WSTRENDCHAR;

	str->s1 = pn;
	str->len = relen;
	str->status = STR0S_OWN;
	return GRET_SUCCEED;
}

int32_t _WStr1ToLocal(wstr1* str)
{
	if (WSTR1_ISNULL(str))
		return GRET_NULL;
	if (WSTR1_ISLOCAL(str))
		return GRET_NONE;

	wch* const psOld = str->s1;
	const bool bOldOwn = WSTR1_ISOWN(str);
	const size_t lenOld = WSTR1_LEN(str);
	const size_t lenNew = min(lenOld , WSTR1_LEN0);
	memmove(str->s0, str->s1, lenNew * sizeof(wch));
	str->len0 = (int8_t)lenNew;
	str->status = STR0S_LOCAL;
	if (bOldOwn)
		safefree(psOld);

	return GRET_SUCCEED;
}

int32_t _WStr1Overlap(const wstr1* str, const wstr1* str2)
{
	return _PtrOverlapWCH(WSTR1_STR(str), WSTR1_LEN(str),
		WSTR1_STR(str2), WSTR1_LEN(str2));
}

int32_t _WStr1OverlapCS(const wstr1* str, const wch* pcs, size_t s, size_t len)
{
	return _PtrOverlapWCH(WSTR1_STR(str), WSTR1_LEN(str), pcs + s, len);
}

int32_t _WStr1Release(wstr1* str)
{
	if (WSTR1_ISNULL(str))
		return GRET_NONE;
	if (WSTR1_ISOWN(str))
		safefree(str->s1);
	_InitWStr1(str);
	return GRET_SUCCEED;
}

int32_t _WStr1Abandon(wstr1* str)
{
	if (WSTR1_ISNULL(str))
		return GRET_NONE;
	_InitWStr1(str);
	return GRET_SUCCEED;
}

const wch* WStr1Str(const wstr1* str)
{
	return (!WSTR1_ISNULLSTR(str)) ? WSTR1_STR(str) : NULL;
}

const wch* WStr1CStr(wstr1* str, int32_t* pRet)
{
	if (WSTR1_ISNULLSTR(str))
		return NULL;

	if (WSTR1_ISREF(str))
	{
		const int32_t ret = WStr1Deref(str);
		SET_PTRVAL(pRet, ret);
		if (ret < 0)
			return NULL;
	}

	return WSTR1_STR(str);
}

size_t WStr1Len(const wstr1* str)
{
	if (WSTR1_ISNULLSTR(str))
		return 0;
	return WSTR1_LEN(str);
}

bool WStr1IsNull0(const wstr1 str)
{
	return WSTR1_ISNULL(&str);
}

bool WStr1IsEmpty0(const wstr1 str)
{
	return !(WSTR1_ISNULL(&str)) && 0 == WSTR1_LEN(&str);
}

bool WStr1IsNullOrEmpty0(const wstr1 str)
{
	if (WStr1IsNull0(str))
		return true;
	return 0 == WSTR1_LEN(&str);
}

bool WStr1IsNull(const wstr1* str)
{
	if (NULL == str)
		return NULL;
	return WSTR1_ISNULL(str);
}

bool WStr1IsEmpty(const wstr1* str)
{
	if (NULL == str)
		return NULL;
	return !(WSTR1_ISNULL(str)) && (0 == WSTR1_LEN(str));
}

bool WStr1IsNullOrEmpty(const wstr1* str)
{
	if (WStr1IsNull(str))
		return true;
	return 0 == WSTR1_LEN(str);
}

int32_t WStr1IsNullP(const wstr1* str)
{
	if (NULL == str) return -1;
	return (WSTR1_ISNULL(str)) ? 1 : 0;
}

int32_t WStr1IsEmptyP(const wstr1* str)
{
	if (NULL == str) return -1;
	if (WSTR1_ISNULL(str)) return -2;
	return (0 == WSTR1_LEN(str)) ? 1 : 0;
}

int32_t WStr1IsNullOrEmptyP(const wstr1* str)
{
	if (NULL == str) return -1;
	return (WSTR1_ISNULL(str) || 0 == WSTR1_LEN(str)) ? 1 : 0;
}

int32_t WStr1Equals(const wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	const wch* const ps2 = WSTR1_STR(str2);
	return _WCSEqual(ps1, WSTR1_LEN(str), ps2, WSTR1_LEN(str2));
}

int32_t WStr1EqualsTmp(const wstr1 tmp1, const wstr1 tmp2)
{
	return WStr1Equals(&tmp1, &tmp2);
}

int32_t WStr1EqualsCH(const wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqual(ps1, WSTR1_LEN(str), &c, 1);
}

int32_t WStr1EqualsCS(const wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqual(ps1, WSTR1_LEN(str), pcs, wcslen(pcs));
}

int32_t WStr1EqualsCSBy(const wstr1* str, const wch* pcs, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqual(ps1, WSTR1_LEN(str), pcs, len);
}

int32_t WStr1EqualsCSBy2(const wstr1* str, const wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqual(ps1, WSTR1_LEN(str), pcs + s, len);
}

int32_t WStr1EqualsCSBy3(const wstr1* str, size_t s1, size_t len1, const wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const size_t lenStr1 = WSTR1_LEN(str);
	if (s1 >= lenStr1 || len1 > lenStr1 - s1)
		return GRET_INVIDX;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqual(ps1 + s1, len1, pcs + s, len);
}

int32_t WStr1EqualsTmpCSBy2(wstr1 tmp, const wch* pcs, size_t s, size_t len)
{
	if (NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULLO(tmp))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STRO(tmp);
	return _WCSEqual(ps1, WSTR1_LENO(tmp), pcs + s, len);
}

int32_t WStr1EqualsI(const wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	const wch* const ps2 = WSTR1_STR(str2);
	return _WCSEqualCI(ps1, WSTR1_LEN(str), ps2, WSTR1_LEN(str2));
}

int32_t WStr1EqualsITmp(const wstr1 tmp1, const wstr1 tmp2)
{
	return WStr1EqualsI(&tmp1, &tmp2);
}

int32_t WStr1EqualsICH(const wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqualCI(ps1, WSTR1_LEN(str), &c, 1);
}

int32_t WStr1EqualsICS(const wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqualCI(ps1, WSTR1_LEN(str), pcs, wcslen(pcs));
}

int32_t WStr1EqualsICSBy(const wstr1* str, const wch* pcs, size_t len2)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqualCI(ps1, WSTR1_LEN(str), pcs, len2);
}

int32_t WStr1EqualsICSBy2(const wstr1* str, const wch* pcs, size_t s2, size_t len2)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqualCI(ps1, WSTR1_LEN(str), pcs + s2, len2);
}

int32_t WStr1EqualsICSBy3(const wstr1* str, size_t s1, size_t len1, const wch* pcs, size_t s2, size_t len2)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const size_t lenStr1 = WSTR1_LEN(str);
	if (s1 >= lenStr1 || len1 > lenStr1 - s1)
		return GRET_INVIDX;

	const wch* const ps1 = WSTR1_STR(str);
	return _WCSEqualCI(ps1 + s1, len1, pcs + s2, len2);
}

int32_t WStr1EqualsITmpCSBy2(wstr1 tmp, const wch* pcs, size_t s2, size_t len2)
{
	if (NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULLO(tmp))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STRO(tmp);
	return _WCSEqualCI(ps1, WSTR1_LENO(tmp), pcs + s2, len2);
}

int32_t WStr1ToLower(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	wch* const ps = WSTR1_STR(str);
	const size_t len = WSTR1_LEN(str);
	for (size_t i = 0; i < len; ++i)
	{
		const wch dc = (L'a' - L'A');
		if (ps[i] >= 'A' && ps[i] <= 'Z')
			ps[i] = ps[i] + dc;
	}

	return GRET_SUCCEED;
}

int32_t WStr1ToUpper(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	wch* ps = WSTR1_STR(str);
	const size_t len = WSTR1_LEN(str);
	for (size_t i = 0; i < len; ++i)
	{
		const wch dc = (L'a' - L'A');
		if (ps[i] >= 'a' && ps[i] <= 'z')
			ps[i] = ps[i] - dc;
	}

	return GRET_SUCCEED;
}

int32_t WStr1Substr(wstr1* str, size_t len, const wstr1* strs)
{
	return WStr1MakeBy(str, strs, len);
}
int32_t WStr1SubstrBy(wstr1* str, size_t s, size_t len, const wstr1* strs)
{
	return WStr1MakeBy2(str, strs, s, len);
}
int32_t WStr1SubstrFromPos(wstr1* str, size_t s, const wstr1* strs)
{
	return WStr1MakeFromPos(str, strs, s);
}

int32_t _WStr1SubstrSelfBy(wstr1* str, size_t s, size_t len)
{
	const size_t lenOld = WSTR1_LEN(str);
	if (s >= lenOld || len > lenOld - s)
		return GRET_INVIDX;

	if (WSTR1_ISREF(str))
	{
		str->s1 += s;
		str->len = len;
		return GRET_SUCCEED;
	}

	WSTR1_MOVE(str, s, 0, len);
	return _WStr1RelenLazy(str, len);
}

int32_t WStr1SubstrSelf(wstr1* str, size_t len)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;
	return _WStr1SubstrSelfBy(str, 0, len);
}

int32_t WStr1SubstrSelfBy(wstr1* str, size_t s, size_t len)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;
	return _WStr1SubstrSelfBy(str, s, len);
}

int32_t WStr1SubstrSelfFromPos(wstr1* str, size_t s)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const size_t lenOld = WSTR1_LEN(str);
	if (s >= lenOld)
		return GRET_INVIDX;
	const size_t lenSub = lenOld - s;
	return _WStr1SubstrSelfBy(str, s, lenSub);
}

int32_t _WStr1RemoveBy(wstr1* str, size_t s, size_t len)
{
	const size_t lenStr = WSTR1_LEN(str);
	if (s >= lenStr || len > lenStr - s)
		return GRET_INVIDX;

	WSTR1_MOVE(str, s + len, s, (lenStr - len - s));

	return _WStr1ShrinkByLazy(str, len);
}

int32_t WStr1RemoveAt(wstr1* str, size_t i)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1RemoveBy(str, i, 1);
}

int32_t WStr1RemoveFrom(wstr1* str, size_t i)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1RemoveBy(str, i, WSTR1_LEN(str));
}

int32_t WStr1RemoveFront(wstr1* str, size_t n)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1RemoveBy(str, 0, n);
}

int32_t WStr1RemoveLast(wstr1* str, size_t n)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	const size_t len = WSTR1_LEN(str);
	const size_t s = (n >= len) ? 0 : len - n;
	const size_t nRemove = (n >= len) ? len : n;

	return _WStr1RemoveBy(str, s, nRemove);
}

int32_t WStr1RemoveBy(wstr1* str, size_t s, size_t len)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1RemoveBy(str, s, len);
}

int32_t _WStr1InsertCSBy2(wstr1* str, size_t pos, const wch* pcs, size_t s2, size_t len2)
{
	const size_t lenStr1 = WSTR1_LEN(str);
	if (pos > lenStr1)
		return GRET_INVIDX;

	const int32_t ret = _WStr1ExtendBy(str, len2);
	RET_ON_NEG(ret);

	pcs += s2;
	const size_t nMV0dst = lenStr1 - pos;
	WSTR1_MOVE(str, pos, pos + len2, nMV0dst);
	memmove(WSTR1_POS(str, pos), pcs, len2 * sizeof(wch));
	return GRET_SUCCEED;
}

int32_t _WStr1InsertCSBy2Checked(wstr1* str, size_t pos, const wch* pcs, size_t s2, size_t len2)
{
	const size_t lenStr1 = WSTR1_LEN(str);
	if (pos > lenStr1)
		return GRET_INVIDX;

	const int32_t ret = _WStr1ExtendBy(str, len2);
	RET_ON_NEG(ret);

	pcs += s2;

	wch* const pMV0dst = WSTR1_POS(str, pos + len2);
	const size_t nMV0dst = lenStr1 - pos;
	if (!_PtrOverlapWCH(pMV0dst, nMV0dst, pcs, len2))
	{
		WSTR1_MOVE(str, pos, pos + len2, nMV0dst);
		memmove(WSTR1_POS(str, pos), pcs, len2 * sizeof(wch));
	}
	else
	{
		wch* const pTmp = (wch*)malloc(len2 * sizeof(wch));
		if (NULL == pTmp)
			return GRET_MALLOC;
		memmove(pTmp, pcs, len2 * sizeof(wch));
		WSTR1_MOVE(str, pos, pos + len2, nMV0dst);
		memmove(WSTR1_POS(str, pos), pTmp, len2 * sizeof(wch));
		safefree(pTmp);
	}

	return GRET_SUCCEED;
}

int32_t WStr1InsertCH(wstr1* str, size_t pos, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1InsertCSBy2(str, pos, &c, 0, 1);
}

int32_t WStr1InsertTmp(wstr1* str, size_t pos, wstr1 tmp)
{
	return WStr1Insert(str, pos, &tmp);
}

int32_t WStr1Insert(wstr1* str, size_t pos, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	return _WStr1InsertCSBy2(str, pos, WSTR1_STR(str2), 0, WSTR1_LEN(str2));
}

int32_t WStr1InsertBy(wstr1* str, size_t pos, const wstr1* str2, size_t len)
{
	return WStr1InsertBy2(str, pos, str2, 0, len);
}

int32_t WStr1InsertBy2(wstr1* str, size_t pos, const wstr1* str2, size_t s2, size_t len2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(str2);
	if (s2 >= lenStr2 || len2 > lenStr2 - s2)
		return GRET_INVIDX;

	return _WStr1InsertCSBy2(str, pos, WSTR1_STR(str2), s2, len2);
}

int32_t WStr1InsertCS(wstr1* str, size_t pos, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1InsertCSBy2(str, pos, pcs, 0, wcslen(pcs));
}

int32_t WStr1InsertCSBy(wstr1* str, size_t pos, const wch* pcs, size_t len)
{
	return WStr1InsertCSBy2(str, pos, pcs, 0, len);
}

int32_t WStr1InsertCSBy2(wstr1* str, size_t pos, const wch* pcs, size_t s2, size_t len2)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1InsertCSBy2(str, pos, pcs, s2, len2);
}

int32_t WStr1Prepend(wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	return _WStr1PrependCSBy2(str, WSTR1_STR(str2), 0, WSTR1_LEN(str2));
}

int32_t WStr1PrependTmp(wstr1* str, wstr1 tmp)
{
	return WStr1Prepend(str, &tmp);
}

int32_t _WStr1PrependCSBy2(wstr1* str, const wch* pcs, size_t s2, size_t len2)
{
	const int32_t ret = _WStr1ExtendBy(str, len2);
	RET_ON_NEG(ret);
	pcs += s2;
	const size_t nMV0dst = WSTR1_LEN(str);
	WSTR1_MOVE(str, 0, len2, nMV0dst);
	memmove(WSTR1_POS(str, 0), pcs, len2 * sizeof(wch));
	return GRET_SUCCEED;
}

int32_t WStr1PrependCH(wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1PrependCSBy2(str, &c, 0, 1);
}

int32_t WStr1PrependBy(wstr1* str, const wstr1* str2, size_t len2)
{
	return WStr1PrependBy2(str, str2, 0, len2);
}

int32_t WStr1PrependBy2(wstr1* str, const wstr1* str2, size_t s2, size_t len2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(str2);
	if (s2 >= lenStr2 || len2 > lenStr2 - s2)
		return GRET_INVIDX;

	return _WStr1PrependCSBy2(str, WSTR1_STR(str2), s2, len2);
}

int32_t WStr1PrependCS(wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1PrependCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1PrependCSBy(wstr1* str, const wch* pcs, size_t len)
{
	return WStr1PrependCSBy2(str, pcs, 0, len);
}

int32_t WStr1PrependCSBy2(wstr1* str, const wch* pcs, size_t s2, size_t len2)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;
	return _WStr1PrependCSBy2(str, pcs, s2, len2);
}

int32_t _WStr1AddCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	const int32_t ret = _WStr1ExtendBy(str, len);
	RET_ON_NEG(ret);

	pcs += s;
	memmove(WSTR1_END(str), pcs, len * sizeof(wch));
	return GRET_SUCCEED;
}

int32_t WStr1AddCH(wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1AddCSBy2(str, &c, 0, 1);
}

int32_t WStr1Add(wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	return _WStr1AddCSBy2(str, WSTR1_STR(str2), 0, WSTR1_LEN(str2));
}

int32_t WStr1AddBy(wstr1* str, const wstr1* str2, size_t len)
{
	return WStr1AddBy2(str, str2, 0, len);
}

int32_t WStr1AddBy2(wstr1* str, const wstr1* str2, size_t s, size_t len)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(str2);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;

	const wch* const ps2 = WSTR1_STR(str2);
	return _WStr1AddCSBy2(str, ps2, s, len);
}

int32_t WStr1AddTmp(wstr1* str, wstr1 tmp)
{
	return WStr1Add(str, &tmp);
}

int32_t WStr1AddCS(wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1AddCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1AddCSBy(wstr1* str, const wch* pcs, size_t len)
{
	return WStr1AddCSBy2(str, pcs, 0, len);
}

int32_t WStr1AddCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return _WStr1AddCSBy2(str, pcs, s, len);
}

int32_t hashWStr_Def32(const wch* pcs, size_t len)
{
	int32_t hash = 0;
	const wch* const pe = pcs + len;
	for (const wch* pn = pcs; pn != pe; ++pn)
	{
		hash += *pn;
	}
	return hash;
}

int64_t hashWStr_Def64(const wch* pcs, size_t len)
{
	int64_t hash = 0;
	const wch* const pe = pcs + len;
	for (const wch* pn = pcs; pn != pe; ++pn)
	{
		hash += *pn;
	}
	return hash;
}

#pragma endregion STR1


#pragma region STR2


#pragma endregion STR2

static const size_t szHashWStr = sizeof(hashwstr);