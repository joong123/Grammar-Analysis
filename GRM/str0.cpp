#include "str0.h"

#include <malloc.h>
#include <wchar.h>

#ifdef __cplusplus
#include <type_traits>
#endif


#pragma region STR0

wstr0 MakeWStr0(const wch* p)
{
	if (NULL == p)
		return NULL;

	const size_t len = wcslen(p);

	return MakeWStr0By(p, len);
}

wstr0 MakeWStr0By(const wch* p, size_t len)
{
	if (NULL == p || len < 0)
		return NULL;

	if (0 == len)
		len = wcslen(p);

	const size_t cap = len + 1;
	//if (cap <= len)
	//	cap = len;
	
	wstr0 str = (wstr0)malloc(cap*sizeof(wch) + STR0HEADSIZE);
	if (NULL == str)
		return NULL;

	STR0_SETLENCAPSTATUS(str, len, cap, STR0_STATUS_OWN);
	_WSTR0_SETENDCHAR(str);
	memcpy(_STR0STRADDR(str), p, len * sizeof(wch));

	return str;
}

wstr0 CopyWStr0(cwstr0c str)
{
	if (NULL != str)
		return MakeWStr0By(STR0STRADDR(str), STR0LEN(str));
	else
		return NULL;
}

int32_t ReleaseWStr0(wstr0c str)
{
	if (NULL != str)
	{
		if (STR0ISOWN(str))
		{
			safefree(str);
			return 1;
		}
		else
			return 2;
	}
	else
		return 0;
}

#pragma endregion STR0


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
static const size_t szWStr1Local = (sizeof(wch*) + sizeof(size_t)) / sizeof(wch);
const wstr1 wstr1Def = WStr1Default();

#ifdef __cplusplus
static const bool bIsPODStr1 = std::is_pod<str1>();
static const bool bIsPODWStr1 = std::is_pod<str1>();
static const bool bIsPODHashWStr1 = std::is_pod<str1>();
#endif


wstr1 WStr1Default()
{
	wstr1 str;
	InitWStr1(&str);
	WStr1MakeDefault(&str);
	return str;
}

void InitWStr1(wstr1* str)
{
	if (NULL != str)
	{
		str->status = STR0_STATUS::STR0_STATUS_NULL;
		str->len = 0;
		str->p = NULL;
	}
}

