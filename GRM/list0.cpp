#include "list0.h"

#include <malloc.h>
#include <wchar.h>
#include "def0.h"

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

int32_t List0Len(clist0c ls, size_t* pLen)
{
	if (LIST0ISNOTNULL(ls) && ISNOTNULLPTR(pLen))
	{
		*pLen = LIST0LEN(ls);
		return 1;
	}
	else
		return -1;
}

int32_t List0Cap(clist0c ls, size_t* pCap)
{
	if (LIST0ISNOTNULL(ls) && ISNOTNULLPTR(pCap))
	{
		*pCap = LIST0CAP(ls);
		return 1;
	}
	else
		return -1;
}

int32_t List0Status(clist0c ls, CTNSTATUS* pStatus)
{
	if (ISNOTNULLPTR(ls) && ISNOTNULLPTR(pStatus))
	{
		*pStatus = (LIST0STATUS(ls));
		return 1;
	}
	else
		return -1;
}

int32_t List0SetStatus(clist0c ls, CTNSTATUS status)
{
	if (ISNOTNULLPTR(ls))
	{
		LIST0_SETSTATUS(ls, status);
		return 1;
	}
	else
		return -1;
}

list0 _List0Make(size_t size)
{
	return _List0MakeBy(LIST0INITCAP, size);
}

list0 _List0MakeBy(size_t cap, size_t size)
{
	if (cap < 0)
		return NULL;

	list0 ls = LIST0MALLOC(cap, size);
	if (ISNULLPTR(ls))
		return NULL;
	LIST0_SETLENCAPSTATUS(ls, 0, cap, CTNSTATUS::CTNS_OWN);

	return ls;
}

int32_t _List0Add(list0* pls, void* pElem, size_t size)
{
	if (ISNULLPTR(pls))
		return -1;

	list0 ls = *pls;
	if (LIST0ISNULLP(ls))
		return -2;

	if (ISNULLPTR(pElem))
		return -3;

	//if (size <= 0)// TODO: do not need, validate with macro
	//	return -4;

	const int32_t ret = __List0TryExtendBy(&ls, 1, size);
	if (ret < 0)
		return ret;
	ls = *pls;

	const size_t lenOld = LIST0LEN(ls);
	LIST0LEN(ls) = lenOld + 1;
	memcpy(_LIST0ADDR(ls, lenOld, size), pElem, size);

	return 1;
}

int32_t _List0SetAt(list0c ls, void* pElem, size_t idx, size_t size)
{
	if (LIST0ISNULLP(ls))
		return -1;

	if (ISNULLPTR(pElem))
		return -3;

	if (idx < 0 || idx >= LIST0LEN(ls))
		return -4;

	memcpy(_LIST0ADDR(ls, idx, size), pElem, size);

	return 1;
}

int32_t List0Clear(clist0c ls)
{
	if (LIST0ISNULLP(ls))
		return -1;

	if (LIST0ISEMPTY(ls))
		return 0;

	LIST0LEN(ls) = 0;

	return 1;
}

int32_t _List0Copy(clist0c ls, list0* const plsr, size_t size)
{
	if (LIST0ISNULLP(ls) || ISNULLPTR(plsr))
		return -1;

	const size_t cap = LIST0CAP(ls);
	list0 ls2 = _List0MakeBy(cap, size);
	if (ISNULLPTR(ls2))
		return -1;

	const size_t len = LIST0LEN(ls);
	memcpy(LIST0ELEMADDR(ls2), LIST0ELEMADDR(ls), len * size);

	*plsr = ls2;

	return 1;
}

int32_t _List0ExtendBy(list0* const pls, size_t dCap, size_t size)
{
	if (ISNULLPTR(pls))
		return -1;

	list0 ls = *pls;
	if (LIST0ISNULLP(ls))
		return -1;

	const size_t cap = LIST0CAP(ls);
	size_t capNew = cap + dCap;
	if (capNew <= cap)// should extend
		return -2;

	const size_t capNew2 = (size_t)(cap * LIST0EXTENDRATE);
	capNew = max(capNew, capNew2);

	return __List0ReCap(pls, capNew, size);
}

int32_t __List0TryExtendBy(list0* pls, size_t len, size_t size)
{
	list0 ls = *pls;

	const size_t lenOld = LIST0LEN(ls);
	size_t lenNew = lenOld + len;
	if (lenNew <= lenOld)// should extend
		return -2;

	const size_t cap = LIST0CAP(ls);
	if (lenNew > cap)
	{
		return __List0ReCap(pls, lenNew, size);
	}

	return 0;
}

