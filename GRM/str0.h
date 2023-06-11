#pragma once

#include "def0.h"
#include <string.h>


#define STRENDCHAR					('\0')
#define WSTRENDCHAR					(L'\0')

#define STR_ISNULL(status)			(STR0_STATUS::STR0_STATUS_NULL == (status))
#define STR_ISOWN(status)			(STR0_STATUS::STR0_STATUS_OWN == (status))
#define STR_ISLOCAL(status)			(STR0_STATUS::STR0_STATUS_LOCAL == (status))
#define STR_ISREF(status)			(STR0_STATUS::STR0_STATUS_REF == (status))


enum STR0_STATUS
{
	STR0_STATUS_NULL	= 0,
	STR0_STATUS_OWN		= 1,
	STR0_STATUS_LOCAL	= 2,
	STR0_STATUS_REF		= 3
};


int32_t	_PtrOverlap(const char* const p1, size_t len1, const char* const p2, size_t len2);
inline int32_t	_PtrOverlapWCH(const wch* p1, size_t len1, const wch* p2, size_t len2)
{
	return _PtrOverlap((char*)p1, len1 * sizeof(wch), (char*)p2, len2 * sizeof(wch));
}


#pragma region str0

#pragma endregion str0


#pragma region str01

typedef ch*					str0;
typedef wch*				wstr0;

typedef const ch*			cstr0;
typedef ch* const			str0c;
typedef const ch* const		cstr0c;
typedef ch*					itstr0;
typedef const ch*			citstr0;

typedef const wch*			cwstr0;
typedef wch* const			wstr0c;
typedef const wch* const	cwstr0c;
typedef wch*				itwstr0;
typedef const wch*			citwstr0;


typedef size_t				str0size;
typedef int32_t				str0status;


// Structure
#define STR0HEADUNIT				(8)
#define STR0SIZELEN					(max(sizeof(str0size), STR0HEADUNIT)) // 8
#define STR0SIZECAP					(max(sizeof(str0size), STR0HEADUNIT)) // 8
#define STR0SIZESTATUS				(max(sizeof(str0status), STR0HEADUNIT)) // 8
#define STR0HEADSIZE				((STR0SIZELEN) + (STR0SIZECAP) + (STR0SIZESTATUS))
#define STR0OFFLEN					(0)
#define STR0OFFCAP					(STR0SIZELEN)
#define STR0OFFSATUS				((STR0SIZELEN) + (STR0SIZECAP))

// Info
#define STR0LEN(str)				(*((str0size*)((int8_t*)(str) + (STR0OFFLEN))))
#define STR0CAP(str)				(*((str0size*)((int8_t*)(str) + (STR0OFFCAP))))
#define STR0STATUS(str)				(*((str0status*)((int8_t*)(str) + (STR0OFFSATUS))))
#define _STR0STRADDR(str)			((void*)((int8_t*)(str) + (STR0HEADSIZE)))
#define STR0_SETLEN(str, len)		((STR0LEN(str)) = (len))
#define STR0_SETCAP(str, cap)		((STR0CAP(str)) = (cap))
#define STR0_SETSTATUS(str, status)	((STR0STATUS(str)) = ((str0status)(status)))
#define STR0_SETLENCAPSTATUS(str, len, cap, status)	\
		{STR0_SETLEN((str), (len)); STR0_SETCAP((str), (cap)); STR0_SETSTATUS((str), (status)); }

// Status
#define STRISFULL(str)				((STR0LEN(str)) == (STR0CAP(str)))
#define STR0ISSTATUS(str, status)	(((str0status)(status)) == (STR0STATUS(str)))
#define STR0ISNULL(str)				(STR0ISSTATUS((str), STR0_STATUS_NULL))
#define STR0ISOWN(str)				(STR0ISSTATUS((str), STR0_STATUS_OWN))
#define STR0ISREF(str)				(STR0ISSTATUS((str), STR0_STATUS_REF))

#define _STR0ADDR(str, idx)			(((decltype(str))(_STR0STRADDR(str))) + (idx))
#define _STR0_SETENDCHAR(str)		(*(_STR0ADDR(str, STR0LEN(str))) = STRENDCHAR)
#define _WSTR0_SETENDCHAR(str)		(*(_STR0ADDR(str, STR0LEN(str))) = WSTRENDCHAR)

