#include "pch.h"
#define LIB_EXPORT
#include "list0.h"

#include <malloc.h>
#include <wchar.h>
#include "def0.h"


const size_t sizeList0 = sizeof(list0);
const size_t sizeList1 = sizeof(list1);
const size_t sizeList2 = sizeof(list2);

void InitList0Info(list0info* const pInfo)
{
	if (ISNOTNULLPTR(pInfo))
	{
		//pInfo->fEqualsKey		= NULL;
		pInfo->fEquals			= NULL;
		pInfo->fConstructorDef	= NULL;
		pInfo->fDestructor		= NULL;
		pInfo->fConstructorCopy	= NULL;
		pInfo->fAssignCopy		= NULL;
		pInfo->fConstructorMove	= NULL;
		pInfo->fAssignMove		= NULL;
	}
}

void InitPtrList0Info(ptrlist0info* const pInfo)
{
	if (ISNOTNULLPTR(pInfo))
	{
		pInfo->fEquals			= NULL;
		pInfo->fConstructorDef	= NULL;
		pInfo->fDestructor		= NULL;
		pInfo->fConstructorCopy	= NULL;
		pInfo->fAssignCopy		= NULL;
		pInfo->fConstructorMove	= NULL;
		pInfo->fAssignMove		= NULL;
	}
}

int32_t ListExtendDLen(const size_t len, const size_t dlen, size_t* const cap)
{
	const size_t lenNew = len + dlen;
	if (lenNew < len)
		return GRET_OVERFLOW;
	if (lenNew <= *cap)
		return GRET_SUCCEED;
	*cap = max(lenNew, (*cap) * 2);
	return GRET_SUCCEED;
}


#pragma region list0

int32_t List0Len(clist0c ls, size_t* const pLen)
{
	if (LIST0ISNOTNULL(ls) && ISNOTNULLPTR(pLen))
	{
		*pLen = LIST0LEN(ls);
		return GRET_SUCCEED;
	}
	else
		return GRET_NULL;
}

int32_t List0Cap(clist0c ls, size_t* const pCap)
{
	if (LIST0ISNOTNULL(ls) && ISNOTNULLPTR(pCap))
	{
		*pCap = LIST0CAP(ls);
		return GRET_SUCCEED;
	}
	else
		return GRET_NULL;
}

int32_t List0Status(clist0c ls, CTNSTATUS* const pStatus)
{
	if (ISNOTNULLPTR(ls) && ISNOTNULLPTR(pStatus))
	{
		*pStatus = (LIST0STATUS(ls));
		return GRET_SUCCEED;
	}
	else
		return GRET_NULL;
}

int32_t List0SetStatus(clist0c ls, const CTNSTATUS status)
{
	if (ISNOTNULLPTR(ls))
	{
		LIST0_SETSTATUS(ls, status);
		return GRET_SUCCEED;
	}
	else
		return GRET_NULL;
}

list0 _List0Make(const size_t size)
{
	return _List0MakeBy(LIST0INITCAP, size);
}

list0 _List0MakeBy(const size_t cap, const size_t size)
{
	if (cap < 0)
		return NULL;
	list0c ls = LIST0MALLOC(cap, size);
	if (ISNULLPTR(ls))
		return NULL;
	LIST0_SETLENCAPSTATUS(ls, 0, cap, CTNSTATUS::CTNS_OWN);
	return ls;
}

int32_t _List0MakePure(list0* const pls, const size_t size)
{
	return _List0MakeByPure(pls, LIST0INITCAP, size);
}

int32_t _List0MakeByPure(list0* const pls, const size_t cap, const size_t size)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (cap < 0)
		return GRET_INVARG;
	list0c ls = LIST0MALLOC(cap, size);
	if (ISNULLPTR(ls))
		return GRET_MALLOC;
	LIST0_SETLENCAPSTATUS(ls, 0, cap, CTNSTATUS::CTNS_OWN);
	*pls = ls;
	return GRET_SUCCEED;
}

int32_t _List0Add(list0* const pls, void* const pElem, const size_t size)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	list0 ls = *pls;
	if (LIST0ISNULLP(ls))
		return GRET_NULL;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	//if (size <= 0)// TODO: do not need, validate with macro
	//	return GRET_INVARG;

	const int32_t ret = __List0ExtendBy(pls, 1, size);
	RET_ON_NEG(ret);
	ls = *pls;

	const size_t lenOld = LIST0LEN(ls);
	memcpy(_LIST0ADDR(ls, lenOld, size), pElem, size);
	LIST0LEN(ls) = lenOld + 1;
	return GRET_SUCCEED;
}