int32_t __List0ReCap(list0* const pls, size_t cap, size_t size)
{
	list0 ls = LIST0REALLOC(*pls, cap, size);
	if (ISNULLPTR(ls))
		return -3;
	*pls = ls;
	return 1;
}

int32_t List0Release(list0c ls)
{
	if (ISNULLPTR(ls))
		return 0;

	const CTNSTATUS status = LIST0STATUS(ls);
	switch (status)
	{
	case CTNSTATUS::CTNS_NULL:
		return 0;
	case CTNSTATUS::CTNS_OWN:
	case CTNSTATUS::CTNS_REF:
		//LIST0_SETSTATUS(ls, CTNSTATUS::CTNS_NULL);
		free(ls);
		return 1;
	default:
		return 0;
	}

	return 0;
}

#pragma region list1

static const size_t szList1 = sizeof(list1);

void _List1MoveTo(list1* const pls, size_t from, size_t to, size_t len)
{
	const funLS0ElemMoveConstructor fMoveCtor = pls->info.fConstructorMove;
	if (ISNOTNULLPTR(fMoveCtor))
	{
		void* const plsf0 = LIST1POS(pls, from);
		void* const plst0 = LIST1POS(pls, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fMoveCtor(LIST1POS(pls, to + i), LIST1POS(pls, from + i));
		}
		else
		{
			size_t i = len;
			while (i-- > 0)
				fMoveCtor(LIST1POS(pls, to + i), LIST1POS(pls, from + i));
		}
	}
	else
	{
		memmove(LIST1POS(pls, to), LIST1POS(pls, from), len * pls->info.size);
	}
}
inline void _List1MoveToBuf(list1* const pls, size_t from, void* pDst, size_t to, size_t len)
{
	const funLS0ElemMoveConstructor fMoveCtor = pls->info.fConstructorMove;
	void* const plst0 = (void*)((char*)pDst + to * pls->info.size);
	if (ISNOTNULLPTR(fMoveCtor))
	{
		void* const plsf0 = LIST1POS(pls, from);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fMoveCtor((char*)pDst + (to + i) * pls->info.size, LIST1POS(pls, from + i));
		}
		else
		{
			for (size_t i = len; i-- > 0;)
				fMoveCtor((char*)pDst + (to + i) * pls->info.size, LIST1POS(pls, from + i));
		}
	}
	else
	{
		memmove(plst0, LIST1POS(pls, from), len * pls->info.size);
	}
}

inline void _List1MoveFromBuf(list1* const pls, size_t to, void* pSrc, size_t from, size_t len)
{
	const funLS0ElemMoveConstructor fMoveCtor = pls->info.fConstructorMove;
	void* const plsf0 = (void*)((char*)pSrc + from * pls->info.size);
	if (ISNOTNULLPTR(fMoveCtor))
	{
		void* const plst0 = LIST1POS(pls, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fMoveCtor(LIST1POS(pls, to + i), (char*)pSrc + (from + i) * pls->info.size);
		}
		else
		{
			for (size_t i = len; i-- > 0;)
				fMoveCtor(LIST1POS(pls, to + i), (char*)pSrc + (from + i) * pls->info.size);
		}
	}
	else
	{
		memmove(LIST1POS(pls, to), plsf0, len * pls->info.size);
	}
}

