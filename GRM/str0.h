#pragma once

#include "def0.h"
#include <string.h>


#define STRENDCHAR					('\0')
#define WSTRENDCHAR					(L'\0')

#define STR_ISNULL(status)			(STR0S_NULL == (status))
#define STR_ISOWN(status)			(STR0S_OWN == (status))
#define STR_ISLOCAL(status)			(STR0S_LOCAL == (status))
#define STR_ISREF(status)			(STR0S_REF == (status))


enum STR0_STATUS
{
	STR0S_NULL		= 0,
	STR0S_OWN		= 1,
	STR0S_LOCAL		= 2,
	STR0S_REF		= 3
};


#pragma region basic str

bool		_PtrOverlap(const void* const p1, size_t len1, const void* const p2, size_t len2);
bool		_PtrOverlapWCH(const wch* p1, size_t len1, const wch* p2, size_t len2);

int32_t		_WCSEqual(const wch* const p1, size_t len, const wch* const p2, size_t len2);
int32_t		_WCSEqualCI(const wch* const p1, size_t len, const wch* const p2, size_t len2);
int32_t		_WCSFind(const wch* p1, size_t len, const wch* p2, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSFindCI(const wch* p1, size_t len, const wch* p2, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSFindReversed(const wch* p1, size_t len, const wch* p2, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSFindReversedCI(const wch* p1, size_t len, const wch* p2, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSContain(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSContainCI(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSIsAt(const wch* p1, size_t len, size_t pos, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSIsAtCI(const wch* p1, size_t len, size_t pos, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSStartWith(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSStartWithCI(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSEndWith(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSEndWithCI(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2);
int32_t		_WCSSplit(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSSplitCI(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSSplitReversed(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSSplitReversedCI(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2, size_t pos = 0, size_t* posRemain = NULL);
int32_t		_WCSTrimCHs(const wch* p1, size_t len, const wch* pcs, size_t s2, size_t len2, size_t* trimf, size_t* trimr);

#pragma endregion basic str


#pragma region str0

typedef ch*		str0;
typedef wch*	wstr0;

str0		Str0Make(const ch* pcs);
str0		Str0MakeBy(const ch* pcs, size_t s, size_t len);
int32_t		Str0Release(str0 str);
str0		Str0Concat(str0 str, str0 str2);
str0		Str0ConcatBy(str0 str, const ch* pcs, size_t s, size_t len);
str0		Str0Insert(str0 str, str0 str2, size_t pos);
str0		Str0InsertBy(str0 str, const ch* pcs, size_t s, size_t len, size_t pos);
str0		Str0Prepend(str0 str, str0 str2);
str0		Str0PrependBy(str0 str, const ch* pcs, size_t s, size_t len);


#pragma endregion str0


#pragma region str1


// ***** TODO List: *****
// 1. malloc 0
// 2. use memmove in assign?
// 3. _InitWStr1


#define STR1_CAP0						16
#define WSTR1_CAP0						8
#define WSTR1_ISNULLSTR(pwstr)			(NULL == (pwstr) || STR_ISNULL((pwstr)->status))
#define WSTR1_ISNULL(pwstr)				(STR_ISNULL((pwstr)->status))
#define WSTR1_ISNULLO(wstr)				(STR_ISNULL((wstr).status))
#define WSTR1_ISOWN(pwstr)				(STR_ISOWN((pwstr)->status))
#define WSTR1_ISOWNO(wstr)				(STR_ISOWN((wstr).status))
#define WSTR1_ISLOCAL(pwstr)			(STR_ISLOCAL((pwstr)->status))
#define WSTR1_ISLOCALO(wstr)			(STR_ISLOCAL((wstr).status))
#define WSTR1_ISREF(pwstr)				(STR_ISREF((pwstr)->status))
#define WSTR1_ISREFO(wstr)				(STR_ISREF((wstr).status))
#define WSTR1_ISDATASTATUS(pwstr)		(WSTR1_ISOWN(pwstr) || WSTR1_ISLOCAL(pwstr) || WSTR1_ISREF(pwstr))
#define WSTR1_STR(pwstr)				(WSTR1_ISLOCAL(pwstr) ? (pwstr)->s0 : (pwstr)->s1)
#define WSTR1_STRO(wstr)				(WSTR1_ISLOCALO(wstr) ? (wstr).s0 : (wstr).s1)
#define WSTR1_STROFSTATUS(wstr, status)	(!STR_ISLOCAL(status) ? (wstr).s0 : (wstr).s1)
#define WSTR1_STRAT(wstr, idx)			(WSTR1_STR(wstr)[idx])
#define WSTR1_LEN(pwstr)				(WSTR1_ISLOCAL(pwstr) ? wcslen((pwstr)->s0) : (pwstr)->len)
#define WSTR1_LENO(wstr)				(WSTR1_ISLOCALO(wstr) ? wcslen((wstr).s0) : (wstr).len)
#define WSTR1_LENGOBAL(pwstr)			(WSTR1_ISNULL(pwstr) ? 0 : WSTR1_LEN(pwstr))
#define WSTR1_SETLEN(pwstr, len)		(if (!WSTR1_ISLOCAL(pwstr)) (pwstr)->len = len;)
#define WSTR1_POS(pwstr, idx)			(WSTR1_STR(pwstr) + (idx))

#define WSTR1_MOVE(pwstr, s, d, len)	(memmove(WSTR1_POS((pwstr), (d)), WSTR1_POS((pwstr), (s)), (len) * sizeof(wch)))


typedef struct string1
{
	STR0_STATUS		status;
	union
	{
		struct {
			ch*		s1;
			size_t	len;
		};
		ch			s0[STR1_CAP0];
	};
} str1;

typedef struct wstring1
{
	STR0_STATUS		status;
	union {
		struct {
			wch*	s1;
			size_t	len;
		};
		wch			s0[WSTR1_CAP0];
	};
	// bool			bWide;
} wstr1;


// make (#24)
wstr1		WStr1Default();// default string
wstr1		MakeWStr1CH(wch c, int32_t* pRet = NULL);
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
int32_t		WStr1MakeCH(wstr1* str, wch c);
int32_t		WStr1MakeCS(wstr1* str, const wch* pcs);
int32_t		WStr1MakeCSBy(wstr1* str, const wch* pcs, size_t len);
int32_t		WStr1MakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		WStr1MakeCSFromPos(wstr1* str, const wch* pcs, size_t s);
int32_t		WStr1MakeTmp(wstr1* str, const wstr1 tmp);
int32_t		WStr1Make(wstr1* str, const wstr1* other);
int32_t		WStr1MakeBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1MakeBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1MakeFromPos(wstr1* str, const wstr1* other, size_t s);
// pure make (#13)
int32_t		WStr1PureMakeDefault(wstr1* str);
int32_t		_WStr1PureMakeCSBy2(wstr1* str, const wch* pcs, size_t s, size_t len);
int32_t		_WStr1PureMakeCSFromPos(wstr1* str, const wch* pcs, size_t s, size_t te);
int32_t		WStr1PureMakeCH(wstr1* str, wch c);
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
// ref (#18)
wstr1		RefWStr1CS(wch* pcs, int32_t* pRet = NULL);
wstr1		RefWStr1CSBy(wch* pcs, size_t len, int32_t* pRet = NULL);
wstr1		RefWStr1CSBy2(wch* pcs, size_t s, size_t len, int32_t* pRet = NULL);
wstr1		RefWStr1CSFromPos(wch* pcs, size_t s, int32_t* pRet = NULL);
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
int32_t		WStr1Ref(wstr1* str, const wstr1* other);
int32_t		WStr1RefBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1RefBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1RefFromPos(wstr1* str, const wstr1* other, size_t s);
// pure ref (#10)
int32_t		_WStr1PureRefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len);
int32_t		_WStr1PureRefCSFromPos(wstr1* str, wch* pcs, size_t s, size_t te);
int32_t		WStr1PureRefCS(wstr1* str, wch* pcs);
int32_t		WStr1PureRefCSBy(wstr1* str, wch* pcs, size_t len);
int32_t		WStr1PureRefCSBy2(wstr1* str, wch* pcs, size_t s, size_t len);
int32_t		WStr1PureRefCSFromPos(wstr1* str, wch* pcs, size_t s);
int32_t		WStr1PureRef(wstr1* str, const wstr1* other);
int32_t		WStr1PureRefBy(wstr1* str, const wstr1* other, size_t len);
int32_t		WStr1PureRefBy2(wstr1* str, const wstr1* other, size_t s, size_t len);
int32_t		WStr1PureRefFromPos(wstr1* str, const wstr1* other, size_t s);
// lifecycle (#8)
void		InitWStr1(wstr1* str);
int32_t		WStr1Deref(wstr1* str);
int32_t		WStr1Clear(wstr1* str);
int32_t		WStr1ClearLazy(wstr1* str);
int32_t		WStr1Attach(wstr1* str, wch* pcs);
int32_t		WStr1Detach(wstr1* str, wch** ppcs = NULL);
int32_t		WStr1ReAttach(wstr1* str, wch* pcs, wch** ppcsOld = NULL);
int32_t		WStr1Release(wstr1* str);
int32_t		WStr1Abandon(wstr1* str);
// store (#4)
int32_t		WStr1ToOwn(wstr1* str);
int32_t		WStr1ToLocal(wstr1* str);
int32_t		WStr1Fit(wstr1* str);
int32_t		WStr1Relen(wstr1* str, size_t len, bool bLazy = false);
// internal (#18)
void		_InitWStr1(wstr1* str);
void		_WStr1ShallowCopy(wstr1* str, const wstr1* other);
int32_t		_WStr1Move(wstr1* str, wstr1* other);
int32_t		_WStr1InitCap(wstr1* str, size_t cap);
int32_t		_WStr1Recap(wstr1* str, size_t cap);
int32_t		_WStr1InitLen(wstr1* str, size_t len);
int32_t		_WStr1Relen(wstr1 * str, size_t len);
int32_t		_WStr1RelenLazy(wstr1* str, size_t len);
int32_t		_WStr1ExtendBy(wstr1 * str, size_t dLen);
int32_t		_WStr1ShrinkBy(wstr1 * str, size_t dLen);
int32_t		_WStr1ShrinkByLazy(wstr1 * str, size_t dLen);
int32_t		_WStr1ToOwn(wstr1 * str, size_t recap = 0);
int32_t		_WStr1ToLocal(wstr1 * str, size_t recap = 0);
int32_t		_WStr1Overlap(const wstr1* str, const wstr1* str2);
int32_t		_WStr1OverlapCS(const wstr1* str, const wch* pcs, size_t s2, size_t len2);
bool		_WStr1RefMgrd(const wstr1* str, const wstr1* str2);
int32_t		_WStr1Release(wstr1* str);
int32_t		_WStr1Abandon(wstr1* str);
// info (#13)
const wch*	WStr1Str(const wstr1* str);
const wch*	WStr1CStr(wstr1* str, int32_t* pRet = NULL);// ToOwn if REF
size_t		WStr1Len(const wstr1* str);// 0 for null
bool		WStr1IsNull0(const wstr1 str);
bool		WStr1IsEmpty0(const wstr1 str);
bool		WStr1IsNullOrEmpty0(const wstr1 str);
bool		WStr1IsNull(const wstr1* str);
bool		WStr1IsEmpty(const wstr1* str);
bool		WStr1IsNullOrEmpty(const wstr1* str);
int32_t		WStr1IsNullP(const wstr1* str);// < 0 for null
int32_t		WStr1IsEmptyP(const wstr1* str);// < 0 for null
int32_t		WStr1IsNullOrEmptyP(const wstr1* str);// < 0 for null
bool		WStr1RefMgrd(const wstr1* str, const wstr1* str2);
// judge (#34)
int32_t		WStr1Equals(const wstr1* str, const wstr1* str2);
int32_t		WStr1EqualsTmp(const wstr1 tmp1, const wstr1 tmp2);
int32_t		WStr1EqualsCH(const wstr1* str, wch c);
int32_t		WStr1EqualsCS(const wstr1* str, const wch* pcs);
int32_t		WStr1EqualsCSBy(const wstr1* str, const wch* pcs, size_t len2);
int32_t		WStr1EqualsCSBy2(const wstr1* str, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsCSBy3(const wstr1* str, size_t s1, size_t len1, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsTmpCSBy2(wstr1 tmp, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsI(const wstr1* str, const wstr1* str2);
int32_t		WStr1EqualsITmp(const wstr1 tmp1, const wstr1 tmp2);
int32_t		WStr1EqualsICH(const wstr1* str, wch c);
int32_t		WStr1EqualsICS(const wstr1* str, const wch* pcs);
int32_t		WStr1EqualsICSBy(const wstr1* str, const wch*, size_t len2);
int32_t		WStr1EqualsICSBy2(const wstr1* str, const wch*, size_t s2, size_t len2);
int32_t		WStr1EqualsICSBy3(const wstr1* str, size_t s1, size_t len1, const wch*, size_t s2, size_t len2);
int32_t		WStr1EqualsITmpCSBy2(wstr1 tmp, const wch*, size_t s2, size_t len2);
int32_t		WStr1EqualsA(const wstr1* str, const str1* str2);
int32_t		WStr1EqualsATmp(const wstr1 tmp1, const str1 tmp2);
int32_t		WStr1EqualsACH(const wstr1* str, ch c);
int32_t		WStr1EqualsACS(const wstr1* str, const ch* pcs);
int32_t		WStr1EqualsACSBy(const wstr1* str, const ch* pcs, size_t len2);
int32_t		WStr1EqualsACSBy2(const wstr1* str, const ch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsACSBy3(const wstr1* str, size_t s1, size_t len1, const ch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsATmpCSBy2(wstr1 tmp, const ch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsIA(const wstr1* str, const str1* str2);
int32_t		WStr1EqualsIATmp(const wstr1 tmp1, const str1 tmp2);
int32_t		WStr1EqualsIACH(const wstr1* str, wch c);
int32_t		WStr1EqualsIACS(const wstr1* str, const wch* pcs);
int32_t		WStr1EqualsIACSBy(const wstr1* str, const wch* pcs, size_t len2);
int32_t		WStr1EqualsIACSBy2(const wstr1* str, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsIACSBy3(const wstr1* str, size_t s1, size_t len1, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1EqualsIATmpCSBy2(wstr1 tmp, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1ToLower(wstr1* str);
int32_t		WStr1ToUpper(wstr1* str);
// operation (#115)
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
int32_t		WStr1RemoveCSBy(wstr1* str, const wch* pcs, const size_t s2, const size_t len2);
int32_t		WStr1Remove(wstr1* str, wstr1* str2);
int32_t		WStr1RemoveTmp(wstr1* str, wstr1 tmp);
int32_t		WStr1Remove1CH(wstr1* str, wch c, size_t pos, size_t* posRemain = NULL);
int32_t		WStr1Remove1CS(wstr1* str, const wch* pcs, size_t pos, size_t* posRemain = NULL);
int32_t		WStr1Remove1CSBy(wstr1* str, const wch* pcs, const size_t s2, const size_t len2);
int32_t		WStr1Remove1(wstr1* str, wstr1* str2, size_t pos, size_t* posRemain = NULL);
int32_t		WStr1Remove1Tmp(wstr1* str, wstr1 tmp, size_t pos, size_t* posRemain = NULL);
int32_t		_WStr1InsertCSBy2(wstr1* str, size_t pos, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1InsertCH(wstr1* str, size_t pos, wch c);
int32_t		WStr1InsertTmp(wstr1* str, size_t pos, wstr1 tmp);
int32_t		WStr1Insert(wstr1* str, size_t pos, const wstr1* str2);
int32_t		WStr1InsertBy(wstr1* str, size_t pos, const wstr1* str2, size_t len);
int32_t		WStr1InsertBy2(wstr1* str, size_t pos, const wstr1* str2, size_t s, size_t len);
int32_t		WStr1InsertCS(wstr1* str, size_t pos, const wch* pcs);
int32_t		WStr1InsertCSBy(wstr1* str, size_t pos, const wch* pcs, size_t len);
int32_t		WStr1InsertCSBy2(wstr1* str, size_t pos, const wch* pcs, size_t s, size_t len);
int32_t		_WStr1PrependCSBy2(wstr1* str, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1Prepend(wstr1* str, const wstr1* str2);
int32_t		WStr1PrependTmp(wstr1* str, wstr1 tmp);
int32_t		WStr1PrependCH(wstr1* str, wch c);
int32_t		WStr1PrependBy(wstr1* str, const wstr1* str2, size_t len);
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
int32_t		WStr1ContainsCSBy(const wstr1* str, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1Contains(const wstr1* str, const wstr1* str2);
int32_t		WStr1ContainsBy(const wstr1* str, const wstr1* str2, size_t s2, size_t len2);
int32_t		WStr1ContainsTmp(const wstr1* str, wstr1 tmp);
int32_t		_WStr1IsAtCSBy2(const wstr1* str, size_t pos, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1StartWithCH(const wstr1* str, wch c);
int32_t		WStr1StartWithCHs(const wstr1* str, const wch* pcs);
int32_t		WStr1StartWithCS(const wstr1* str, const wch* pcs);
int32_t		WStr1StartWithCSBy(const wstr1* str, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1StartWith(const wstr1* str, const wstr1* str2);
int32_t		WStr1StartWithBy(const wstr1* str, const wstr1* str2, size_t s2, size_t len2);
int32_t		WStr1StartWithTmp(const wstr1* str, wstr1 tmp);
int32_t		WStr1EndWithCH(const wstr1* str, wch c);
int32_t		WStr1EndWithCHs(const wstr1* str, const wch* pcs);
int32_t		WStr1EndWithCS(const wstr1* str, const wch* pcs);
int32_t		WStr1EndWithCSBy(const wstr1* str, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1EndWith(const wstr1* str, const wstr1* str2);
int32_t		WStr1EndWithBy(const wstr1* str, const wstr1* str2, size_t s2, size_t len2);
int32_t		WStr1EndWithTmp(const wstr1* str, wstr1 tmp);
int32_t		WStr1IsAtCH(const wstr1* str, size_t pos, wch c);
int32_t		WStr1IsAtCHs(const wstr1* str, size_t pos, const wch* pcs);
int32_t		WStr1IsAtCS(const wstr1* str, size_t pos, const wch* pcs);
int32_t		WStr1IsAtCSBy(const wstr1* str, size_t pos, const wch* pcs, size_t s2, size_t len2);
int32_t		WStr1IsAt(const wstr1* str, size_t pos, const wstr1* str2);
int32_t		WStr1IsAtBy(const wstr1* str, size_t pos, const wstr1* str2, size_t s2, size_t len2);
int32_t		WStr1IsAtTmp(const wstr1* str, size_t pos, wstr1 tmp);
int32_t		WStr1FindCH(const wstr1* str, wch c, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindCHs(const wstr1* str, const wch* pcs, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindCS(const wstr1* str, const wch* pcs, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindCSBy(const wstr1* str, const wch* pcs, size_t s2, size_t len2, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1Find(const wstr1* str, const wstr1* str2, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindBy(const wstr1* str, const wstr1* str2, size_t s2, size_t len2, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindTmp(const wstr1* str, wstr1 tmp, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindReversedCH(const wstr1* str, wch c, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindReversedCHs(const wstr1* str, const wch* pcs, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindReversedCS(const wstr1* str, const wch* pcs, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindReversedCSBy(const wstr1* str, const wch* pcs, size_t s2, size_t len2, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindReversed(const wstr1* str, const wstr1* str2, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindReversedBy(const wstr1* str, const wstr1* str2, size_t s2, size_t len2, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1FindReversedTmp(const wstr1* str, wstr1 tmp, size_t* pIdx = NULL, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1Replace(const wstr1 str, wstr1* str2f, wstr1* strt, size_t pos = 0);
int32_t		WStr1ReplaceTmp(const wstr1 str, wstr1 tmp1, wstr1 tmp2, size_t pos = 0);
int32_t		WStr1ReplaceCS(const wstr1 str, const wch* pcs1, const wch* pcs2, size_t pos = 0);
int32_t		WStr1Replace1(const wstr1 str, wstr1* str2f, wstr1* strt, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1Replace1Tmp(const wstr1 str, wstr1 tmp1, wstr1 tmp2, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1Replace1CS(const wstr1 str, const wch* pcs1, const wch* pcs2, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitCH(const wstr1* str, wch c, wstr1* stro, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitCHs(const wstr1* str, const wch* pcs, wstr1* stro, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitCS(const wstr1* str, const wch* pcs, wstr1* stro, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitCSBy(const wstr1* str, const wch* pcs, size_t s2, size_t len2, wstr1* stro, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1Split(const wstr1* str, wstr1* str2, wstr1* stro, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitBy(const wstr1* str, wstr1* str2, size_t s2, size_t len2, wstr1* stro, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitTmp(const wstr1* str, wstr1 tmp, wstr1* stro, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitRefCH(const wstr1* str, wch c, size_t* pLen, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitRefCHs(const wstr1* str, const wch* pcs, size_t* pLen, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitRefCS(const wstr1* str, const wch* pcs, size_t* pLen, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitRefCSBy(const wstr1* str, const wch* pcs, size_t s2, size_t len2, size_t* pLen, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitRef(const wstr1* str, wstr1* str2, size_t* pLen, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitRefBy(const wstr1* str, wstr1* str2, size_t s2, size_t len2, size_t* pLen, size_t pos = 0, size_t* idxRemain = NULL);
int32_t		WStr1SplitRefTmp(const wstr1* str, wstr1 tmp, size_t* pLen, size_t pos = 0, size_t* idxRemain = NULL);

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