int32_t _List0SetAt(list0c ls, void* const pElem, const size_t idx, const size_t size)
{
	if (ISNULLPTR(ls))
		return GRET_NULL;
	if (LIST0ISNULL(ls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx < 0 || idx >= LIST0LEN(ls))
		return GRET_INVIDX;

	memcpy(_LIST0ADDR(ls, idx, size), pElem, size);
	return GRET_SUCCEED;
}

int32_t List0Clear(clist0c ls)
{
	if (ISNULLPTR(ls))
		return GRET_NULL;
	if (LIST0ISNULL(ls))
		return GRET_INVSTATUS;

	if (LIST0ISEMPTY(ls))
		return GRET_NONE;
	LIST0LEN(ls) = 0;
	return GRET_SUCCEED;
}

int32_t _List0Copy(clist0c ls, list0* const plsr, const size_t size)
{
	if (ISNULLPTR(ls))
		return GRET_NULL;
	if (LIST0ISNULL(ls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	const size_t cap = LIST0CAP(ls);
	list0 ls2 = NULL;
	const int32_t ret = _List0MakeByPure(&ls2, cap, size);
	RET_ON_NEG(ret);

	const size_t len = LIST0LEN(ls);
	memcpy(LIST0ELEMADDR(ls2), LIST0ELEMADDR(ls), len * size);
	*plsr = ls2;
	return GRET_SUCCEED;
}

int32_t _List0ExtendBy(list0* const pls, const size_t dLen, const size_t size)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	clist0c ls = *pls;
	if (LIST0ISNULLP(ls))
		return GRET_INVSTATUS;
	return __List0ExtendBy(pls, dLen, size);
}

int32_t __List0ExtendBy(list0* const pls, const size_t dLen, const size_t size)
{
	clist0c ls = *pls;
	size_t capNew = LIST0CAP(ls);
	const int32_t ret = ListExtendDLen(LIST0LEN(ls), dLen, &capNew);
	RET_ON_NPOS(ret);
	return __List0ReCap(pls, capNew, size);
}

int32_t __List0ReCap(list0* const pls, const size_t cap, const size_t size)
{
	list0c ls = LIST0REALLOC(*pls, cap, size);
	if (ISNULLPTR(ls))
		return GRET_MALLOC;
	*pls = ls;
	return GRET_SUCCEED;
}

int32_t List0Release(list0c ls)
{
	if (ISNULLPTR(ls))
		return GRET_NONE;

	const CTNSTATUS status = LIST0STATUS(ls);
	switch (status)
	{
	case CTNSTATUS::CTNS_NULL:
		return GRET_NONE;
	case CTNSTATUS::CTNS_OWN:
	case CTNSTATUS::CTNS_REF:
		//LIST0_SETSTATUS(ls, CTNSTATUS::CTNS_NULL);
		free(ls);
		return GRET_SUCCEED;
	default:
		return GRET_NONE;
	}
	return GRET_NONE;
}

#pragma endregion list0


#pragma region list1

int32_t _List1ExtendDLen(list1* const pls, const size_t dLen)
{
	size_t capNew = pls->cap;
	const int32_t ret = ListExtendDLen(pls->len, dLen, &capNew);
	RET_ON_NPOS(ret);
	return _List1ReCap(pls, capNew);
}

int32_t _List1ReCap(list1* const pls, const size_t cap)
{
	if (OVERFLOW_MUL_sizet(cap, pls->szData))
		return GRET_OVERFLOW;
	void* const ptr = realloc(pls->ptr, cap * pls->szData);
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	pls->ptr = ptr;
	pls->cap = cap;
	return GRET_SUCCEED;
}

void List1Init0(list1* const pls)
{
	if (ISNOTNULLPTR(pls))
	{
		pls->ptr	= NULL;
		pls->len	= 0;
		pls->cap	= 0;
		pls->szData	= 0;
	}
}

int32_t List1Make(list1* const pls, const size_t size)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (size <= 0)
		return GRET_INVARG;
	List1Release(pls);
	return List1MakePure(pls, size);
}

int32_t List1MakePure(list1* const pls, const size_t size)
{
	return List1MakePureBy(pls, LIST0INITCAP, size);
}

int32_t List1MakePureBy(list1* const pls, const size_t cap, const size_t size)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (size <= 0)
		return GRET_INVARG;

	List1Init0(pls);
	if (OVERFLOW_MUL_sizet(cap, size))
		return GRET_OVERFLOW;
	void* const ptr = malloc(cap * size);
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	//memset(ptr, 0, cap * sizeof(size));
	pls->ptr	= ptr;
	pls->cap	= cap;
	pls->szData	= size;
	return GRET_SUCCEED;
}

int32_t List1From(list1* const pls, const size_t size, void* const pls2, const size_t len)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (size <= 0)
		return GRET_INVARG;
	if (ISNULLPTR(pls2))
		return GRET_NULLARG;
	if (len <= 0)
		return GRET_INVARG;
	List1Release(pls);
	return _List1FromPure(pls, size, pls2, len);
}

int32_t List1FromPure(list1* const pls, const size_t size, void* const pls2, const size_t len)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (size <= 0)
		return GRET_INVARG;
	if (ISNULLPTR(pls2))
		return GRET_NULLARG;
	if (len <= 0)
		return GRET_INVARG;
	return _List1FromPure(pls, size, pls2, len);
}

int32_t _List1FromPure(list1* const pls, const size_t size, void* const pls2, const size_t len)
{
	List1Init0(pls);
	pls->ptr = pls2;
	pls->len = pls->cap = len;
	pls->szData = size;
	return GRET_SUCCEED;
}

int32_t List1Add(list1* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (NULL == pElem)
		return GRET_NULL;
	const int32_t ret = _List1ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	memcpy(LISTPOS(pls->ptr, pls->len, pls->szData), pElem, pls->szData);
	pls->len++;
	return GRET_SUCCEED;
}

int32_t List1Clear(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	//if (0 == pls->len)
	//	return GRET_NONE;
	pls->len = 0;
	return GRET_SUCCEED;
}

int32_t List1ClearFree(list1* const pls, const funFreeElem funFree)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (ISNULLPTR(funFree))
		return GRET_NULL;
	//if (0 == pls->len)
	//	return GRET_NONE;
	for (size_t i = 0; i < pls->len; ++i)
		const int32_t ret = funFree(LISTPOS(pls->ptr, i, pls->szData));
	//memset(pls->ptr, 0, pls->len * sizeof(pls->szData));
	pls->len = 0;
	return GRET_SUCCEED;
}

int32_t List1Release(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	safefreereset(&(pls->ptr));
	List1Init0(pls);
	return GRET_SUCCEED;
}

#pragma endregion list1


#pragma region list2