void _List1CopyToList(list1* const pls, size_t from, list1* const pls2, size_t to, size_t len)
{
	const funLS0ElemCopyConstructor fCopyCtor = pls->info.fConstructorCopy;
	if (ISNOTNULLPTR(fCopyCtor))
	{
		void* const plsf0 = LIST1POS(pls, from);
		void* const plst0 = LIST1POS(pls2, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
				fCopyCtor(LIST1POS(pls2, to + i), LIST1POS(pls, from + i));
		}
		else
		{
			for (size_t i = len; i-- > 0;)
				fCopyCtor(LIST1POS(pls2, to + i), LIST1POS(pls, from + i));
		}
	}
	else
	{
		memmove(LIST1POS(pls2, to), LIST1POS(pls, from), len * pls->info.size);
	}
}
void _List1Make(list1* const pls, size_t from, size_t num)
{
	const funLS0ElemConstructorDef fCTor = pls->info.fConstructorDef;
	if (ISNOTNULLPTR(fCTor))
	{
		for (size_t idx = from; idx < num; ++idx)
			fCTor(LIST1POS(pls, idx));
	}
	else
	{
		memset(LIST1POS(pls, from), 0, num * sizeof(pls->info.size));
	}
}
void _List1Clear(list1* const pls, size_t from, size_t num)
{
	const funLS0ElemDestructor fDTor = pls->info.fDestructor;
	if (ISNOTNULLPTR(fDTor))
	{
		for (size_t idx = from; idx < num; ++idx)
			fDTor(LIST1POS(pls, idx));
	}
}
void _List1MakeAtCopy(list1* const pls, size_t idx, void* pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorCopy))
		pls->info.fConstructorCopy(pElem, LIST1POS(pls, idx));
	else
		memmove(LIST1POS(pls, idx), pElem, pls->info.size);
}
void _List1MakeAtMove(list1* const pls, size_t idx, void* pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorMove))
		pls->info.fConstructorMove(pElem, LIST1POS(pls, idx));
	else
	{
		memmove(LIST1POS(pls, idx), pElem, pls->info.size);
		memset(pElem, 0, pls->info.size);
	}
}
void _List1SetAtCopy(list1* const pls, size_t idx, void* pElem)
{
	if (ISNOTNULLPTR(pls->info.fAssignCopy))
		pls->info.fAssignCopy(pElem, LIST1POS(pls, idx));
	else
		memmove(LIST1POS(pls, idx), pElem, pls->info.size);
}
void _List1SetAtMove(list1* const pls, size_t idx, void* pElem)
{
	if (ISNOTNULLPTR(pls->info.fAssignMove))
		pls->info.fAssignMove(pElem, LIST1POS(pls, idx));
	else
	{
		memmove(LIST1POS(pls, idx), pElem, pls->info.size);
		memset(pElem, 0, pls->info.size);
	}
}
void _List1GetAtCopy(list1* const pls, size_t idx, void* pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorCopy))
		pls->info.fConstructorCopy(LIST1POS(pls, idx), pElem);
	else
		memmove(pElem, LIST1POS(pls, idx), pls->info.size);
}
void _List1GetAtMove(list1* const pls, size_t idx, void* pElem)
{
	if (ISNOTNULLPTR(pls->info.fConstructorMove))
		pls->info.fConstructorMove(LIST1POS(pls, idx), pElem);
	else
	{
		memmove(pElem, LIST1POS(pls, idx), pls->info.size);
		memset(LIST1POS(pls, idx), 0, pls->info.size);
	}
}

void List1Init(list1* const pls)
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

int32_t List1Make(list1* const pls, size_t size, const list0info* const pinfo)
{
	return List1MakeBy(pls, LIST0INITCAP, size, pinfo);
}

int32_t List1MakePure(list1* const pls, size_t size, const list0info* const pinfo)
{
	return List1MakeByPure(pls, LIST0INITCAP, size, pinfo);
}

int32_t List1MakeBy(list1* const pls, size_t cap, size_t size, const list0info* const pinfo)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	List1Clear(pls);
	return List1MakeByPure(pls, cap, size, pinfo);
}

int32_t List1MakeByPure(list1* const ls, size_t cap, size_t size, const list0info* const pinfo)
{
	if (ISNULLPTR(ls))
		return GRET_NULL;
	if (size <= 0)
		return GRET_INVARG;

	List1Init(ls);
	void* ptr = malloc(cap * size);
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	ls->ptr = ptr;
	ls->cap = cap;
	ls->status = CTNSTATUS::CTNS_OWN;
	if (ISNOTNULLPTR(pinfo))
		ls->info = *pinfo;
	ls->info.size = size;
	return GRET_SUCCEED;
}

