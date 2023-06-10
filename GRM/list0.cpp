#include "list0.h"

#include <malloc.h>
#include <wchar.h>
#include "def0.h"

void InitList0Info(list0info* const pInfo)
{
	if (NULL != pInfo)
	{
		//pInfo->fEqualsKey		= NULL;
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
	if (LIST0ISNOTNULL(ls) && NULL != pLen)
	{
		*pLen = LIST0LEN(ls);
		return 1;
	}
	else
		return -1;
}

int32_t List0Cap(clist0c ls, size_t* pCap)
{
	if (LIST0ISNOTNULL(ls) && NULL != pCap)
	{
		*pCap = LIST0CAP(ls);
		return 1;
	}
	else
		return -1;
}

int32_t List0Status(clist0c ls, CTNSTATUS* pStatus)
{
	if (NULL != ls && NULL != pStatus)
	{
		*pStatus = (LIST0STATUS(ls));
		return 1;
	}
	else
		return -1;
}

int32_t List0SetStatus(clist0c ls, CTNSTATUS status)
{
	if (NULL != ls)
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
	if (NULL == ls)
		return NULL;
	LIST0_SETLENCAPSTATUS(ls, 0, cap, CTNSTATUS::CTNS_OWN);

	return ls;
}

int32_t _List0Add(list0* pls, void* pElem, size_t size)
{
	if (NULL == pls)
		return -1;

	list0 ls = *pls;
	if (LIST0ISNULLP(ls))
		return -2;

	if (NULL == pElem)
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

	if (NULL == pElem)
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
	if (LIST0ISNULLP(ls) || NULL == plsr)
		return -1;

	const size_t cap = LIST0CAP(ls);
	list0 ls2 = _List0MakeBy(cap, size);
	if (NULL == ls2)
		return -1;

	const size_t len = LIST0LEN(ls);
	memcpy(LIST0ELEMADDR(ls2), LIST0ELEMADDR(ls), len * size);

	*plsr = ls2;

	return 1;
}

int32_t _List0ExtendBy(list0* const pls, size_t dCap, size_t size)
{
	if (NULL == pls)
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
	if (NULL == ls)
		return -3;
	*pls = ls;
	return 1;
}

int32_t List0Release(list0c ls)
{
	if (NULL == ls)
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


// list1
static const size_t szList1 = sizeof(list1);

void List1Init(list1* const pls)
{
	if (NULL != pls)
	{
		pls->status = CTNSTATUS::CTNS_NULL;
		pls->ptr = NULL;
		pls->len = 0;
		pls->cap = 0;
		InitList0Info(&(pls->info));
	}
}

int32_t List1Make(list1* const pls, size_t size, list0info* const pinfo)
{
	return List1MakeBy(pls, LIST0INITCAP, size, pinfo);
}

int32_t List1MakeBy(list1* const pls, size_t cap, size_t size, list0info* const pinfo)
{
	if (NULL == pls)
		return GRET_NULLARG;
	if (size <= 0)
		return GRET_INVARG;

	list1 ls = *pls;
	void* ptr = malloc(cap*size);
	if (NULL == ptr)
		return GRET_MALLOC;

	ls.ptr = ptr;
	ls.len = 0;
	ls.cap = cap;
	InitList0Info(&ls.info);
	ls.status = CTNSTATUS::CTNS_OWN;
	if (NULL != pinfo)
		ls.info = *pinfo;
	ls.info.size = size;
	return GRET_SUCCEED;
}

int32_t List1Clear(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;

	const funLS0ElemDestructor fDTor = pls->info.fDestructor;
	const CTNSTATUS status = pls->status;
	switch (status)
	{
	case CTNSTATUS::CTNS_NULL:
		return GRET_NONE;
	case CTNSTATUS::CTNS_OWN:
		if (fDTor)
		{
			for (size_t idx = 0; idx < pls->len; ++idx)
			{
				fDTor(LIST1POS(pls, idx));
			}
		}
		pls->len = 0;
		return GRET_SUCCEED;
	case CTNSTATUS::CTNS_REF:
	case CTNSTATUS::CTNS_LOCAL:
		return GRET_INVSTATUS;
	default:
		return GRET_NONE;
	}

	return GRET_NONE;
}

int32_t List1Abandon(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (LIST1_ISNULL(pls))
		return GRET_INVSTATUS;

	pls->len = 0;
	switch (pls->status)
	{
	case CTNSTATUS::CTNS_REF:
		return GRET_INVSTATUS;
	case CTNSTATUS::CTNS_OWN:
	case CTNSTATUS::CTNS_LOCAL:
	case CTNSTATUS::CTNS_NULL:
	default:
		break;
	}

	return GRET_SUCCEED;
}

int32_t List1Release(list1* const pls)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;

	const int32_t ret = List1Clear(pls);
	if (ret < 0)
		return ret;

	const CTNSTATUS status = pls->status;
	switch (status)
	{
	case CTNSTATUS::CTNS_OWN:
		safefree(pls->ptr);
	case CTNSTATUS::CTNS_REF:
	case CTNSTATUS::CTNS_LOCAL:
		break;
	case CTNSTATUS::CTNS_NULL:
	default:
		return GRET_NONE;
	}
	pls->status = CTNSTATUS::CTNS_NULL;
	pls->len = 0;
	pls->cap = 0;
	pls->ptr = NULL;
	InitList0Info(&pls->info);

	return GRET_NONE;
}

int32_t List1ReleaseAbandon(list1* const pls)
{
	if (NULL == pls)
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
	if (NULL == pElem)
		return GRET_NULLARG;

	const int32_t ret = _List1TryDLenExtend(pls, 1);
	if (ret < 0)
		return ret;

	_List1AddCopy(pls, pls->len, pElem);
	pls->len = pls->len + 1;
	return GRET_SUCCEED;
}

int32_t List1AddMove(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (NULL == pElem)
		return GRET_NULLARG;

	const int32_t ret = _List1TryDLenExtend(pls, 1);
	if (ret < 0)
		return ret;

	_List1AddMove(pls, pls->len, pElem);
	pls->len = pls->len + 1;
	return GRET_SUCCEED;
}

int32_t List1AddFront(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (NULL == pElem)
		return GRET_NULLARG;

	const int32_t ret = _List1TryDLenExtend(pls, 1);
	if (ret < 0)
		return ret;

	_ListMemMoveTo(pls, 0, 1, pls->len);
	_List1AddCopy(pls, 0, pElem);
	pls->len = pls->len + 1;
	return GRET_SUCCEED;
}

int32_t List1AddFrontMove(list1* const pls, void* pElem)
{
	if (ISNULLPTR(pls))
		return GRET_NULL;
	if (!LIST1_ISOWN(pls))
		return GRET_INVSTATUS;
	if (NULL == pElem)
		return GRET_NULLARG;

	const int32_t ret = _List1TryDLenExtend(pls, 1);
	if (ret < 0)
		return ret;

	_ListMemMoveTo(pls, 0, 1, pls->len);
	_List1AddMove(pls, 0, pElem);
	pls->len = pls->len + 1;
	return GRET_SUCCEED;
}

int32_t List1RemoveAt(list1* const pls, size_t idx)
{
	return GRET_SUCCEED;
}

int32_t _List1TryDLenExtend(list1* const pls, size_t dLen)
{
	const size_t lenOld = pls->len;
	size_t lenNew = lenOld + dLen;
	if (lenNew < lenOld)
		return GRET_OVERFLOW;

	const size_t cap = pls->cap;
	if (lenNew > cap)
	{
		return _List1ReCap(pls, lenNew);
	}

	return GRET_NONE;
}

int32_t _List1ReCap(list1* const pls, size_t cap)
{
	if (!LIST1_ISNULL(pls))
		return GRET_INVSTATUS;

	void* ptr = realloc(pls->ptr, cap * pls->info.size);
	if (NULL == ptr)
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
	//if(cap <= 0)

	return _List1ReCap(pls, cap);
}