void _List2MoveTo(list2* const pls, const size_t from, const size_t to, const size_t len)
{
	const funLS0ElemMoveConstructor fMoveCtor = pls->info.fConstructorMove;
	if (ISNOTNULLPTR(fMoveCtor))
	{
		void* const plsf0 = LIST2POS(pls, from);
		void* const plst0 = LIST2POS(pls, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fMoveCtor(LIST2POS(pls, to + i), LIST2POS(pls, from + i));
		}
		else
		{
			size_t i = len;
			while (i-- > 0)
				fMoveCtor(LIST2POS(pls, to + i), LIST2POS(pls, from + i));
		}
	}
	else
	{
		memmove(LIST2POS(pls, to), LIST2POS(pls, from), len * pls->info.sizeUnit);
	}
}
inline void _List2MoveToBuf(list2* const pls, const size_t from, void* const pDst, const size_t to, const size_t len)
{
	const funLS0ElemMoveConstructor fMoveCtor = pls->info.fConstructorMove;
	void* const plst0 = (void*)((char*)pDst + to * pls->info.sizeUnit);
	if (ISNOTNULLPTR(fMoveCtor))
	{
		void* const plsf0 = LIST2POS(pls, from);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fMoveCtor((char*)pDst + (to + i) * pls->info.sizeUnit, LIST2POS(pls, from + i));
		}
		else
		{
			for (size_t i = len; i-- > 0;)
				fMoveCtor((char*)pDst + (to + i) * pls->info.sizeUnit, LIST2POS(pls, from + i));
		}
	}
	else
	{
		memmove(plst0, LIST2POS(pls, from), len * pls->info.sizeUnit);
	}
}

inline void _List2MoveFromBuf(list2* const pls, const size_t to, void* const pSrc, const size_t from, const size_t len)
{
	const funLS0ElemMoveConstructor fMoveCtor = pls->info.fConstructorMove;
	void* const plsf0 = (void*)((char*)pSrc + from * pls->info.sizeUnit);
	if (ISNOTNULLPTR(fMoveCtor))
	{
		void* const plst0 = LIST2POS(pls, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fMoveCtor(LIST2POS(pls, to + i), (char*)pSrc + (from + i) * pls->info.sizeUnit);
		}
		else
		{
			for (size_t i = len; i-- > 0;)
				fMoveCtor(LIST2POS(pls, to + i), (char*)pSrc + (from + i) * pls->info.sizeUnit);
		}
	}
	else
	{
		memmove(LIST2POS(pls, to), plsf0, len * pls->info.sizeUnit);
	}
}

void _List2CopyToList(list2* const pls, const size_t from, list2* const pls2, const size_t to, const size_t len)
{
	const funLS0ElemCopyConstructor fCopyCtor = pls->info.fConstructorCopy;
	if (ISNOTNULLPTR(fCopyCtor))
	{
		void* const plsf0 = LIST2POS(pls, from);
		void* const plst0 = LIST2POS(pls2, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fCopyCtor(LIST2POS(pls2, to + i), LIST2POS(pls, from + i));
		}
		else
		{
			for (size_t i = len; i-- > 0;)
				fCopyCtor(LIST2POS(pls2, to + i), LIST2POS(pls, from + i));
		}
	}
	else
	{
		memmove(LIST2POS(pls2, to), LIST2POS(pls, from), len * pls->info.sizeUnit);
	}
}
void _List2Make(list2* const pls, const size_t from, const size_t num)
{
	const funLS0ElemConstructorDef fCTor = pls->info.fConstructorDef;
	if (ISNOTNULLPTR(fCTor))
	{
		for (size_t idx = from; idx < num; ++idx)
			fCTor(LIST2POS(pls, idx));
	}
	else
	{
		memset(LIST2POS(pls, from), 0, num * sizeof(pls->info.sizeUnit));
	}
}
void _List2Free(list2* const pls, const size_t from, const size_t num)
{
	const funLS0ElemDestructor fDTor = pls->info.fDestructor;
	if (ISNOTNULLPTR(fDTor))
	{
		for (size_t idx = from; idx < num; ++idx)
			fDTor(LIST2POS(pls, idx));
	}
}
void _List2MakeAtCopy(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorCopy))
		pls->info.fConstructorCopy(pElem, LIST2POS(pls, idx));
	else
		memmove(LIST2POS(pls, idx), pElem, pls->info.sizeData);
}
void _List2MakeAtMove(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorMove))
		pls->info.fConstructorMove(pElem, LIST2POS(pls, idx));
	else
	{
		memmove(LIST2POS(pls, idx), pElem, pls->info.sizeData);
		memset(pElem, 0, pls->info.sizeData);
	}
}
void _List2SetAtCopy(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNOTNULLPTR(pls->info.fAssignCopy))
		pls->info.fAssignCopy(pElem, LIST2POS(pls, idx));
	else
		memmove(LIST2POS(pls, idx), pElem, pls->info.sizeData);
}
void _List2SetAtMove(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNOTNULLPTR(pls->info.fAssignMove))
		pls->info.fAssignMove(pElem, LIST2POS(pls, idx));
	else
	{
		memmove(LIST2POS(pls, idx), pElem, pls->info.sizeData);
		memset(pElem, 0, pls->info.sizeData);
	}
}
void _List2GetAtCopy(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorCopy))
		pls->info.fConstructorCopy(LIST2POS(pls, idx), pElem);
	else
		memmove(pElem, LIST2POS(pls, idx), pls->info.sizeData);
}
void _List2GetAtMove(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorMove))
		pls->info.fConstructorMove(LIST2POS(pls, idx), pElem);
	else
	{
		memmove(pElem, LIST2POS(pls, idx), pls->info.sizeData);
		memset(LIST2POS(pls, idx), 0, pls->info.sizeData);
	}
}

void List2Init0(list2* const pls)
{
	if (ISNOTNULLPTR(pls))
	{
		pls->status = CTNSTATUS::CTNS_NULL;
		pls->ptr = NULL;
		pls->len = 0;
		pls->cap = 0;
		InitList0Info(&(pls->info));
	}
}

int32_t List2Init(list2* const pls, const size_t size, const list0info* const pinfo)
{
	return List2InitBy(pls, LIST0INITCAP, size, pinfo);
}

int32_t List2InitPure(list2* const pls, const size_t size, const list0info* const pinfo)
{
	return List2InitByPure(pls, LIST0INITCAP, size, pinfo);
}