int32_t List1Clear(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		_List1Clear(pls, 0, pls->len);
		pls->len = 0;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
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

int32_t List1Abandon(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		pls->len = 0;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
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

int32_t List1Release(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_NONE;

	const int32_t ret = List1Clear(pls);
	if (ret < 0)
		return ret;

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

int32_t List1ReleaseAbandon(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_NONE;
	const int32_t ret = List1Abandon(pls);
	return List1Release(pls);
}

int32_t List1Add(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return List1Insert(pls, pElem, pls->len);
}

int32_t List1AddMove(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return List1InsertMove(pls, pElem, pls->len);
}

int32_t List1AddFront(list1* const pls, void* pElem)
{
	return List1Insert(pls, pElem, 0);
}

int32_t List1AddFrontMove(list1* const pls, void* pElem)
{
	return List1InsertMove(pls, pElem, 0);
}

int32_t _List1RemoveIn(list1* const pls, size_t idx, size_t num)
{
	if (idx >= pls->len || num > pls->len - idx)
		return GRET_INVARG;
	_List1Clear(pls, idx, num);
	_List1MoveTo(pls, idx + num, idx, pls->len - idx - num);
	pls->len -= num;
	return GRET_SUCCEED;
}

int32_t List1RemoveAt(list1* const pls, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _List1RemoveIn(pls, idx, 1);
}

int32_t List1RemoveFront(list1* const pls, size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _List1RemoveIn(pls, 0, num);
}

int32_t List1RemoveBack(list1* const pls, size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (num > pls->len)
		return GRET_INVARG;
	return _List1RemoveIn(pls, pls->len - num, num);
}

int32_t List1RemoveIn(list1* const pls, size_t idx, size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _List1RemoveIn(pls, idx, num);
}

int32_t List1RemoveFrom(list1* const pls, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVARG;
	return _List1RemoveIn(pls, idx, pls->len - idx);
}

int32_t List1PopAt(list1* const pls, size_t idx, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List1GetAtMove(pls, idx, pElem);
	_List1MoveTo(pls, idx + 1, idx, pls->len - idx - 1);
	pls->len--;
	return GRET_SUCCEED;
}

int32_t List1PopFront(list1* const pls, void* pElem)
{
	return List1PopAt(pls, 0, pElem);
}

int32_t List1PopBack(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return List1PopAt(pls, pls->len - 1, pElem);
}

int32_t List1Insert(list1* const pls, void* pElem, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx > pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List1ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_List1MoveTo(pls, idx, idx + 1, pls->len - idx);
	_List1MakeAtCopy(pls, idx, pElem);
	pls->len++;
	return GRET_SUCCEED;
}

int32_t List1InsertMove(list1* const pls, void* pElem, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNOTNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx > pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List1ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_List1MoveTo(pls, idx, idx + 1, pls->len - idx);
	_List1MakeAtMove(pls, idx, pElem);
	pls->len++;
	return GRET_SUCCEED;
}

int32_t List1GetAt(list1* const pls, size_t idx, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNOTNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List1GetAtCopy(pls, idx, pElem);
	return GRET_SUCCEED;
}

int32_t List1GetFront(list1* const pls, void* pElem)
{
	return List1GetAt(pls, 0, pElem);
}

int32_t List1GetBack(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return List1GetAt(pls, pls->len - 1, pElem);
}

int32_t List1SetAt(list1* const pls, void* pElem, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List1SetAtCopy(pls, idx, pElem);
	return GRET_SUCCEED;
}

int32_t List1SetAtMove(list1* const pls, void* pElem, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	_List1SetAtMove(pls, idx, pElem);
	return GRET_SUCCEED;
}

int32_t List1SetFront(list1* const pls, void* pElem)
{
	return List1SetAt(pls, pElem, 0);
}

int32_t List1SetFrontMove(list1* const pls, void* pElem)
{
	return List1SetAtMove(pls, pElem, 0);
}

int32_t List1SetBack(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNOTNULLPTR(pElem))
		return GRET_NULLARG;
	if (pls->len <= 0)
		return GRET_ERROR;
	_List1SetAtCopy(pls, pls->len - 1, pElem);
	return GRET_SUCCEED;
}

int32_t List1SetBackMove(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNOTNULLPTR(pElem))
		return GRET_NULLARG;
	if (pls->len <= 0)
		return GRET_ERROR;
	_List1SetAtMove(pls, pls->len - 1, pElem);
	return GRET_SUCCEED;
}

int32_t List1FindFirst(list1* const pls, void* pElem, size_t* pIdx, funLS0ElemEquals fe)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNOTNULLPTR(pElem))
		return GRET_NULLARG;
	const funLS0ElemEquals fEqual = (ISNOTNULLPTR(fe)) ? fe : pls->info.fEquals;
	if (ISNULLPTR(fEqual))
		return GRET_NULLFUNC;

	for (size_t idx = 0; idx < pls->len; ++idx)
	{
		if (fEqual(LIST1POS(pls, idx), pElem))
		{
			SET_PTRVAL(pIdx, idx);
			return 1;
		}
	}
	return 0;
}

int32_t List1FindLast(list1* const pls, void* pElem, size_t* pIdx, funLS0ElemEquals fe)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNOTNULLPTR(pElem))
		return GRET_NULLARG;
	const funLS0ElemEquals fEqual = (ISNOTNULLPTR(fe)) ? fe : pls->info.fEquals;
	if (ISNULLPTR(fEqual))
		return GRET_NULLFUNC;

	for (size_t idx = pls->len; idx-- > 0;)
	{
		if (fEqual(LIST1POS(pls, idx), pElem))
		{
			SET_PTRVAL(pIdx, idx);
			return 1;
		}
	}
	return 0;
}

int32_t _List1ExtendDLen(list1* const pls, size_t dLen)
{
	const size_t lenOld = pls->len;
	size_t lenNew = lenOld + dLen;
	if (lenNew < lenOld)
		return GRET_OVERFLOW;
	if (lenNew > pls->cap)
	{
		const size_t capNew = max(lenNew, pls->cap * 2);
		if (capNew > pls->cap)
			return _List1ReCap(pls, lenNew);
	}
	return GRET_NONE;
}