// Specification
#define STR0INITCAP					(4)
#define STR0EXTENDRATE				(1.5)

// Interface
#define STR0STRADDR(str)			(((decltype(str))(_STR0STRADDR(str))) + (0))


//wch*		Stra2w(const ch* p);
//ch*		Strw2a(const wch* p);
wch*		Stra2w(const ch* p, const size_t len, size_t* lenr);
ch*			Strw2a(const wch* p, const size_t len, size_t* lenr);

str0		MakeStr0(const ch* p);
str0		MakeStr0By(const ch* p, size_t len);
str0		MakeStr0Wide(const wch* p);
str0		MakeStr0WideBy(const wch* p, size_t len);
wstr0		Str02WStr0(cstr0c str);
ch*			Str0Str(str0 str);
itstr0		Str0Begin(str0 str);
itstr0		Str0End(str0 str);
itstr0		Str0Last(str0 str);
itstr0		Str0EndFront(str0 str);
citstr0		Str0BeginC(str0 str);
citstr0		Str0EndC(str0 str);
citstr0		Str0LastC(str0 str);
citstr0		Str0EndFrontC(str0 str);
str0		CopyStr0(cstr0c str);
//str0		CopyStr0By(cstr0c str, size_t start = 0, size_t len = -1);
int32_t		Str0Add(str0 str, ch c);
int32_t		Str0Cat(str0 str, cstr0c str2);
int32_t		Str0Remove(str0 str, size_t idx);
int32_t		Str0RemoveBy(str0 str, size_t idx, size_t len);
int32_t		ReleaseStr0(str0c str);
//str0		AttachStr0(cstr0c str);
//int		DetachStr0(str0c str);


wstr0		MakeWStr0Ascii(const ch* p);
wstr0		MakeWStr0AsciiBy(const ch* p, size_t len);
wstr0		MakeWStr0(const wch* p);
wstr0		MakeWStr0By(const wch* p, size_t len);
str0		WStr02Str0(cwstr0c str);
wch*		WStr0Str(wstr0 str);
itwstr0		WStr0Begin(wstr0 str);
itwstr0		WStr0End(wstr0 str);
itwstr0		WStr0Last(wstr0 str);
itwstr0		WStr0EndFront(wstr0 str);
citwstr0	WStr0BeginC(wstr0 str);
citwstr0	WStr0EndC(wstr0 str);
citwstr0	WStr0LastC(wstr0 str);
citwstr0	WStr0EndFrontC(wstr0 str);
wstr0		CopyWStr0(cwstr0c str);
//wstr0		CopyWStr0By(cwstr0c str, size_t start = 0, size_t len = -1);
int32_t		WStr0Add(wstr0 str, wch c);
int32_t		WStr0Cat(wstr0 str, cwstr0c str2);
int32_t		WStr0Remove(wstr0 str, size_t idx);
int32_t		WStr0RemoveBy(wstr0 str, size_t idx, size_t len);
int32_t		ReleaseWStr0(wstr0c str);
//wstr0		AttachWStr0(cwstr0c str);
//int		DetachWStr0(wstr0c str);

#pragma endregion str01


#pragma region str1


// ***** TODO List: *****
// 1. malloc 0
// 2. use memmove in assign?
// 3. _InitWStr1


#define WSTR1_LEN0						8
#define WSTR1_ISNULLP(pwstr)			(NULL == (pwstr) || STR_ISNULL((pwstr)->status))
#define WSTR1_ISNULL(pwstr)				(STR_ISNULL((pwstr)->status))
#define WSTR1_ISOWN(pwstr)				(STR_ISOWN((pwstr)->status))
#define WSTR1_ISLOCAL(pwstr)			(STR_ISLOCAL((pwstr)->status))
#define WSTR1_ISREF(pwstr)				(STR_ISREF((pwstr)->status))
#define WSTR1_STR(pwstr)				(WSTR1_ISLOCAL(pwstr) ? (pwstr)->s0 : (pwstr)->p)
#define WSTR1_STROFSTATUS(wstr, status)	(!STR_ISLOCAL(status) ? (wstr).s0 : (wstr).p)
#define WSTR1_LEN(pwstr)				(WSTR1_ISLOCAL(pwstr) ? wcslen((pwstr)->s0) : (pwstr)->len)
#define WSTR1_LENGOBAL(pwstr)			(WSTR1_ISNULL(pwstr) ? 0 : WSTR1_LEN(pwstr))
#define WSTR1_SETLEN(pwstr, len)		(if (!WSTR1_ISLOCAL(pwstr)) (pwstr)->len = len;)
#define WSTR1_POS(pwstr, idx)			(WSTR1_STR(pwstr) + (idx))