int32_t List2InitBy(list2* const pls, const size_t cap, const size_t size, const list0info* const pinfo)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	List2Clear(pls);
	return List2InitByPure(pls, cap, size, pinfo);
}

int32_t List2InitByPure(list2* const ls, const size_t cap, const size_t size, const list0info* const pinfo)
{
	if (ISNULLPTR(ls))
		return GRET_NULL;
	if (size <= 0)
		return GRET_INVARG;

	List2Init0(ls);
	void* const ptr = malloc(cap * size);
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	//memset(ptr, 0, cap * size);
	ls->ptr = ptr;
	ls->cap = cap;
	ls->status = CTNSTATUS::CTNS_OWN;
	if (ISNOTNULLPTR(pinfo))
		ls->info = *pinfo;
	ls->info.sizeData = size;
	const size_t sizeBase = sizeof(void*);
	ls->info.sizeUnit = (ls->info.sizeData + sizeBase - 1) / sizeBase * sizeBase;
	if (ls->info.sizeData == 0)
		return GRET_INVARG;
	return GRET_SUCCEED;
}

int32_t List2Clear(list2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		//if (0 == pls->len)
		//	return GRET_NONE;
		_List2Free(pls, 0, pls->len);
		pls->len = 0;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
		//if (0 == pls->len)
		//	return GRET_NONE;
		pls->len = 0;
		pls->ptr = NULL;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_NULL:
	case CTNSTATUS::CTNS_LOCAL:
	default:
		return GRET_INVSTATUS;
	}
	return GRET_NONE;
}

int32_t List2Abandon(list2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		//if (0 == pls->len)
		//	return GRET_NONE;
		pls->len = 0;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
		//if (0 == pls->len)
		//	return GRET_NONE;
		pls->len = 0;
		pls->ptr = NULL;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_NULL:
	case CTNSTATUS::CTNS_LOCAL:
	default:
		return GRET_INVSTATUS;
	}
	return GRET_SUCCEED;
}

int32_t List2Release(list2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_NONE;

	const int32_t ret = List2Clear(pls);
	RET_ON_NEG(ret);

	const CTNSTATUS status = pls->status;
	switch (status)
	{
	case CTNSTATUS::CTNS_OWN:
		safefree(pls->ptr);
	case CTNSTATUS::CTNS_REF:
		break;
	case CTNSTATUS::CTNS_NULL:
	case CTNSTATUS::CTNS_LOCAL:
	default:
		break;
	}
	pls->status = CTNSTATUS::CTNS_NULL;
	pls->len = 0;
	pls->cap = 0;
	pls->ptr = NULL;
	InitList0Info(&pls->info);
	return GRET_SUCCEED;
}

int32_t List2ReleaseAbandon(list2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_NONE;
	const int32_t ret = List2Abandon(pls);
	return List2Release(pls);
}

void* List2At(list2* const pls, const size_t idx)
{
	if (ISNULLPTR(pls))
		return NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return NULL;
	if (idx >= pls->len)
		return NULL;
	return LIST2POS(pls, idx);
}

void* List2Begin(list2* const pls)
{
	return List2At(pls, 0);
}

void* List2End(list2* const pls)
{
	if (ISNULLPTR(pls))
		return NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return NULL;
	return LIST2POS(pls, pls->len);
}

void* List2Next(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return NULL;
	return (void*)((char*)pElem + pls->info.sizeUnit);
}

void* List2SafeNext(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pElem))
		return NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return NULL;
	void* const pBegin = LIST2BEGIN(pls);
	if (pBegin > pElem)
		return NULL;
	const size_t diff = ((char*)pElem - (char*)pBegin);
	const size_t remain = diff % pls->info.sizeUnit;
	if (remain != 0)
		return NULL;
	const size_t idx = diff / pls->info.sizeUnit;
	if (idx >= pls->len)
		return NULL;
	return (void*)((char*)pElem + pls->info.sizeUnit);
}

int32_t List2Add(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return List2Insert(pls, pElem, pls->len);
}

int32_t List2AddMove(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return List2InsertMove(pls, pElem, pls->len);
}

int32_t List2AddFront(list2* const pls, void* const pElem)
{
	return List2Insert(pls, pElem, 0);
}

int32_t List2AddFrontMove(list2* const pls, void* const pElem)
{
	return List2InsertMove(pls, pElem, 0);
}

int32_t _List2RemoveIn(list2* const pls, const size_t idx, const size_t num)
{
	if (idx >= pls->len || num > pls->len - idx)
		return GRET_INVARG;
	_List2Free(pls, idx, num);
	_List2MoveTo(pls, idx + num, idx, pls->len - idx - num);
	pls->len -= num;
	return GRET_SUCCEED;
}

int32_t List2RemoveAt(list2* const pls, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return _List2RemoveIn(pls, idx, 1);
}

int32_t List2RemoveFront(list2* const pls, const size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return _List2RemoveIn(pls, 0, num);
}

int32_t List2RemoveBack(list2* const pls, size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (num > pls->len)
		return GRET_INVARG;
	return _List2RemoveIn(pls, pls->len - num, num);
}

int32_t List2RemoveIn(list2* const pls, const size_t idx, const size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return _List2RemoveIn(pls, idx, num);
}

int32_t List2RemoveFrom(list2* const pls, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVARG;
	return _List2RemoveIn(pls, idx, pls->len - idx);
}

int32_t List2PopAt(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List2GetAtMove(pls, idx, pElem);
	_List2MoveTo(pls, idx + 1, idx, pls->len - idx - 1);
	pls->len--;
	return GRET_SUCCEED;
}

int32_t List2PopFront(list2* const pls, void* const pElem)
{
	return List2PopAt(pls, 0, pElem);
}

int32_t List2PopBack(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return List2PopAt(pls, pls->len - 1, pElem);
}

int32_t List2Insert(list2* const pls, void* const pElem, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx > pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List2ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_List2MoveTo(pls, idx, idx + 1, pls->len - idx);
	_List2MakeAtCopy(pls, idx, pElem);
	pls->len++;
	return GRET_SUCCEED;
}