int32_t _List1ReCap(list1* const pls, size_t cap)
{
	void* ptr = realloc(pls->ptr, cap * pls->info.size);
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	pls->ptr = ptr;
	pls->cap = cap;
	return GRET_SUCCEED;
}

int32_t List1ReCap(list1* const pls, size_t cap)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _List1ReCap(pls, cap);
}

int32_t List1ReLen(list1* const pls, size_t len)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (len <= 0)
		return GRET_INVARG;
	size_t lenOld = pls->len;
	if (lenOld > len)
	{
		_List1Clear(pls, len, lenOld - len);
	}
	else if (len == lenOld)
	{
		return GRET_NONE;
	}
	else if (len > lenOld)
	{
		int ret = _List1ReCap(pls, len);
		RET_ON_NEG(ret);
		_List1Make(pls, lenOld, len - lenOld);
	}
	return GRET_SUCCEED;
}

int32_t List1MakeType(list1* const pls, const list1* const plsf)
{
	return List1MakeTypeBy(pls, plsf, LIST0INITCAP);
}

int32_t List1MakeTypeBy(list1* const pls, const list1* const plsf, size_t cap)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (ISNULLPTR(plsf))
		return GRET_NULLARG;
	if (LIST1_ISNULL(plsf))
		return GRET_INVSTATUS;
	return List1MakeBy(pls, cap, plsf->info.size, &plsf->info);
}

int32_t List1CopyTo(list1* const pls, list1* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	int ret = List1MakeTypeBy(plsr, pls, pls->len);
	RET_ON_NEG(ret);
	_List1CopyToList(pls, 0, plsr, 0, pls->len);
	return GRET_SUCCEED;
}

int32_t List1MoveTo(list1* const pls, list1* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	List1Release(plsr);
	plsr->cap = pls->cap;
	plsr->len = pls->len;
	plsr->ptr = pls->ptr;
	plsr->status = pls->status;
	plsr->info = pls->info;
	List1Init(pls);
	return GRET_SUCCEED;
}
int32_t List1Swap(list1* const pls, list1* const plsr)
{
	if (ISNULLPTR(pls) || ISNULLPTR(plsr))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(plsr) && !LIST1_ISREF(plsr))
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