// str1
typedef struct string1
{
	STR0_STATUS		status;
	union
	{
		struct {
			ch*		p;
			size_t	len;
		};
		ch			s0[8];
	};
} str1;

typedef struct wstring1
{
	STR0_STATUS		status;
	union {
		struct {
			wch*	p;
			size_t	len;
		};
		wch			s0[8];
	};
	// bool			bW;
} wstr1;


int32_t _WStrEquals(const wch* const p1, size_t len, const wch* const p2, size_t len2);
int32_t _WStrEqualsCI(const wch* const p1, size_t len, const wch* const p2, size_t len2);

// make (#22)
wstr1		WStr1Default();// default string
wstr1		MakeWStr1CS(const wch* pcs, int32_t* pRet = NULL);
wstr1		MakeWStr1CSBy(const wch* pcs, size_t len, int32_t* pRet = NULL);
wstr1		MakeWStr1CSBy2(const wch* pcs, size_t s, size_t len, int32_t* pRet = NULL);
wstr1		MakeWStr1CSFromPos(const wch* pcs, size_t s, int32_t* pRet = NULL);
wstr1		MakeWStr1Tmp(const wstr1 tmp, int32_t* pRet = NULL);
wstr1		MakeWStr1(const wstr1* other, int32_t* pRet = NULL);
wstr1		MakeWStr1By(const wstr1* other, size_t len, int32_t* pRet = NULL);
wstr1		MakeWStr1By2(const wstr1* other, size_t s, size_t len, int32_t* pRet = NULL);
wstr1		MakeWStr1FromPos(const wstr1* other, size_t s, int32_t* pRet = NULL);
int32_t		WStr1MakeDefault(wstr1* str);
int32_t		_WStr1MakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		_WStr1MakeCSFromPos(wstr1* str, const wch* pcs, size_t s, size_t te);
int32_t		WStr1MakeCS(wstr1* str, const wch* pcs);
int32_t		WStr1MakeCSBy(wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1MakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1MakeCSFromPos(wstr1* str, const wch* pcs, size_t s);
int32_t		WStr1MakeTmp(wstr1* str, const wstr1 tmp);
int32_t		WStr1Make(wstr1* str, const wstr1* other);
int32_t		WStr1MakeBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1MakeBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1MakeFromPos(wstr1* str, const wstr1* other, size_t s);
// pure make (#12)
int32_t		WStr1PureMakeDefault(wstr1* str);
int32_t		_WStr1PureMakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		_WStr1PureMakeCSFromPos(wstr1* str, const wch* pcs, size_t s, size_t te);
int32_t		WStr1PureMakeCS(wstr1* str, const wch* pcs);
int32_t		WStr1PureMakeCSBy(wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1PureMakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1PureMakeCSFromPos(wstr1* str, const wch* pcs, size_t s);
int32_t		WStr1PureMakeTmp(wstr1* str, const wstr1 tmp);
int32_t		WStr1PureMake(wstr1* str, const wstr1* other);
int32_t		WStr1PureMakeBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1PureMakeBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1PureMakeFromPos(wstr1 * str, const wstr1* other, size_t s);
// copy (#19)
wstr1		CopyWStr1CS(const wch* pcs, int32_t* pRet = NULL);
wstr1		CopyWStr1CSBy(const wch* pcs, size_t len, int32_t* pRet = NULL);
wstr1		CopyWStr1CSBy2(const wch* pcs, size_t s, size_t len, int32_t* pRet = NULL);
wstr1		CopyWStr1CSFromPos(const wch* pcs, size_t s, int32_t* pRet = NULL);
wstr1		CopyWStr1Tmp(const wstr1 tmp, int32_t* pRet = NULL);
wstr1		CopyWStr1(const wstr1* str, int32_t* pRet = NULL);
wstr1		CopyWStr1By(const wstr1* str, size_t len, int32_t* pRet = NULL);
wstr1		CopyWStr1By2(const wstr1* str, size_t s, size_t len, int32_t* pRet = NULL);
wstr1		CopyWStr1FromPos(const wstr1* str, size_t s, int32_t* pRet = NULL);
int32_t		WStr1CopyCS(wstr1* str, const wch* pcs);
int32_t		WStr1CopyCSBy(wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1CopyCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1CopyCSFromPos(wstr1* str, const wch* pcs, size_t s);
int32_t		_WStr1CopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1CopyTmp(wstr1* str, const wstr1 tmp);
int32_t		WStr1Copy(wstr1* str, const wstr1* other);
int32_t		WStr1CopyBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1CopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1CopyFromPos(wstr1* str, const wstr1* other, size_t s);
// pure copy (#11)
int32_t		WStr1PureCopyCS(wstr1* str, const wch* pcs);
int32_t		WStr1PureCopyCSBy(wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1PureCopyCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1PureCopyCSFromPos(wstr1* str, const wch* pcs, size_t s);
int32_t		_WStr1PureCopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		_WStr1PureCopyFromPos(wstr1* str, const wstr1* other, size_t s, size_t te);
int32_t		WStr1PureCopyTmp(wstr1* str, const wstr1 tmp);
int32_t		WStr1PureCopy(wstr1* str, const wstr1* other);
int32_t		WStr1PureCopyBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1PureCopyBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1PureCopyFromPos(wstr1* str, const wstr1* other, size_t s);
// move (#1)
int32_t		WStr1Move(wstr1* str, wstr1* other);
//int32_t		WStr1MoveBy(wstr1* str, wstr1* other, size_t len);
//int32_t		WStr1MoveBy2(wstr1* str, wstr1* other, size_t s, size_t len);
// ref (#20)
wstr1		RefWStr1CS(wch* pcs, int32_t* pRet = NULL);
wstr1		RefWStr1CSBy(wch* pcs, size_t len, int32_t* pRet = NULL);
wstr1		RefWStr1CSBy2(wch* pcs, size_t s, size_t len, int32_t* pRet = NULL);
wstr1		RefWStr1CSFromPos(wch* pcs, size_t s, int32_t* pRet = NULL);
wstr1		RefWStr1Tmp(const wstr1 tmp, int32_t* pRet = NULL);
wstr1		RefWStr1(const wstr1* str, int32_t* pRet = NULL);
wstr1		RefWStr1By(const wstr1* str, size_t len, int32_t* pRet = NULL);
wstr1		RefWStr1By2(const wstr1* str, size_t s, size_t len, int32_t* pRet = NULL);
wstr1		RefWStr1FromPos(const wstr1* str, size_t s, int32_t* pRet = NULL);
int32_t		_WStr1RefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len);
int32_t		_WStr1RefCSFromPos(wstr1* str, wch* pcs, size_t s, size_t te);
int32_t		WStr1RefCS(wstr1* str, wch* pcs);
int32_t		WStr1RefCSBy(wstr1* str, wch* pcs, size_t len);
int32_t		WStr1RefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len);
int32_t		WStr1RefCSFromPos(wstr1* str, wch* pcs, size_t s);
int32_t		WStr1RefTmp(wstr1* str, const wstr1 tmp);
int32_t		WStr1Ref(wstr1* str, const wstr1* other);
int32_t		WStr1RefBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1RefBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1RefFromPos(wstr1* str, const wstr1* other, size_t s);
// pure ref (#11)
int32_t		_WStr1PureRefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len);
int32_t		_WStr1PureRefCSFromPos(wstr1* str, wch* pcs, size_t s, size_t te);
int32_t		WStr1PureRefCS(wstr1* str, wch* pcs);
int32_t		WStr1PureRefCSBy(wstr1* str, wch* pcs, size_t len);
int32_t		WStr1PureRefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len);
int32_t		WStr1PureRefCSFromPos(wstr1* str, wch* pcs, size_t s);
int32_t		WStr1PureRefTmp(wstr1* str, const wstr1 tmp);
int32_t		WStr1PureRef(wstr1* str, const wstr1* other);
int32_t		WStr1PureRefBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1PureRefBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1PureRefFromPos(wstr1* str, const wstr1* other, size_t s);
// lifecycle (#7)
void		InitWStr1(wstr1* str);
int32_t		WStr1Deref(wstr1* str);
int32_t		WStr1Clear(wstr1* str);
int32_t		WStr1Attach(wstr1* str, wch* pcs);
int32_t		WStr1Detach(wstr1* str, wch** ppcs);
int32_t		WStr1Release(wstr1* str);
int32_t		WStr1Abandon(wstr1* str);
// store (#4)
int32_t		WStr1ToOwn(wstr1* str);
int32_t		WStr1ToLocal(wstr1* str);
int32_t		WStr1Fit(wstr1* str);
int32_t		WStr1Relen(wstr1* str, size_t len);
// internal (#15)
void		_WStr1ShallowCopy(wstr1* str, const wstr1* other);
int32_t		_WStr1Move(wstr1* str, wstr1* other);
int32_t		_WStr1InitCap(wstr1* str, size_t cap);
int32_t		_WStr1Recap(wstr1* str, size_t cap);
int32_t		_WStr1InitLen(wstr1* str, size_t len);
int32_t		_WStr1Relen(wstr1 * str, size_t len);
int32_t		_WStr1ExtendBy(wstr1 * str, size_t dLen);
int32_t		_WStr1ShrinkBy(wstr1 * str, size_t dLen);
int32_t		_WStr1ToOwn(wstr1 * str, size_t recap = 0);
int32_t		_WStr1ToLocal(wstr1 * str, size_t recap = 0);
int32_t		_WStr1AddrOverlap(const wstr1* str, const wstr1* str2);
int32_t		_WStr1AddrOverlapCS(const wstr1* str, const wch* pcs, size_t len);
bool		_WStr1RefMgrd(const wstr1* str, const wstr1* str2);
int32_t		_WStr1Release(wstr1* str);
int32_t		_WStr1Abandon(wstr1* str);
// info (#12)
const wch*	WStr1Str(const wstr1* str);
const wch*	WStr1CStr(wstr1* str, int32_t* pRet = NULL);// ToOwn if REF
size_t		WStr1Len(const wstr1* str);// 0 for null
bool		WStr1IsNull0(const wstr1 str);
bool		WStr1IsEmpty0(const wstr1 str);
bool		WStr1IsNull(const wstr1* str);
bool		WStr1IsEmpty(const wstr1* str);
bool		WStr1IsNullOrEmpty(const wstr1* str);
int32_t		WStr1IsNullP(const wstr1* str);// < 0 for null
int32_t		WStr1IsEmptyP(const wstr1* str);// < 0 for null
int32_t		WStr1IsNullOrEmptyP(const wstr1* str);// < 0 for null
bool		WStr1RefMgrd(const wstr1* str, const wstr1* str2);
// judge (#22)
int32_t		WStr1EqualsTmp(const wstr1 tmp1, const wstr1 tmp2);
int32_t		WStr1Equals(const wstr1* str, const wstr1* str2);
int32_t		WStr1EqualsCS(const wstr1* str, const wch* pcs);
int32_t		WStr1EqualsCS2(const wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1EqualsCS3(const wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1EqualsITmp(const wstr1 tmp1, const wstr1 tmp2);
int32_t		WStr1EqualsI(const wstr1* str, const wstr1* str2);
int32_t		WStr1EqualsICS(const wstr1* str, const wch* pcs);
int32_t		WStr1EqualsICS2(const wstr1* str, const wch*, size_t len);
int32_t		WStr1EqualsICS3(const wstr1* str, const wch*, size_t s, size_t len);
int32_t		WStr1EqualsATmp(const wstr1 tmp1, const str1 tmp2);
int32_t		WStr1EqualsA(const wstr1* str, const str1* str2);
int32_t		WStr1EqualsAC(const wstr1* str, const ch* pcs);
int32_t		WStr1EqualsAC2(const wstr1* str, const ch* pcs, size_t len);
int32_t		WStr1EqualsAC3(const wstr1* str, const ch* pcs, size_t s, size_t len);
int32_t		WStr1EqualsIATmp(const wstr1 tmp1, const str1 tmp2);
int32_t		WStr1EqualsIA(const wstr1* str, const str1* str2);
int32_t		WStr1EqualsIACS(const wstr1* str, const wch* pcs);
int32_t		WStr1EqualsIACS2(const wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1EqualsIACS3(const wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1ToLower(wstr1* str);
int32_t		WStr1ToUpper(wstr1* str);
// operation (#76)
int32_t		WStr1Substr(const wstr1* strs, size_t len, wstr1* strd);
int32_t		WStr1SubstrBy(const wstr1* strs, size_t s, size_t len, wstr1* strd);
int32_t		WStr1SubstrFromPos(const wstr1* strs, size_t s, wstr1* strd);
int32_t		_WStr1SubstrSelfBy(wstr1* str, size_t s, size_t len);
int32_t		WStr1SubstrSelf(wstr1* str, size_t len);
int32_t		WStr1SubstrSelfBy(wstr1* str, size_t s, size_t len);
int32_t		WStr1SubstrSelfFromPos(wstr1* str, size_t s);
int32_t		_WStr1RemoveBy(wstr1* str, size_t s, size_t len);
int32_t		WStr1RemoveAt(wstr1* str, size_t i);
int32_t		WStr1RemoveFrom(wstr1* str, size_t i);
int32_t		WStr1RemoveFront(wstr1* str, size_t n);
int32_t		WStr1RemoveLast(wstr1* str, size_t n);
int32_t		WStr1RemoveBy(wstr1* str, size_t s, size_t len);
int32_t		WStr1RemoveCH(wstr1* str, wch c);
int32_t		WStr1RemoveCS(wstr1* str, const wch* pcs);
int32_t		WStr1RemoveTmp(wstr1* str, wstr1 tmp);
int32_t		WStr1Remove(wstr1* str, wstr1* str2);
int32_t		WStr1Remove1CH(wstr1* str, wch c, size_t pos, size_t* posRemain = NULL);
int32_t		WStr1Remove1CS(wstr1* str, const wch* pcs, size_t pos, size_t* posRemain = NULL);
int32_t		WStr1Remove1Tmp(wstr1* str, wstr1 tmp, size_t pos, size_t* posRemain = NULL);
int32_t		WStr1Remove1(wstr1* str, wstr1* str2, size_t pos, size_t* posRemain = NULL);
int32_t		WStr1InsertCH(wstr1* str, wch c, size_t i);
int32_t		WStr1InsertTmp(wstr1* str, wstr1 tmp, size_t i);
int32_t		_WStr1InsertBy2(wstr1* str, const wstr1* str2, size_t s, size_t len, size_t i);
int32_t		WStr1Insert(wstr1* str, const wstr1* str2, size_t i);
int32_t		WStr1InsertBy(wstr1* str, const wstr1* str2, size_t len, size_t i);
int32_t		WStr1InsertBy2(wstr1* str, const wstr1* str2, size_t s, size_t len, size_t i);
int32_t		WStr1InsertCS(wstr1* str, const wch* pcs, size_t i);
int32_t		WStr1InsertCSBy(wstr1* str, const wch* pcs, size_t len, size_t i);
int32_t		WStr1InsertCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len, size_t i);
int32_t		WStr1PrependCH(wstr1* str, wch c);
int32_t		WStr1PrependTmp(wstr1* str, wstr1 tmp);
int32_t		WStr1Prepend(wstr1* str, const wstr1* str2);
int32_t		WStr1PrependBy(wstr1* str, const wstr1* str2, size_t len);
int32_t		_WStr1PrependBy2(wstr1* str, const wstr1* str2, size_t s, size_t len);
int32_t		WStr1PrependBy2(wstr1* str, const wstr1* str2, size_t s, size_t len);
int32_t		WStr1PrependCS(wstr1* str, const wch* pcs);
int32_t		WStr1PrependCSBy(wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1PrependCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1AddCH(wstr1* str, wch c);
int32_t		WStr1AddTmp(wstr1* str, wstr1 tmp);
int32_t		_WStr1AddCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1Add(wstr1* str, const wstr1* str2);
int32_t		WStr1AddBy(wstr1* str, const wstr1* str2, size_t len);
int32_t		WStr1AddBy2(wstr1* str, const wstr1* str2, size_t s, size_t len);
int32_t		WStr1AddCS(wstr1* str, const wch* pcs);
int32_t		WStr1AddCSBy(wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1AddCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);

int32_t		WStr1Trim(wstr1* str);
int32_t		WStr1TrimCHs(wstr1* str, const wch* pcs);
int32_t		WStr1ContainsCH(const wstr1* str, wch c);
int32_t		WStr1ContainsCHs(const wstr1* str, const wch* pcs);
int32_t		WStr1ContainsCS(const wstr1* str, const wch* pcs);
int32_t		WStr1ContainsCSBy(const wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1ContainsTmp(const wstr1* str, wstr1 tmp);
int32_t		WStr1Contains(const wstr1* str, const wstr1* str2);
int32_t		WStr1ContainsBy(const wstr1* str, const wstr1* str2, size_t s, size_t len);
int32_t		WStr1StartWithCH(const wstr1* str, wch c);
int32_t		WStr1StartWithCHs(const wstr1* str, const wch* pcs);
int32_t		WStr1StartWithCS(const wstr1* str, const wch* pcs);
int32_t		WStr1StartWithCSBy(const wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1StartWithTmp(const wstr1* str, wstr1 tmp);
int32_t		WStr1StartWith(const wstr1* str, const wstr1* str2);
int32_t		WStr1StartWithBy(const wstr1* str, const wstr1* str2, size_t s, size_t len);
int32_t		WStr1EndWithCH(const wstr1* str, wch c);
int32_t		WStr1EndWithCHs(const wstr1* str, const wch* pcs);
int32_t		WStr1EndWithCS(const wstr1* str, const wch* pcs);
int32_t		WStr1EndWithCSBy(const wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1EndWithTmp(const wstr1* str, wstr1 tmp);
int32_t		WStr1EndWith(const wstr1* str, const wstr1* str2);
int32_t		WStr1EndWithBy(const wstr1* str, const wstr1* str2, size_t s, size_t len);
int32_t		WStr1FindCH(const wstr1* str, wch c, size_t s, size_t* pIdx);
int32_t		WStr1FindCHs(const wstr1* str, const wch* pcs, size_t s, size_t* pIdx);
int32_t		WStr1FindCS(const wstr1* str, const wch* pcs, size_t s, size_t* pIdx);
int32_t		WStr1FindTmp(const wstr1* str, wstr1 tmp, size_t s, size_t* pIdx);
int32_t		WStr1Find(const wstr1* str, const wstr1* str2, size_t s, size_t* pIdx);
int32_t		WStr1FindReversedCH(const wstr1* str, wch c, size_t s, size_t* pIdx);
int32_t		WStr1FindReversedCHs(const wstr1* str, const wch* pcs, size_t s, size_t* pIdx);
int32_t		WStr1FindReversedCS(const wstr1* str, const wch* pcs, size_t s, size_t* pIdx);
int32_t		WStr1FindReversedTmp(const wstr1* str, wstr1 tmp, size_t s, size_t* pIdx);
int32_t		WStr1FindReversed(const wstr1* str, const wstr1* str2, size_t s, size_t* pIdx);
int32_t		WStr1SplitCH(const wstr1* str, wch c, wstr1* stro, size_t s, size_t* idxRemain = NULL);
int32_t		WStr1SplitCHs(const wstr1* str, const wch* pcs, wstr1* stro, size_t s, size_t* idxRemain = NULL);
int32_t		WStr1SplitCS(const wstr1* str, const wch* pcs, wstr1* stro, size_t s, size_t* idxRemain = NULL);
int32_t		WStr1SplitTmp(const wstr1* str, wstr1 tmp, wstr1* stro, size_t s, size_t* idxRemain = NULL);
int32_t		WStr1Split(const wstr1* str, wstr1* str2, wstr1* stro, size_t s, size_t* idxRemain = NULL);
//int32_t		WStr1SplitRefCh(const wstr1* str, wch c, size_t* i, size_t s = 0);
//int32_t		WStr1SplitRef(const wstr1* str, wstr1* str2, size_t* i, size_t s = 0);

#pragma endregion str1


#pragma region hashstr

inline int32_t hashWStr_Def32(const wch* p, size_t len);
inline int64_t hashWStr_Def64(const wch* p, size_t len);


// hashstr
typedef struct hashwstring
{
	wstr1	str;
	int64_t	hash;
	bool	bHashed;
} hashwstr;

#pragma endregion hashstr