int32_t List2InsertMove(list2* const pls, void* const pElem, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx > pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List2ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_List2MoveTo(pls, idx, idx + 1, pls->len - idx);
	_List2MakeAtMove(pls, idx, pElem);
	pls->len++;
	return GRET_SUCCEED;
}

int32_t List2GetAt(list2* const pls, const size_t idx, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List2GetAtCopy(pls, idx, pElem);
	return GRET_SUCCEED;
}

int32_t List2GetFront(list2* const pls, void* const pElem)
{
	return List2GetAt(pls, 0, pElem);
}

int32_t List2GetBack(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return List2GetAt(pls, pls->len - 1, pElem);
}

int32_t List2SetAt(list2* const pls, void* const pElem, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List2SetAtCopy(pls, idx, pElem);
	return GRET_SUCCEED;
}

int32_t List2SetAtMove(list2* const pls, void* const pElem, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List2SetAtMove(pls, idx, pElem);
	return GRET_SUCCEED;
}

int32_t List2SetFront(list2* const pls, void* const pElem)
{
	return List2SetAt(pls, pElem, 0);
}

int32_t List2SetFrontMove(list2* const pls, void* const pElem)
{
	return List2SetAtMove(pls, pElem, 0);
}

int32_t List2SetBack(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (pls->len <= 0)
		return GRET_ERROR;
	_List2SetAtCopy(pls, pls->len - 1, pElem);
	return GRET_SUCCEED;
}

int32_t List2SetBackMove(list2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (pls->len <= 0)
		return GRET_ERROR;
	_List2SetAtMove(pls, pls->len - 1, pElem);
	return GRET_SUCCEED;
}

int32_t List2FindFirst(list2* const pls, void* const pElem, size_t* const pIdx, const funLS0ElemEquals fe)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	const funLS0ElemEquals fEqual = (ISNOTNULLPTR(fe)) ? fe : pls->info.fEquals;
	if (ISNULLPTR(fEqual))
		return GRET_NULLFUNC;

	for (size_t idx = 0; idx < pls->len; ++idx)
	{
		if (fEqual(LIST2POS(pls, idx), pElem))
		{
			SET_PTRVAL(pIdx, idx);
			return 1;
		}
	}
	return 0;
}

int32_t List2FindLast(list2* const pls, void* const pElem, size_t* const pIdx, const funLS0ElemEquals fe)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	const funLS0ElemEquals fEqual = (ISNOTNULLPTR(fe)) ? fe : pls->info.fEquals;
	if (ISNULLPTR(fEqual))
		return GRET_NULLFUNC;

	for (size_t idx = pls->len; idx-- > 0;)
	{
		if (fEqual(LIST2POS(pls, idx), pElem))
		{
			SET_PTRVAL(pIdx, idx);
			return 1;
		}
	}
	return 0;
}

int32_t _List2ExtendDLen(list2* const pls, const size_t dLen)
{
	size_t capNew = pls->cap;
	const int32_t ret = ListExtendDLen(pls->len, dLen, &capNew);
	RET_ON_NPOS(ret);
	return _List2ReCap(pls, capNew);
}

int32_t _List2ReCap(list2* const pls, const size_t cap)
{
	void* const ptr = realloc(pls->ptr, cap * pls->info.sizeUnit);
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	pls->ptr = ptr;
	pls->cap = cap;
	return GRET_SUCCEED;
}

int32_t List2ReCap(list2* const pls, const size_t cap)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (cap <= 0)
		return GRET_INVARG;
	if (cap < pls->len)
		_List2Free(pls, cap, pls->len - cap);
	return _List2ReCap(pls, cap);
}

int32_t List2ReLen(list2* const pls, const size_t len)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (len <= 0)
		return GRET_INVARG;
	const size_t lenOld = pls->len;
	if (lenOld > len)
		_List2Free(pls, len, lenOld - len);
	else if (len == lenOld)
		return GRET_NONE;
	else
	{
		const int32_t ret = _List2ReCap(pls, len);
		RET_ON_NEG(ret);
		_List2Make(pls, lenOld, len - lenOld);
	}
	return GRET_SUCCEED;
}

int32_t List2InitType(list2* const pls, const list2* const plsf)
{
	return List2InitTypeBy(pls, plsf, LIST0INITCAP);
}

int32_t List2InitTypeBy(list2* const pls, const list2* const plsf, const size_t cap)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (ISNULLPTR(plsf))
		return GRET_NULLARG;
	if (LIST2_ISNULL(plsf))
		return GRET_INVSTATUS;
	return List2InitBy(pls, cap, plsf->info.sizeData, &plsf->info);
}

int32_t List2CopyTo(list2* const pls, list2* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	const int32_t ret = List2InitTypeBy(plsr, pls, pls->len);
	RET_ON_NEG(ret);
	_List2CopyToList(pls, 0, plsr, 0, pls->len);
	return GRET_SUCCEED;
}

int32_t List2MoveTo(list2* const pls, list2* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	List2Release(plsr);
	plsr->cap = pls->cap;
	plsr->len = pls->len;
	plsr->ptr = pls->ptr;
	plsr->status = pls->status;
	plsr->info = pls->info;
	List2Init0(pls);
	return GRET_SUCCEED;
}
int32_t List2Swap(list2* const pls, list2* const plsr)
{
	if (ISNULLPTR(pls) || ISNULLPTR(plsr))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(plsr) && !LIST2_ISREF(plsr))
		return GRET_INVSTATUS;

	size_t tmp = 0;
	tmp = pls->cap;
	pls->cap = plsr->cap;
	plsr->cap = tmp;
	tmp = pls->len;
	pls->len = plsr->len;
	plsr->len = tmp;
	list0info tmpInfo;
	tmpInfo = pls->info;
	pls->info = plsr->info;
	plsr->info = tmpInfo;
	void* tmpPtr = pls->ptr;
	pls->ptr = plsr->ptr;
	plsr->ptr = tmpPtr;
	CTNSTATUS tmpStatus = pls->status;
	pls->status = plsr->status;
	plsr->status = tmpStatus;
	return GRET_SUCCEED;
}