int32_t List1CatList(list1* const pls, list1* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;

	size_t lenNew = pls->len + pls2->len;
	if (lenNew < pls->len)
		return GRET_OVERFLOW;
	int ret = _List1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List1CopyToList(pls2, 0, pls, pls->len, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t List1CatListMove(list1* const pls, list1* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;

	size_t lenNew = pls->len + pls2->len;
	if (lenNew < pls->len)
		return GRET_OVERFLOW;
	int ret = _List1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List1MoveToBuf(pls2, 0, LIST1POS(pls, 0), pls->len, pls2->len);
	pls->len += pls2->len;
	return List1Abandon(pls2);
}

int32_t List1InsertList(list1* const pls, list1* const pls2, size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List1MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_List1CopyToList(pls2, 0, pls, idx, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t List1InsertListMove(list1* const pls, list1* const pls2, size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _List1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_List1MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_List1MoveToBuf(pls2, 0, LIST1POS(pls, 0), idx, pls2->len);
	pls->len += pls2->len;
	return List1Abandon(pls2);
}

#pragma endregion list1

void _PtrList1MoveTo(ptrlist1* const pls, size_t from, size_t to, size_t len)
{
	memmove(PTRLIST1POS(pls, to), PTRLIST1POS(pls, from), len * sizeof(void*));
}
void _PtrList1CopyToList(ptrlist1* const pls, size_t from, ptrlist1* const pls2, size_t to, size_t len)
{
	memmove(PTRLIST1POS(pls2, to), PTRLIST1POS(pls, from), len * sizeof(void*));
}
void _PtrList1DeepCopyToList(ptrlist1* const pls, size_t from, ptrlist1* const pls2, size_t to, size_t len)
{
	const funPLS0ElemCopyConstructor fCopyCtor = pls->info.fConstructorCopy;
	if (ISNOTNULLPTR(fCopyCtor))
	{
		void** const plsf0 = PTRLIST1POS(pls, from);
		void** const plst0 = PTRLIST1POS(pls2, to);
		if (plsf0 > plst0)
		{
			for (size_t i = 0; i < len; ++i)
			{
				void* pElemCopy = NULL;
				int ret = fCopyCtor(PTRLIST1POS(pls2, to + i), &pElemCopy);
				PTRLIST1AT(pls2, to + i) = pElemCopy;
			}
		}
		else
		{
			for (size_t i = len; i-- > 0;)
			{
				void* pElemCopy = NULL;
				int ret = fCopyCtor(PTRLIST1POS(pls2, to + i), &pElemCopy);
				PTRLIST1AT(pls2, to + i) = pElemCopy;
			}
		}
	}
	else
	{
		memmove(LIST1POS(pls2, to), LIST1POS(pls, from), len * pls->info.size);
	}
}
void _PtrList1Make(ptrlist1* const pls, size_t from, size_t num)
{
	const funPLS0ElemConstructorDef fCTor = pls->info.fConstructorDef;
	if (ISNOTNULLPTR(fCTor))
	{
		for (size_t idx = from; idx < num; ++idx)
		{
			void* pElem = NULL;
			int32_t ret = fCTor(&pElem);
			if (ret < 0)
				return;
			PTRLIST1AT(pls, idx) = pElem;
		}
	}
	else
	{
		for (size_t idx = from; idx < num; ++idx)
		{
			PTRLIST1AT(pls, idx) = NULL;
		}
	}
}
void _PtrList1Clear(ptrlist1* const pls, size_t from, size_t num)
{
	const funPLS0ElemDestructor fDTor = pls->info.fDestructor;
	if (ISNOTNULLPTR(fDTor))
	{
		for (size_t idx = from; idx < num; ++idx)
		{
			fDTor(PTRLIST1AT(pls, idx));
		}
	}
	for (size_t idx = from; idx < num; ++idx)
	{
		safefreeandreset(PTRLIST1POS(pls, idx));
	}
}

int32_t _PtrList1DeepCopy(ptrlist1* const pls, void* pElem, void** ppElemCopy)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(ppElemCopy))
		return GRET_NULLARG;
	*ppElemCopy = NULL;
	if (ISNOTNULLPTR(pElem))
	{
		if (ISNULLPTR(pls->info.fConstructorCopy))
			return GRET_NULLFUNC;
		int32_t ret = pls->info.fConstructorCopy(pElem, ppElemCopy);
		RET_ON_NEG(ret);
	}
	return GRET_SUCCEED;
}

void PtrList1Init(ptrlist1* const pls)
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

int32_t PtrList1Make(ptrlist1* const pls, const ptrlist0info* const pinfo)
{
	return PtrList1MakeBy(pls, LIST0INITCAP, pinfo);
}

int32_t PtrList1MakeBy(ptrlist1* const pls, size_t cap, const ptrlist0info* const pinfo)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	PtrList1Init(pls);
	return PtrList1MakeByPure(pls, cap, pinfo);
}

int32_t PtrList1MakeByPure(ptrlist1* const pls, size_t cap, const ptrlist0info* const pinfo)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;

	void** ptr = (void**)malloc(cap * sizeof(void*));
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	pls->ptr = ptr;
	pls->cap = cap;
	pls->status = CTNSTATUS::CTNS_OWN;
	if (ISNOTNULLPTR(pinfo))
		pls->info = *pinfo;
	pls->info.size = sizeof(void*);
	return GRET_SUCCEED;
}