wstr1 MakeWStr1CS(const wch* p, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeCS(&str, p);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1CSBy(const wch* p, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeCSBy(&str, p, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1CSBy2(const wch* p, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeCSBy2(&str, p, s, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1CSFromPos(const wch* p, size_t s, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeCSFromPos(&str, p, s);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1Tmp(const wstr1 tmp, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeTmp(&str, tmp);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1(const wstr1* other, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1Make(&str, other);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1By(const wstr1* other, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeBy(&str, other, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1By2(const wstr1* other, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeBy2(&str, other, s, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 MakeWStr1FromPos(const wstr1* other, size_t s, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1MakeFromPos(&str, other, s);
	PTRVAL(pRet, ret);
	return str;
}

int32_t WStr1MakeDefault(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1Relen(str, 0);
}

int32_t _WStr1MakeCSBy2(wstr1* str, const wch* p, size_t s, size_t len)
{
	wstr1 strTmp;
	const int32_t ret = _WStr1PureMakeCSBy2(&strTmp, p, s, len);
	RET_ON_NEG(ret);
	return _WStr1Move(str, &strTmp);
}

int32_t _WStr1MakeCSFromPos(wstr1* str, const wch* p, size_t s, size_t te)
{
	wstr1 strTmp;
	const int32_t ret = _WStr1PureMakeCSFromPos(&strTmp, p, s, te);
	RET_ON_NEG(ret);
	return _WStr1Move(str, &strTmp);
}

int32_t WStr1MakeCS(wstr1* str, const wch* p)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	return _WStr1MakeCSBy2(str, p, 0, wcslen(p));
}

int32_t WStr1MakeCSBy(wstr1* str, const wch* p, size_t len)
{
	// DO NOT CHECK WCSLEN(p)
	return WStr1MakeCSBy2(str, p, 0, len);
}

int32_t WStr1MakeCSBy2(wstr1* str, const wch* p, size_t s, size_t len)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	// DO NOT CHECK WCSLEN(p)
	return _WStr1MakeCSBy2(str, p, s, len);
}

int32_t WStr1MakeCSFromPos(wstr1* str, const wch* p, size_t s)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	return _WStr1MakeCSFromPos(str, p, s, wcslen(p));
}

int32_t WStr1MakeTmp(wstr1* str, const wstr1 tmp)
{
	return WStr1Make(str, &tmp);
}

int32_t WStr1Make(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	// DO NOT CHECK SAME OBJ
	//if (str == str2)
	//	return GRET_NONE;
	if (WSTR1_ISNULL(other))
		return WStr1Release(str);
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
	// DO NOT CHECK SAME OBJ
	//if (str == str2)
	//	return WStr1SubstrSelfBy(str, s, len);
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(other);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;
	return _WStr1MakeCSBy2(str, WSTR1_STR(other), s, len);
}

int32_t WStr1MakeFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	// DO NOT CHECK SAME OBJ
	//if (str == str2)
	//	return GRET_NONE;
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

int32_t _WStr1PureMakeCSBy2(wstr1* str, const wch* p, size_t s, size_t len)
{
	const int32_t ret = _WStr1InitLen(str, len);
	if (ret < 0)
		return ret;
	memmove(WSTR1_STR(str), p + s, len * sizeof(wch));
	return GRET_SUCCEED;
}

int32_t _WStr1PureMakeCSFromPos(wstr1* str, const wch* p, size_t s, size_t te)
{
	if (s >= te)
		return GRET_INVIDX;
	const size_t len = te - s;
	const int32_t ret = _WStr1InitLen(str, len);
	if (ret < 0)
		return ret;
	memmove(WSTR1_STR(str), p + s, len * sizeof(wch));
	return GRET_SUCCEED;
}

int32_t WStr1PureMakeCS(wstr1* str, const wch* p)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	return _WStr1PureMakeCSBy2(str, p, 0, wcslen(p));
}

int32_t WStr1PureMakeCSBy(wstr1* str, const wch* p, size_t len)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	// DO NOT CHECK wcslen(p)
	return _WStr1PureMakeCSBy2(str, p, 0, len);
}

int32_t WStr1PureMakeCSBy2(wstr1* str, const wch* p, size_t s, size_t len)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	// DO NOT CHECK wcslen(p)
	return _WStr1PureMakeCSBy2(str, p, s, len);
}

int32_t WStr1PureMakeCSFromPos(wstr1* str, const wch* p, size_t s)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	return _WStr1PureMakeCSFromPos(str, p, s, wcslen(p));
}

int32_t WStr1PureMakeTmp(wstr1* str, const wstr1 tmp)
{
	return WStr1PureMake(str, &tmp);
}

int32_t WStr1PureMake(wstr1* str, const wstr1* other)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	// DO NOT CHECK SAME OBJ
	//if (str == other)
	//	return GRET_INVSTATUS;
	if (WSTR1_ISNULL(other))
	{
		InitWStr1(str);
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
	// DO NOT CHECK SAME OBJ
	//if (str == other)
	//	return GRET_INVSTATUS;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(other);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;
	return _WStr1PureMakeCSBy2(str, WSTR1_STR(other), s, len);
}

int32_t WStr1PureMakeFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	// DO NOT CHECK SAME OBJ
	//if (str == other)
	//	return GRET_INVSTATUS;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	return _WStr1PureMakeCSFromPos(str, WSTR1_STR(other), s, WSTR1_LEN(other));
}

wstr1 CopyWStr1CS(const wch* pcs, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyCS(&str, pcs);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1CSBy(const wch* pcs, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyCSBy(&str, pcs, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1CSBy2(const wch* pcs, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyCSBy2(&str, pcs, s, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1CSFromPos(const wch* pcs, size_t s, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyCSFromPos(&str, pcs, s);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1Tmp(const wstr1 tmp, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyTmp(&str, tmp);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1(const wstr1* other, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1Copy(&str, other);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1By(const wstr1* other, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyBy(&str, other, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1By2(const wstr1* other, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyBy2(&str, other, s, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 CopyWStr1FromPos(const wstr1* other, size_t s, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1CopyFromPos(&str, other, s);
	PTRVAL(pRet, ret);
	return str;
}

int32_t WStr1CopyCS(wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1MakeCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1CopyCSBy(wstr1* str, const wch* pcs, size_t len)
{
	return WStr1CopyCSBy2(str, pcs, 0, len);
}

int32_t WStr1CopyCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1MakeCSBy2(str, pcs, s, len);
}

int32_t WStr1CopyCSFromPos(wstr1* str, const wch* pcs, size_t s)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1MakeCSFromPos(str, pcs, s, wcslen(pcs));
}

int32_t _WStr1CopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	const size_t lenStr2 = WSTR1_LEN(other);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;

	const STR0_STATUS status = other->status;
	int32_t ret = 0;
	switch (status)
	{
	case STR0_STATUS_OWN:
		ret = _WStr1MakeCSBy2(str, WSTR1_STR(other), s, len);
		RET_ON_NEG(ret);
		if (!WSTR1_ISOWN(str))
			ret = _WStr1ToOwn(str);
		return ret;
	case STR0_STATUS_LOCAL:
		_WStr1ShallowCopy(str, other);
		memmove(WSTR1_STR(str), WSTR1_STR(str) + s, len * sizeof(wch));
		return GRET_SUCCEED;
	case STR0_STATUS_REF:
		_WStr1ShallowCopy(str, other);
		str->p += s;
		str->len = len;
		return GRET_SUCCEED;
	case STR0_STATUS_NULL:
	default:
		return GRET_INVSTATUS;
	}
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
		return WStr1Release(str);

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

	// DO NOT CHECK REFMGR
	//if (_WStr1RefMgrd(other, str))
	//	return GRET_INVSTATUS;

	return _WStr1CopyBy2(str, other, s, len);
}

int32_t WStr1CopyFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	// DO NOT CHECK REFMGR
	//if (_WStr1RefMgrd(other, str))
	//	return GRET_INVSTATUS;

	const size_t lenOther = WSTR1_LEN(other);
	if (s >= lenOther)
		return GRET_INVIDX;
	return _WStr1CopyBy2(str, other, s, lenOther);
}

int32_t WStr1PureCopyCS(wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureMakeCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1PureCopyCSBy(wstr1* str, const wch* pcs, size_t len)
{
	return WStr1PureCopyCSBy2(str, pcs, 0, len);
}

int32_t WStr1PureCopyCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureMakeCSBy2(str, pcs, s, len);
}

int32_t WStr1PureCopyCSFromPos(wstr1* str, const wch* pcs, size_t s)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1PureMakeCSFromPos(str, pcs, s, wcslen(pcs));
}

int32_t _WStr1PureCopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len)
{
	const size_t lenStr2 = WSTR1_LEN(other);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;

	const STR0_STATUS status = other->status;
	int32_t ret = 0;
	switch (status)
	{
	case STR0_STATUS_OWN:
		ret = _WStr1PureMakeCSBy2(str, WSTR1_STR(other), s, len);
		RET_ON_NEG(ret);
		if (!WSTR1_ISOWN(str))
			ret = _WStr1ToOwn(str);
		return ret;
	case STR0_STATUS_LOCAL:
		_WStr1ShallowCopy(str, other);
		memmove(WSTR1_STR(str), WSTR1_STR(str) + s, len * sizeof(wch));
		return GRET_SUCCEED;
	case STR0_STATUS_REF:
		_WStr1ShallowCopy(str, other);
		str->p += s;
		str->len = len;
		return GRET_SUCCEED;
	case STR0_STATUS_NULL:
	default:
		return GRET_INVSTATUS;
	}
}

int32_t _WStr1PureCopyFromPos(wstr1* str, const wstr1* other, size_t s, size_t te)
{
	if (s >= te)
		return GRET_INVIDX;
	const size_t len = te - s;
	return _WStr1PureCopyBy2(str, other, s, len);
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
		return WStr1Release(str);

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

//int32_t WStr1MoveBy(wstr1* str, wstr1* other, size_t len)
//{
//	return WStr1MoveBy2(str, other, 0, len);
//}

//int32_t WStr1MoveBy2(wstr1* str, wstr1* other, size_t s, size_t len)
//{
//	const int32_t ret = WStr1Move(str, other);
//	if (ret < 0)
//		return ret;
//	return WStr1SubstrSelfBy(other, s, len);
//}

wstr1 RefWStr1CS(const wch* pcs, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefCS(&str, pcs);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1CSBy(const wch* pcs, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefCSBy(&str, pcs, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1CSBy2(const wch* pcs, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefCSBy2(&str, pcs, s, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1CSFromPos(const wch* pcs, size_t s, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefCSFromPos(&str, pcs, s);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1Tmp(const wstr1 tmp, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefTmp(&str, tmp);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1(const wstr1* other, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1Ref(&str, other);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1By(const wstr1* other, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefBy(&str, other, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1By2(const wstr1* other, size_t s, size_t len, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefBy2(&str, other, s, len);
	PTRVAL(pRet, ret);
	return str;
}

wstr1 RefWStr1FromPos(const wstr1* other, size_t s, int32_t* pRet)
{
	wstr1 str;
	InitWStr1(&str);
	const int32_t ret = WStr1RefFromPos(&str, other, s);
	PTRVAL(pRet, ret);
	return str;
}

int32_t _WStr1RefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;

	const int32_t ret = _WStr1Release(str);

	str->p = pcs + s;
	str->len = len;
	str->status = STR0_STATUS_REF;

	return GRET_SUCCEED;
}

int32_t _WStr1RefCSFromPos(wstr1* str, wch* pcs, size_t s, size_t te)
{
	if (s >= te)
		return GRET_INVIDX;
	const size_t len = te - s;

	const int32_t ret = _WStr1Release(str);

	str->p = pcs + s;
	str->len = len;
	str->status = STR0_STATUS_REF;

	return GRET_SUCCEED;
}

int32_t WStr1RefCS(wstr1* str, const wch* pcs)
{
	if (NULL == str || NULL == pcs)
		return GRET_NULL;
	return _WStr1RefCSBy2(str, pcs, 0, wcslen(pcs));
}

int32_t WStr1Ref(wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	// DO NOT CHECK REFMGR
	//if (_WStr1RefMgrd(str2, str))
	//	return GRET_INVSTATUS;

	int32_t ret = WStr1Release(str);

	str->p = str2->p;
	str->len = WSTR1_LEN(str2);
	str->status = STR0_STATUS_REF;

	return GRET_SUCCEED;
}

int32_t WStr1RefBy(wstr1* str, const wstr1* str2, size_t len)
{
	return WStr1RefBy2(str, str2, 0, len);
}

int32_t WStr1RefBy2(wstr1* str, const wstr1* str2, size_t s, size_t len)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	// DO NOT CHECK REFMGR
	//if (_WStr1RefMgrd(str2, str))
	//	return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(str2);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;

	const int32_t ret = WStr1Release(str);

	str->p = str2->p + s;
	str->len = len;
	str->status = STR0_STATUS_REF;

	return GRET_SUCCEED;
}

int32_t WStr1RefFromPos(wstr1* str, const wstr1* other, size_t s)
{
	if (NULL == str || NULL == other)
		return GRET_NULL;
	if (WSTR1_ISNULL(other))
		return GRET_INVSTATUS;

	// DO NOT CHECK REFMGR
	//if (_WStr1RefMgrd(other, str))
	//	return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(other);
	if (s >= lenStr2)
		return GRET_INVIDX;

	const int32_t ret = WStr1Release(str);

	const size_t len = lenStr2 - s;
	str->p = other->p + s;
	str->len = len;
	str->status = STR0_STATUS_REF;

	return GRET_SUCCEED;
}

int32_t WStr1Deref(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	if (!WSTR1_ISREF(str))
		return GRET_NONE;

	return _WStr1MakeCSBy2(str, str->p, 0, str->len);
}

int32_t WStr1Clear(wstr1* str)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_NONE;

	return _WStr1Relen(str, 0);
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
	if (WSTR1_ISNULL(str))
		return GRET_NONE;
	InitWStr1(str);
	return GRET_SUCCEED;
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

	return _WStr1Relen(str, WSTR1_LEN(str));
}

int32_t WStr1Relen(wstr1* str, size_t len)
{
	if (NULL == str)
		return GRET_NULL;
	return _WStr1Relen(str, len);
}

void _WStr1ShallowCopy(wstr1* str, const wstr1* str2)
{
	memmove(str, str2, sizeof(wstr1));
}

int32_t _WStr1Move(wstr1* str, wstr1* str2)
{
	// CHECK SAME OBJ
	if (str == str2)
		return GRET_NONE;

	// DO NOT CHECK REFMGR
	//if (_WStr1RefMgrd(str2, str))
	//	return GRET_INVSTATUS;

	const int32_t ret = WStr1Release(str);

	if (!WSTR1_ISLOCAL(str2))
	{
		str->len = str2->len;
		str->p = str2->p;
	}
	else
	{
		memmove(str->s0, str2->s0, WSTR1_LEN0 * sizeof(wch));
	}
	str->status = str2->status;

	InitWStr1(str2);

	return GRET_SUCCEED;
}

int32_t _WStr1InitCap(wstr1* str, size_t cap)
{
	if (cap > WSTR1_LEN0)
	{
		wch* const pn = (wch*)malloc(cap * sizeof(wch));
		if (NULL == pn)
			return GRET_MALLOC;
		str->p = pn;
		str->status = STR0_STATUS::STR0_STATUS_OWN;
	}
	else
	{
		str->status = STR0_STATUS::STR0_STATUS_LOCAL;
	}
	return GRET_SUCCEED;
}

int32_t _WStr1Recap(wstr1* str, size_t cap)
{
	if (cap > WSTR1_LEN0 && !WSTR1_ISOWN(str))
	{
		return _WStr1ToOwn(str, cap);
	}
	else if (cap <= WSTR1_LEN0 && !WSTR1_ISLOCAL(str))
	{
		return _WStr1ToLocal(str, cap);
	}
	else if (WSTR1_ISOWN(str))
	{
		wch* const pn = (wch*)realloc(str->p, cap * sizeof(wch));
		if (NULL == pn)
			return GRET_MALLOC;
		str->p = pn;
		return GRET_SUCCEED;
	}
	return GRET_NONE;
}

int32_t _WStr1InitLen(wstr1* str, size_t len)
{
	const size_t cap = len + 1;
	if (cap < len)
		return GRET_OVERFLOW;

	const int32_t ret = _WStr1InitCap(str, cap);
	RET_ON_NEG(ret);

	if (!WSTR1_ISLOCAL(str))
		str->len = len;
	WSTR1_STR(str)[len] = WSTRENDCHAR;

	return GRET_SUCCEED;
}

int32_t _WStr1Relen(wstr1* str, size_t len)
{
	const size_t cap = len + 1;
	if (cap < len)
		return GRET_OVERFLOW;

	const int32_t ret = _WStr1Recap(str, cap);
	if (ret < 0)
		return ret;

	if (!WSTR1_ISLOCAL(str))
		str->len = len;
	WSTR1_STR(str)[len] = WSTRENDCHAR;

	return GRET_SUCCEED;
}

int32_t _WStr1ExtendBy(wstr1* str, size_t dLen)
{
	const size_t len = WSTR1_LEN(str);
	const size_t lenNew = len + dLen;
	if (lenNew < len)
		return GRET_OVERFLOW;

	return _WStr1Relen(str, lenNew);
}

int32_t _WStr1ShrinkBy(wstr1* str, size_t dLen)
{
	const size_t len = WSTR1_LEN(str);
	const size_t lenNew = len > dLen ? len - dLen : 0;

	return _WStr1Relen(str, lenNew);
}

int32_t _WStr1ToOwn(wstr1* str, size_t recap)
{
	if (!WSTR1_ISOWN(str))
	{
		const bool bIsNull = WSTR1_ISNULL(str);
		const size_t lenOld = WSTR1_LENGOBAL(str);

		if (0 == recap)
			recap = lenOld + 1;

		wch* const pn = (wch*)malloc(recap * sizeof(wch));
		if (NULL == pn)
			return GRET_MALLOC;

		const size_t lenCopy = min(lenOld, recap - 1);
		if (!bIsNull)
			memmove(pn, WSTR1_STR(str), lenCopy * sizeof(wch));
		pn[lenCopy] = WSTRENDCHAR;// needed for REF

		//const size_t lenNew = recap - 1;
		str->p = pn;
		str->len = lenOld;// lenNew - recap do not change len
		str->status = STR0_STATUS::STR0_STATUS_OWN;

		return GRET_SUCCEED;
	}
	return GRET_NONE;
}

int32_t _WStr1ToLocal(wstr1* str, size_t recap)
{
	if (!WSTR1_ISLOCAL(str))
	{
		if (WSTR1_ISNULL(str))
		{
			const size_t lenOld = 0;
			str->s0[lenOld] = WSTRENDCHAR;
			str->status = STR0_STATUS::STR0_STATUS_LOCAL;
			return GRET_SUCCEED;
		}

		const size_t lenOld = WSTR1_LEN(str);
		if (0 == recap)
			recap = lenOld + 1;
		recap = min(recap, WSTR1_LEN0);
		const size_t lenCopy = min(lenOld, recap - 1);
		//const size_t lenNew = recap - 1;

		wch* const psOld = str->p;
		const bool bOldOwn = WSTR1_ISOWN(str);

		memmove(str->s0, str->p, lenCopy * sizeof(wch));
		str->s0[lenCopy] = WSTRENDCHAR;// needed
		//str->len = lenOld;// no need
		str->status = STR0_STATUS::STR0_STATUS_LOCAL;

		if (bOldOwn)
			safefree(psOld);

		return GRET_SUCCEED;
	}
	return GRET_NONE;
}

int32_t _WStr1Release(wstr1* str)
{
	if (WSTR1_ISNULL(str))
		return GRET_NONE;

	if (WSTR1_ISOWN(str))
	{
		safefree(str->p);
	}
	InitWStr1(str);
	return GRET_SUCCEED;
}

const wch* WStr1Str(const wstr1* str)
{
	return (!WSTR1_ISNULLP(str)) ? WSTR1_STR(str) : NULL;
}

const wch* WStr1CStr(wstr1* str, int32_t* pRet)
{
	if (WSTR1_ISNULLP(str))
		return NULL;

	if (WSTR1_ISREF(str))
	{
		const int32_t ret = WStr1Deref(str);
		PTRVAL(pRet, ret)
	}

	return WSTR1_STR(str);
}

size_t WStr1Len(const wstr1* str)
{
	if (WSTR1_ISNULLP(str))
		return 0;
	return WSTR1_LEN(str);
}

bool WStr1IsNull0(const wstr1 str)
{
	return WSTR1_ISNULL(&str);
}

bool WStr1IsEmpty0(const wstr1 str)
{
	return !(WSTR1_ISNULL(&str)) && (0 == WSTR1_LEN(&str));
}

bool WStr1IsNull(const wstr1* str)
{
	return NULL == str || WSTR1_ISNULL(str);
}

bool WStr1IsEmpty(const wstr1* str)
{
	return NULL != str && !(WSTR1_ISNULL(str)) && (0 == WSTR1_LEN(str));
}

bool WStr1IsNullOrEmpty(const wstr1* str)
{
	if (WStr1IsNull(str))
		return true;
	return (0 == WSTR1_LEN(str));
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
	return _WStrEquals(ps1, WSTR1_LEN(str), ps2, WSTR1_LEN(str2));
}

int32_t _WStrEquals(const wch* p1, size_t len, const wch* p2, size_t len2)
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

int32_t WStr1EqualsCS(const wstr1* str, const wch* p)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WStrEquals(ps1, WSTR1_LEN(str), p, wcslen(p));
}

int32_t WStr1EqualsCS2(const wstr1* str, const wch* p, size_t len)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WStrEquals(ps1, WSTR1_LEN(str), p, len);
}

int32_t WStr1EqualsCS3(const wstr1* str, const wch* p, size_t s, size_t len)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WStrEquals(ps1, WSTR1_LEN(str), p + s, len);
}

int32_t _WStrEqualsCI(const wch* p1, size_t len, const wch* p2, size_t len2)
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
			if ((c1 >= L'a' && c1 <= 'z' && c1 - dc == c2) ||
				(c1 >= 'A' && c1 <= 'Z' && c1 + dc == c2))
				return 0;
		}
	}
	return 1;
}

int32_t WStr1EqualsI(const wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	const wch* const ps2 = WSTR1_STR(str2);
	return _WStrEqualsCI(ps1, WSTR1_LEN(str), ps2, WSTR1_LEN(str2));
}

int32_t WStr1EqualsICS(const wstr1* str, const wch* p)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WStrEqualsCI(ps1, WSTR1_LEN(str), p, wcslen(p));
}

int32_t WStr1EqualsICS2(const wstr1* str, const wch* p, size_t len)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WStrEqualsCI(ps1, WSTR1_LEN(str), p, len);
}

int32_t WStr1EqualsICS3(const wstr1* str, const wch* p, size_t s, size_t len)
{
	if (NULL == str || NULL == p)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const wch* const ps1 = WSTR1_STR(str);
	return _WStrEqualsCI(ps1, WSTR1_LEN(str), p + s, len);
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

	wch* const ps = WSTR1_STR(str);
	const size_t len = WSTR1_LEN(str);
	for (size_t i = 0; i < len; ++i)
	{
		const wch dc = (L'a' - L'A');
		if (ps[i] >= 'a' && ps[i] <= 'z')
			ps[i] = ps[i] - dc;
	}

	return GRET_SUCCEED;
}

int32_t WStr1SubstrSelf(wstr1* str, size_t len)
{
	return WStr1SubstrSelfBy(str, 0, len);
}

int32_t WStr1SubstrSelfBy(wstr1* str, size_t s, size_t len)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str))
		return GRET_INVSTATUS;

	const size_t lenOld = WSTR1_LEN(str);
	if (s >= lenOld || len > lenOld - s)
		return GRET_INVIDX;

	if (WSTR1_ISREF(str))
	{
		str->p += s;
		str->len = len;
		return GRET_SUCCEED;
	}

	wch* const ps = WSTR1_STR(str);
	memmove(ps, ps + s, len * sizeof(wch));

	return _WStr1Relen(str, len);
}

int32_t WStr1RemoveAt(wstr1* str, size_t i)
{
	if (NULL == str)
		return GRET_NULL;

	return WStr1RemoveBy(str, i, 1);
}

int32_t WStr1RemoveFrom(wstr1* str, size_t i)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return WStr1RemoveBy(str, i, WSTR1_LEN(str));
}

int32_t WStr1RemoveFront(wstr1* str, size_t n)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	return WStr1RemoveBy(str, 0, n);
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

	return WStr1RemoveBy(str, s, nRemove);
}

int32_t WStr1RemoveBy(wstr1* str, size_t s, size_t len)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	const size_t lenStr = WSTR1_LEN(str);
	if (s >= lenStr)
		return GRET_INVIDX;
	len = min(len, lenStr - s);

	wch* const ps = WSTR1_STR(str);
	memmove(ps + s, ps + s + len, (lenStr - len - s) * sizeof(wch));

	return _WStr1ShrinkBy(str, len);
}

//int32_t _WStr1RemoveBy(wstr1* str, size_t s, size_t len)
//{
//	const size_t lenStr = WSTR1_LEN(str);
//	if (s >= lenStr)
//		return GRET_INVIDX;
//	len = min(len, lenStr - s);
//
//	wch* const ps = WSTR1_STR(str);
//	memmove(ps + s, ps + s + len, (lenStr - len - s) * sizeof(wch));
//
//	return GRET_SUCCEED;
//}

int32_t WStr1InsertCH(wstr1* str, wch c, size_t i)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	const size_t len = WSTR1_LEN(str);
	if (i > len)
		return GRET_INVIDX;

	const size_t nMov = (i >= len) ? 0 : len - i;

	const size_t lenNew = len + 1;
	if (lenNew < len)
		return GRET_OVERFLOW;

	const int32_t ret = WStr1Relen(str, lenNew);
	if (ret < 0)
		return ret;

	wch* const ps = WSTR1_STR(str);
	memmove(ps + i + 1, ps + i, nMov * sizeof(wch));
	ps[i] = c;

	return GRET_SUCCEED;
}

int32_t WStr1Insert(wstr1* str, const wstr1* str2, size_t i)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	return WStr1InsertBy2(str, str2, 0, WSTR1_LEN(str2), i);
}

int32_t WStr1InsertBy(wstr1* str, const wstr1* str2, size_t len, size_t i)
{
	return WStr1InsertBy2(str, str2, 0, len, i);
}

int32_t WStr1InsertBy2(wstr1* str, const wstr1* str2, size_t s, size_t len, size_t i)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(str2);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;
	if (i > WSTR1_LEN(str))
		return GRET_INVIDX;

	const wstr1* pstrFrom = str2;
	size_t sActual = s;
	wstr1 strTmp;
	const bool bOverlap = WSTR1_ISREF(str2) && _WStr1AddrOverlap(str, str2);
	if (bOverlap)
	{
		const int32_t ret = WStr1CopyBy2(&strTmp, str2, s, len);
		if (ret < 0)
			return ret;
		pstrFrom = &strTmp;
		sActual = 0;
	}

	const wch* const ps2 = WSTR1_STR(pstrFrom);
	const int32_t ret = WStr1InsertCSBy2(str, ps2, sActual, len, i);

	if (bOverlap)
		const int32_t ret2 = WStr1Release(&strTmp);

	return ret;
}

int32_t WStr1InsertCS(wstr1* str, const wch* p2, size_t i)
{
	if (NULL == str || NULL == p2)
		return GRET_NULL;

	return WStr1InsertCSBy2(str, p2, 0, wcslen(p2), i);
}

int32_t WStr1InsertCSBy(wstr1* str, const wch* p2, size_t len, size_t i)
{
	return WStr1InsertCSBy2(str, p2, 0, len, i);
}

int32_t WStr1InsertCSBy2(wstr1* str, const wch* p2, size_t s, size_t len, size_t i)
{
	if (NULL == str || NULL == p2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	const size_t lenStr = WSTR1_LEN(str);
	if (i > lenStr)
		return GRET_INVIDX;

	const int32_t ret = _WStr1ExtendBy(str, len);
	if (ret < 0)
		return ret;

	wch* const ps = WSTR1_STR(str);
	memmove(ps + i + len, ps + i, (lenStr - i) * sizeof(wch));
	memmove(ps + i, p2 + s, len * sizeof(wch));

	return GRET_SUCCEED;
}

int32_t WStr1PrependCH(wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	const size_t lenOld = WSTR1_LEN(str);
	const int32_t ret = _WStr1ExtendBy(str, 1);
	if (ret < 0)
		return ret;

	memmove(WSTR1_POS(str, 1), WSTR1_POS(str, 0), lenOld);

	return GRET_SUCCEED;
}

int32_t WStr1Prepend(wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	return WStr1PrependBy2(str, str2, 0, WSTR1_LEN(str2));
}

int32_t WStr1PrependBy(wstr1* str, const wstr1* str2, size_t len)
{
	return WStr1PrependBy2(str, str2, 0, len);
}

int32_t WStr1PrependBy2(wstr1* str, const wstr1* str2, size_t s, size_t len)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	const size_t lenStr2 = WSTR1_LEN(str2);
	if (s >= lenStr2 || len > lenStr2 - s)
		return GRET_INVIDX;

	const wch* const ps2 = WSTR1_STR(str2);
	return WStr1PrependCSBy2(str, ps2, s, len);
}

int32_t WStr1PrependCS(wstr1* str, const wch* p2)
{
	if (NULL == p2)
		return GRET_NULL;
	return WStr1InsertCSBy2(str, p2, 0, wcslen(p2), 0);
}

int32_t WStr1PrependCSBy(wstr1* str, const wch* p2, size_t len)
{
	return WStr1InsertCSBy2(str, p2, 0, len, 0);
}

int32_t WStr1PrependCSBy2(wstr1* str, const wch* p2, size_t s, size_t len)
{
	if (NULL == str || NULL == p2)
		return GRET_NULL;
	return WStr1InsertCSBy2(str, p2, s, len, 0);
}

int32_t WStr1AddCH(wstr1* str, wch c)
{
	if (NULL == str)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	const int32_t ret = _WStr1ExtendBy(str, 1);
	if (ret < 0)
		return ret;

	WSTR1_STR(str)[WSTR1_LEN(str) - 1] = c;

	return GRET_SUCCEED;
}

int32_t WStr1Add(wstr1* str, const wstr1* str2)
{
	if (NULL == str || NULL == str2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str) || WSTR1_ISNULL(str2))
		return GRET_INVSTATUS;

	return WStr1AddBy2(str, str2, 0, WSTR1_LEN(str2));
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
	return WStr1AddCSBy2(str, ps2, s, len);
}

int32_t WStr1AddCS(wstr1* str, const wch* p2)
{
	if (NULL == str || NULL == p2)
		return GRET_NULL;

	return WStr1AddCSBy2(str, p2, 0, wcslen(p2));
}

int32_t WStr1AddCSBy(wstr1* str, const wch* p2, size_t len)
{
	return WStr1AddCSBy2(str, p2, 0, len);
}

int32_t WStr1AddCSBy2(wstr1* str, const wch* p2, size_t s, size_t len)
{
	if (NULL == str || NULL == p2)
		return GRET_NULL;
	if (WSTR1_ISNULL(str) || WSTR1_ISREF(str))
		return GRET_INVSTATUS;

	const size_t lenOld = WSTR1_LEN(str);
	const int32_t ret = _WStr1ExtendBy(str, len);
	if (ret < 0)
		return ret;

	memmove(WSTR1_POS(str, lenOld), p2, len * sizeof(wch));

	return GRET_SUCCEED;
}

int32_t hashWStr_Def32(const wch* p, size_t len)
{
	int32_t hash = 0;
	const wch* const pe = p + len;
	for (const wch* pn = p; p != pe; ++pn)
	{
		hash += *pn;
	}
	return hash;
}

int64_t hashWStr_Def64(const wch* p, size_t len)
{
	int64_t hash = 0;
	const wch* const pe = p + len;
	for (const wch* pn = p; p != pe; ++pn)
	{
		hash += *pn;
	}
	return hash;
}

int32_t	_PtrOverlap(const char* const p1, size_t len1, const char* const p2, size_t len2)
{
	return (p2 < p1 + len1) && (p1 < p2 + len2);
}

#pragma endregion STR1


static const size_t szHashWStr = sizeof(hashwstr);