int32_t List2CatList(list2* const pls, list2* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;

	size_t lenNew = pls->len + pls2->len;
	if (lenNew < pls->len)
		return GRET_OVERFLOW;
	const int32_t ret = _List2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List2CopyToList(pls2, 0, pls, pls->len, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t List2CatListMove(list2* const pls, list2* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;

	size_t lenNew = pls->len + pls2->len;
	if (lenNew < pls->len)
		return GRET_OVERFLOW;
	const int32_t ret = _List2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List2MoveToBuf(pls2, 0, LIST2POS(pls, 0), pls->len, pls2->len);
	pls->len += pls2->len;
	return List2Abandon(pls2);
}

int32_t List2InsertList(list2* const pls, list2* const pls2, const size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List2MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_List2CopyToList(pls2, 0, pls, idx, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t List2InsertListMove(list2* const pls, list2* const pls2, const size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List2MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_List2MoveToBuf(pls2, 0, LIST2POS(pls, 0), idx, pls2->len);
	pls->len += pls2->len;
	return List2Abandon(pls2);
}

#pragma endregion list2


#pragma region ptrlist2

void _PtrList2MoveTo(ptrlist2* const pls, const size_t from, const size_t to, const size_t len)
{
	memmove(PTRLIST2POS(pls, to), PTRLIST2POS(pls, from), len * sizeof(void*));
}
void _PtrList2CopyToList(ptrlist2* const pls, const size_t from, ptrlist2* const pls2, const size_t to, const size_t len)
{
	memmove(PTRLIST2POS(pls2, to), PTRLIST2POS(pls, from), len * sizeof(void*));
}
void _PtrList2DeepCopyToList(ptrlist2* const pls, const size_t from, ptrlist2* const pls2, const size_t to, const size_t len)
{
	const funPLS0ElemCopyConstructor fCopyCtor = pls->info.fConstructorCopy;
	if (ISNOTNULLPTR(fCopyCtor))
	{
		void** const plsf0 = PTRLIST2POS(pls, from);
		void** const plst0 = PTRLIST2POS(pls2, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
			{
				void* pElemCopy = NULL;
				const int32_t ret = fCopyCtor(PTRLIST2POS(pls2, to + i), &pElemCopy);
				PTRLIST2AT(pls2, to + i) = pElemCopy;
			}
		}
		else
		{
			for (size_t i = len; i-- > 0;)
			{
				void* pElemCopy = NULL;
				const int32_t ret = fCopyCtor(PTRLIST2POS(pls2, to + i), &pElemCopy);
				PTRLIST2AT(pls2, to + i) = pElemCopy;
			}
		}
	}
	else
	{
		memmove(LIST2POS(pls2, to), LIST2POS(pls, from), len * pls->info.sizeUnit);
	}
}
void _PtrList2Make(ptrlist2* const pls, const size_t from, const size_t num)
{
	const funPLS0ElemConstructorDef fCTor = pls->info.fConstructorDef;
	if (ISNOTNULLPTR(fCTor))
	{
		for (size_t idx = from; idx < num; ++idx)
		{
			void* pElem = NULL;
			const int32_t ret = fCTor(&pElem);
			if (ret < 0)
				return;
			PTRLIST2AT(pls, idx) = pElem;
		}
	}
	else
	{
		for (size_t idx = from; idx < num; ++idx)
		{
			PTRLIST2AT(pls, idx) = NULL;
		}
	}
}
void _PtrList2Clear(ptrlist2* const pls, const size_t from, const size_t num)
{
	const funPLS0ElemDestructor fDTor = pls->info.fDestructor;
	if (ISNOTNULLPTR(fDTor))
	{
		for (size_t idx = from; idx < num; ++idx)
		{
			fDTor(PTRLIST2AT(pls, idx));
		}
	}
	for (size_t idx = from; idx < num; ++idx)
	{
		safefreereset(PTRLIST2POS(pls, idx));
	}
}

int32_t _PtrList2DeepCopy(ptrlist2* const pls, void* const pElem, void** const ppElemCopy)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(ppElemCopy))
		return GRET_NULLARG;
	*ppElemCopy = NULL;
	if (ISNOTNULLPTR(pElem))
	{
		if (ISNULLPTR(pls->info.fConstructorCopy))
			return GRET_NULLFUNC;
		const int32_t ret = pls->info.fConstructorCopy(pElem, ppElemCopy);
		RET_ON_NEG(ret);
	}
	return GRET_SUCCEED;
}

void PtrList2Init0(ptrlist2* const pls)
{
	if (ISNOTNULLPTR(pls))
	{
		pls->status = CTNSTATUS::CTNS_NULL;
		pls->ptr = NULL;
		pls->len = 0;
		pls->cap = 0;
		InitPtrList0Info(&(pls->info));
	}
}

int32_t PtrList2Init(ptrlist2* const pls, const ptrlist0info* const pinfo)
{
	return PtrList2InitBy(pls, LIST0INITCAP, pinfo);
}

int32_t PtrList2InitBy(ptrlist2* const pls, const size_t cap, const ptrlist0info* const pinfo)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	PtrList2Init0(pls);
	return PtrList2InitByPure(pls, cap, pinfo);
}

int32_t PtrList2InitByPure(ptrlist2* const pls, const size_t cap, const ptrlist0info* const pinfo)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;

	if (OVERFLOW_MUL_sizet(cap, sizeof(void*)))
		return GRET_OVERFLOW;
	void** const ptr = (void**)malloc(cap * sizeof(void*));
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	//memset(ptr, 0, cap * sizeof(void*));
	pls->ptr = ptr;
	pls->cap = cap;
	pls->status = CTNSTATUS::CTNS_OWN;
	if (ISNOTNULLPTR(pinfo))
		pls->info = *pinfo;
	pls->info.sizeData = sizeof(void*);
	pls->info.sizeUnit = pls->info.sizeData;
	return GRET_SUCCEED;
}