int32_t PtrList1Clear(ptrlist1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		pls->len = 0;
		_PtrList1Clear(pls, 0, pls->len);
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
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

int32_t PtrList1Abandon(ptrlist1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_INVSTATUS;

	switch (pls->status)
	{
	case CTNSTATUS::CTNS_OWN:
		pls->len = 0;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
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

int32_t PtrList1Release(ptrlist1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_NONE;

	const int32_t ret = PtrList1Clear(pls);
	if (ret < 0)
		return ret;

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

int32_t PtrList1ReleaseAbandon(ptrlist1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_NONE;
	const int32_t ret = PtrList1Abandon(pls);
	return PtrList1Release(pls);
}

int32_t PtrList1Add(ptrlist1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return PtrList1Insert(pls, pElem, pls->len);
}

int32_t PtrList1AddDeepCopy(ptrlist1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return PtrList1InsertDeepCopy(pls, pElem, pls->len);
}

int32_t PtrList1AddFront(ptrlist1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;

	const int32_t ret = _PtrList1ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_PtrList1MoveTo(pls, 0, 1, pls->len);
	PTRLIST1AT(pls, 0) = pElem;
	pls->len++;
	return GRET_SUCCEED;
}

int32_t PtrList1AddFrontDeepCopy(ptrlist1* const pls, void* pElem)
{
	return PtrList1Insert(pls, pElem, 0);
}

int32_t _PtrList1RemoveIn(ptrlist1* const pls, size_t idx, size_t num)
{
	if (idx >= pls->len || num > pls->len - idx)
		return GRET_INVARG;
	_PtrList1Clear(pls, idx, num);
	_PtrList1MoveTo(pls, idx + num, idx, pls->len - idx - num);
	pls->len -= num;
	return GRET_SUCCEED;
}

int32_t PtrList1RemoveAt(ptrlist1* const pls, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _PtrList1RemoveIn(pls, idx, 1);
}

int32_t PtrList1RemoveFront(ptrlist1* const pls, size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _PtrList1RemoveIn(pls, 0, num);
}

int32_t PtrList1RemoveBack(ptrlist1* const pls, size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (num > pls->len)
		return GRET_INVARG;
	return _PtrList1RemoveIn(pls, pls->len - num, num);
}

int32_t PtrList1RemoveIn(ptrlist1* const pls, size_t idx, size_t num)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _PtrList1RemoveIn(pls, idx, num);
}

int32_t PtrList1RemoveFrom(ptrlist1* const pls, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVARG;
	return _PtrList1RemoveIn(pls, idx, pls->len - idx);
}

int32_t PtrList1PopAt(ptrlist1* const pls, size_t idx, void** pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	*pElem = PTRLIST1AT(pls, idx);
	_PtrList1MoveTo(pls, idx + 1, idx, pls->len - idx - 1);
	pls->len--;
	return GRET_SUCCEED;
}

int32_t PtrList1PopFront(ptrlist1* const pls, void** pElem)
{
	return PtrList1PopAt(pls, 0, pElem);
}

int32_t PtrList1PopBack(ptrlist1* const pls, void** pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return PtrList1PopAt(pls, pls->len - 1, pElem);
}

int32_t PtrList1Insert(ptrlist1* const pls, void* pElem, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx > pls->len)
		return GRET_INVIDX;

	const int32_t ret = _PtrList1ExtendDLen(pls, 1);
	RET_ON_NEG(ret);
	_PtrList1MoveTo(pls, idx, idx + 1, pls->len - idx);
	PTRLIST1AT(pls, idx) = pElem;
	pls->len++;
	return GRET_SUCCEED;
}

int32_t PtrList1InsertDeepCopy(ptrlist1* const pls, void* pElem, size_t idx)
{
	if (idx > pls->len)
		return GRET_INVIDX;

	void* pElemCopy = NULL;
	int32_t ret = _PtrList1DeepCopy(pls, pElem, &pElemCopy);
	RET_ON_NEG(ret);
	return PtrList1Insert(pls, pElemCopy, idx);
}

int32_t PtrList1GetAt(ptrlist1* const pls, size_t idx, void** pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(pElem))
		return GRET_NULLARG;
	if (idx >= pls->len)
		return GRET_INVIDX;
	*pElem = PTRLIST1AT(pls, idx);
	return GRET_SUCCEED;
}

int32_t PtrList1GetFront(ptrlist1* const pls, void** pElem)
{
	return PtrList1GetAt(pls, 0, pElem);
}

int32_t PtrList1GetBack(ptrlist1* const pls, void** pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	return PtrList1GetAt(pls, pls->len - 1, pElem);
}

int32_t PtrList1SetAt(ptrlist1* const pls, void* pElem, size_t idx)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;
	PTRLIST1AT(pls, idx) = pElem;
	return GRET_SUCCEED;
}

int32_t PtrList1SetAtDeepCopy(ptrlist1* const pls, void* pElem, size_t idx)
{
	if (idx >= pls->len)
		return GRET_INVIDX;

	void* pElemCopy = NULL;
	int32_t ret = _PtrList1DeepCopy(pls, pElem, &pElemCopy);
	RET_ON_NEG(ret);
	return PtrList1SetAt(pls, pElemCopy, idx);
}

int32_t PtrList1SetFront(ptrlist1* const pls, void* pElem)
{
	return PtrList1SetAt(pls, pElem, 0);
}

int32_t PtrList1SetFrontDeepCopy(ptrlist1* const pls, void* pElem)
{
	return PtrList1SetAtDeepCopy(pls, pElem, 0);
}

int32_t PtrList1SetBack(ptrlist1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (pls->len <= 0)
		return GRET_ERROR;
	PTRLIST1AT(pls, pls->len - 1) = pElem;
	return GRET_SUCCEED;
}

int32_t PtrList1SetBackDeepCopy(ptrlist1* const pls, void* pElem)
{
	if (pls->len <= 0)
		return GRET_ERROR;

	void* pElemCopy = NULL;
	int32_t ret = _PtrList1DeepCopy(pls, pElem, &pElemCopy);
	RET_ON_NEG(ret);
	return PtrList1SetBack(pls, pElemCopy);
}