int32_t PtrList2Clear(ptrlist2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		//if (0 == pls->len)
		//	return GRET_NONE;
		pls->len = 0;
		_PtrList2Clear(pls, 0, pls->len);
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
		//if (0 == pls->len)
		//	return GRET_NONE;
		pls->len = 0;
		pls->ptr = NULL;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_NULL:
	case CTNSTATUS::CTNS_LOCAL:
	default:
		return GRET_INVSTATUS;
	}
	return GRET_NONE;
}

int32_t PtrList2Abandon(ptrlist2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		//if (0 == pls->len)
		//	return GRET_NONE;
		pls->len = 0;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
		//if (0 == pls->len)
		//	return GRET_NONE;
		pls->len = 0;
		pls->ptr = NULL;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_NULL:
	case CTNSTATUS::CTNS_LOCAL:
	default:
		return GRET_INVSTATUS;
	}
	return GRET_SUCCEED;
}

int32_t PtrList2Release(ptrlist2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_NONE;

	const int32_t ret = PtrList2Clear(pls);
	RET_ON_NEG(ret);

	const CTNSTATUS status = pls->status;
	switch (status)
	{
	case CTNSTATUS::CTNS_OWN:
		safefree(pls->ptr);
	case CTNSTATUS::CTNS_REF:
		break;
	case CTNSTATUS::CTNS_NULL:
	case CTNSTATUS::CTNS_LOCAL:
	default:
		break;
	}
	pls->status = CTNSTATUS::CTNS_NULL;
	pls->len = 0;
	pls->cap = 0;
	pls->ptr = NULL;
	InitPtrList0Info(&pls->info);
	return GRET_SUCCEED;
}

int32_t PtrList2ReleaseAbandon(ptrlist2* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST2_ISNULL(pls))
		return GRET_NONE;
	const int32_t ret = PtrList2Abandon(pls);
	return PtrList2Release(pls);
}

int32_t PtrList2Add(ptrlist2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return PtrList2Insert(pls, pElem, pls->len);
}

int32_t PtrList2AddDeepCopy(ptrlist2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return PtrList2InsertDeepCopy(pls, pElem, pls->len);
}

int32_t PtrList2AddFront(ptrlist2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;

	const int32_t ret = _PtrList2ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_PtrList2MoveTo(pls, 0, 1, pls->len);
	PTRLIST2AT(pls, 0) = pElem;
	pls->len++;
	return GRET_SUCCEED;
}

int32_t PtrList2AddFrontDeepCopy(ptrlist2* const pls, void* const pElem)
{
	return PtrList2Insert(pls, pElem, 0);
}

int32_t _PtrList2RemoveIn(ptrlist2* const pls, const size_t idx, const size_t num)
{
	if (idx >= pls->len || num > pls->len - idx)
		return GRET_INVARG;
	_PtrList2Clear(pls, idx, num);
	_PtrList2MoveTo(pls, idx + num, idx, pls->len - idx - num);
	pls->len -= num;
	return GRET_SUCCEED;
}

int32_t PtrList2RemoveAt(ptrlist2* const pls, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return _PtrList2RemoveIn(pls, idx, 1);
}

int32_t PtrList2RemoveFront(ptrlist2* const pls, const size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return _PtrList2RemoveIn(pls, 0, num);
}

int32_t PtrList2RemoveBack(ptrlist2* const pls, const size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (num > pls->len)
		return GRET_INVARG;
	return _PtrList2RemoveIn(pls, pls->len - num, num);
}

int32_t PtrList2RemoveIn(ptrlist2* const pls, const size_t idx, const size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	return _PtrList2RemoveIn(pls, idx, num);
}

int32_t PtrList2RemoveFrom(ptrlist2* const pls, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVARG;
	return _PtrList2RemoveIn(pls, idx, pls->len - idx);
}

int32_t PtrList2PopAt(ptrlist2* const pls, const size_t idx, void** const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	*pElem = PTRLIST2AT(pls, idx);
	_PtrList2MoveTo(pls, idx + 1, idx, pls->len - idx - 1);
	pls->len--;
	return GRET_SUCCEED;
}

int32_t PtrList2PopFront(ptrlist2* const pls, void** const pElem)
{
	return PtrList2PopAt(pls, 0, pElem);
}

int32_t PtrList2PopBack(ptrlist2* const pls, void** const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return PtrList2PopAt(pls, pls->len - 1, pElem);
}

int32_t PtrList2Insert(ptrlist2* const pls, void* const pElem, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx > pls->len)
		return GRET_INVIDX;

	const int32_t ret = _PtrList2ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_PtrList2MoveTo(pls, idx, idx + 1, pls->len - idx);
	PTRLIST2AT(pls, idx) = pElem;
	pls->len++;
	return GRET_SUCCEED;
}

int32_t PtrList2InsertDeepCopy(ptrlist2* const pls, void* const pElem, const size_t idx)
{
	if (idx > pls->len)
		return GRET_INVIDX;

	void* pElemCopy = NULL;
	const int32_t ret = _PtrList2DeepCopy(pls, pElem, &pElemCopy);
	RET_ON_NEG(ret);
	return PtrList2Insert(pls, pElemCopy, idx);
}

int32_t PtrList2GetAt(ptrlist2* const pls, const size_t idx, void** const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	*pElem = PTRLIST2AT(pls, idx);
	return GRET_SUCCEED;
}

int32_t PtrList2GetFront(ptrlist2* const pls, void** const pElem)
{
	return PtrList2GetAt(pls, 0, pElem);
}

int32_t PtrList2GetBack(ptrlist2* const pls, void** const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return PtrList2GetAt(pls, pls->len - 1, pElem);
}

int32_t PtrList2SetAt(ptrlist2* const pls, void* const pElem, const size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;
	PTRLIST2AT(pls, idx) = pElem;
	return GRET_SUCCEED;
}

int32_t PtrList2SetAtDeepCopy(ptrlist2* const pls, void* const pElem, const size_t idx)
{
	if (idx >= pls->len)
		return GRET_INVIDX;

	void* pElemCopy = NULL;
	const int32_t ret = _PtrList2DeepCopy(pls, pElem, &pElemCopy);
	RET_ON_NEG(ret);
	return PtrList2SetAt(pls, pElemCopy, idx);
}

int32_t PtrList2SetFront(ptrlist2* const pls, void* const pElem)
{
	return PtrList2SetAt(pls, pElem, 0);
}

int32_t PtrList2SetFrontDeepCopy(ptrlist2* const pls, void* const pElem)
{
	return PtrList2SetAtDeepCopy(pls, pElem, 0);
}

int32_t PtrList2SetBack(ptrlist2* const pls, void* const pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	PTRLIST2AT(pls, pls->len - 1) = pElem;
	return GRET_SUCCEED;
}

int32_t PtrList2SetBackDeepCopy(ptrlist2* const pls, void* const pElem)
{
	if (pls->len <= 0)
		return GRET_ERROR;

	void* pElemCopy = NULL;
	const int32_t ret = _PtrList2DeepCopy(pls, pElem, &pElemCopy);
	RET_ON_NEG(ret);
	return PtrList2SetBack(pls, pElemCopy);
}

int32_t _PtrList2ExtendDLen(ptrlist2* const pls, const size_t dLen)
{
	size_t capNew = pls->cap;
	const int32_t ret = ListExtendDLen(pls->len, dLen, &capNew);
	RET_ON_NPOS(ret);
	return _PtrList2ReCap(pls, capNew);
}

int32_t _PtrList2ReCap(ptrlist2* const pls, const size_t cap)
{
	if (OVERFLOW_MUL_sizet(cap, sizeof(void*)))
		return GRET_OVERFLOW;
	void** const ptr = (void**)realloc(pls->ptr, cap * sizeof(void*));
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	pls->ptr = ptr;
	pls->cap = cap;
	return GRET_SUCCEED;
}

int32_t PtrList2ReCap(ptrlist2* const pls, const size_t cap)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (cap <= 0)
		return GRET_INVARG;
	if (pls->len > cap)
		_PtrList2Clear(pls, cap, pls->len - cap);
	return _PtrList2ReCap(pls, cap);
}

int32_t PtrList2ReLen(ptrlist2* const pls, const size_t len)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls))
		return GRET_INVSTATUS;
	if (len <= 0)
		return GRET_INVARG;
	const size_t lenOld = pls->len;
	if (lenOld > len)
	{
		_PtrList2Clear(pls, len, lenOld - len);
	}
	else if (len == lenOld)
	{
		return GRET_NONE;
	}
	else if (len > lenOld)
	{
		const int32_t ret = _PtrList2ReCap(pls, len);
		RET_ON_NEG(ret);
		_PtrList2Make(pls, lenOld, len - lenOld);
	}
	return GRET_SUCCEED;
}

int32_t PtrList2CopyTo(ptrlist2* const pls, ptrlist2* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	const int32_t ret = PtrList2InitBy(plsr, pls->len);
	RET_ON_NEG(ret);
	_PtrList2CopyToList(pls, 0, plsr, 0, pls->len);
	return GRET_SUCCEED;
}

int32_t PtrList2DeepCopyTo(ptrlist2* const pls, ptrlist2* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	const int32_t ret = PtrList2InitBy(plsr, pls->len);
	RET_ON_NEG(ret);
	_PtrList2CopyToList(pls, 0, plsr, 0, pls->len);
	return GRET_SUCCEED;
}

int32_t PtrList2MoveTo(ptrlist2* const pls, ptrlist2* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	PtrList2Release(plsr);
	plsr->cap = pls->cap;
	plsr->len = pls->len;
	plsr->ptr = pls->ptr;
	plsr->status = pls->status;
	plsr->info = pls->info;
	PtrList2Init0(pls);
	return GRET_SUCCEED;
}
int32_t PtrList2Swap(ptrlist2* const pls, ptrlist2* const plsr)
{
	if (ISNULLPTR(pls) || ISNULLPTR(plsr))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(plsr) && !LIST2_ISREF(plsr))
		return GRET_INVSTATUS;

	size_t tmp = 0;
	tmp = pls->cap;
	pls->cap = plsr->cap;
	plsr->cap = tmp;
	tmp = pls->len;
	pls->len = plsr->len;
	plsr->len = tmp;
	ptrlist0info tmpInfo;
	tmpInfo = pls->info;
	pls->info = plsr->info;
	plsr->info = tmpInfo;
	void** tmpPtr = pls->ptr;
	pls->ptr = plsr->ptr;
	plsr->ptr = tmpPtr;
	CTNSTATUS tmpStatus = pls->status;
	pls->status = plsr->status;
	plsr->status = tmpStatus;
	return GRET_SUCCEED;
}

int32_t PtrList2CatList(ptrlist2* const pls, ptrlist2* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;

	const int32_t ret = _PtrList2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList2CopyToList(pls2, 0, pls, pls->len, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t PtrList2CatListDeepCopy(ptrlist2* const pls, ptrlist2* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;

	const int32_t ret = _PtrList2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList2DeepCopyToList(pls2, 0, pls, pls->len, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t PtrList2InsertList(ptrlist2* const pls, ptrlist2* const pls2, const size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _PtrList2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList2MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_PtrList2CopyToList(pls2, 0, pls, idx, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t PtrList2InsertListDeepCopy(ptrlist2* const pls, ptrlist2* const pls2, const size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST2_ISOWN(pls) && !LIST2_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST2_ISOWN(pls2) && !LIST2_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _PtrList2ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList2MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_PtrList2DeepCopyToList(pls2, 0, pls, idx, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

#pragma endregion ptrlist2