int32_t _PtrList1ExtendDLen(ptrlist1* const pls, size_t dLen)
{
	const size_t lenOld = pls->len;
	size_t lenNew = lenOld + dLen;
	if (lenNew < lenOld)
		return GRET_OVERFLOW;
	if (lenNew > pls->cap)
	{
		const size_t capNew = max(lenNew, pls->cap * 2);
		if (capNew > pls->cap)
			return _PtrList1ReCap(pls, lenNew);
	}
	return GRET_NONE;
}

int32_t _PtrList1ReCap(ptrlist1* const pls, size_t cap)
{
	void** ptr = (void**)realloc(pls->ptr, cap * sizeof(void*));
	if (ISNULLPTR(ptr))
		return GRET_MALLOC;
	pls->ptr = ptr;
	pls->cap = cap;
	return GRET_SUCCEED;
}

int32_t PtrList1ReCap(ptrlist1* const pls, size_t cap)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	return _PtrList1ReCap(pls, cap);
}

int32_t PtrList1ReLen(ptrlist1* const pls, size_t len)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (len <= 0)
		return GRET_INVARG;
	size_t lenOld = pls->len;
	if (lenOld > len)
	{
		_PtrList1Clear(pls, len, lenOld - len);
	}
	else if (len == lenOld)
	{
		return GRET_NONE;
	}
	else if (len > lenOld)
	{
		int ret = _PtrList1ReCap(pls, len);
		RET_ON_NEG(ret);
		_PtrList1Make(pls, lenOld, len - lenOld);
	}
	return GRET_SUCCEED;
}

int32_t PtrList1CopyTo(ptrlist1* const pls, ptrlist1* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	int ret = PtrList1MakeBy(plsr, pls->len);
	RET_ON_NEG(ret);
	_PtrList1CopyToList(pls, 0, plsr, 0, pls->len);
	return GRET_SUCCEED;
}

int32_t PtrList1DeepCopyTo(ptrlist1* const pls, ptrlist1* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	int ret = PtrList1MakeBy(plsr, pls->len);
	RET_ON_NEG(ret);
	_PtrList1CopyToList(pls, 0, plsr, 0, pls->len);
	return GRET_SUCCEED;
}

int32_t PtrList1MoveTo(ptrlist1* const pls, ptrlist1* const plsr)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (ISNULLPTR(plsr))
		return GRET_NULLARG;

	PtrList1Release(plsr);
	plsr->cap = pls->cap;
	plsr->len = pls->len;
	plsr->ptr = pls->ptr;
	plsr->status = pls->status;
	plsr->info = pls->info;
	PtrList1Init(pls);
	return GRET_SUCCEED;
}
int32_t PtrList1Swap(ptrlist1* const pls, ptrlist1* const plsr)
{
	if (ISNULLPTR(pls) || ISNULLPTR(plsr))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(plsr) && !LIST1_ISREF(plsr))
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

int32_t PtrList1CatList(ptrlist1* const pls, ptrlist1* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;

	size_t lenNew = pls->len + pls2->len;
	if (lenNew < pls->len)
		return GRET_OVERFLOW;
	int ret = _PtrList1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList1CopyToList(pls2, 0, pls, pls->len, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t PtrList1CatListDeepCopy(ptrlist1* const pls, ptrlist1* const pls2)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;

	size_t lenNew = pls->len + pls2->len;
	if (lenNew < pls->len)
		return GRET_OVERFLOW;
	int ret = _PtrList1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList1DeepCopyToList(pls2, 0, pls, pls->len, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t PtrList1InsertList(ptrlist1* const pls, ptrlist1* const pls2, size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _PtrList1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList1MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_PtrList1CopyToList(pls2, 0, pls, idx, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}

int32_t PtrList1InsertListDeepCopy(ptrlist1* const pls, ptrlist1* const pls2, size_t idx)
{
	if (ISNULLPTR(pls) || ISNULLPTR(pls2))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls) && !LIST1_ISREF(pls))
		return GRET_INVSTATUS;
	if (!LIST1_ISOWN(pls2) && !LIST1_ISREF(pls2))
		return GRET_INVSTATUS;
	if (idx >= pls->len)
		return GRET_INVIDX;

	const int32_t ret = _PtrList1ExtendDLen(pls, pls2->len);
	RET_ON_NEG(ret);
	_PtrList1MoveTo(pls, idx, idx + pls2->len, pls->len - idx);
	_PtrList1DeepCopyToList(pls2, 0, pls, idx, pls2->len);
	pls->len += pls2->len;
	return GRET_SUCCEED;
}