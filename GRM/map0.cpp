#include "pch.h"
#define LIB_EXPORT
#include "map0.h"
#include <stdlib.h>
#include <stdio.h>


void PtrMap1InfoInit(ptrmap1info* info)
{
	if (NULL != info)
	{
		info->fID			= NULL;
		info->fEqualsKey	= NULL;
		info->fCopyCtorKey	= NULL;
		info->fCopyCtorVal	= NULL;
		info->fCopyKey		= NULL;
		info->fCopyVal		= NULL;
		info->fReleaseKey	= NULL;
		info->fReleaseVal	= NULL;
		info->fDispKey		= NULL;
		info->fDispVal		= NULL;
	}
}

void PtrMap1Init(ptrmap1* const map)
{
	if (NULL != map)
	{
		map->cap		= 0;
		map->len		= 0;
		map->pm			= NULL;
		map->pfirst		= NULL;
		map->plast		= NULL;
		PtrMap1InfoInit(&(map->info));
	}
}

int32_t PtrMap1Make(ptrmap1* const map, ptrmap1info* pInfo)
{
	if (NULL == map)
		return GRET_NULL;
	const int32_t ret = PtrMap1Release(map);
	return PtrMap1MakeByPure(map, PTRMAP1_CAP0, pInfo);
}

int32_t PtrMap1MakeBy(ptrmap1* const map, size_t cap, ptrmap1info* pInfo)
{
	if (NULL == map)
		return GRET_NULL;
	const int32_t ret = PtrMap1Release(map);
	return PtrMap1MakeByPure(map, cap, pInfo);
}

int32_t PtrMap1MakeByPure(ptrmap1* const map, size_t cap, ptrmap1info* pInfo)
{
	if (NULL == map)
		return GRET_NULL;
	cap = max(cap, 1);
	PtrMap1Init(map);
	ptrmap0node** const pm = (ptrmap0node**)malloc(cap * sizeof(ptrmap0node*));
	if (NULL == pm)
		return GRET_MALLOC;
	memset(pm, 0, cap * sizeof(ptrmap0node*));
	map->pm = pm;
	map->cap = cap;
	if (NULL != pInfo)
		map->info = *pInfo;
	return GRET_SUCCEED;
}

int32_t _PtrMap1MakeNode(const ptrmap1* const map, void* const key, void* const val, ptrmap0node** pnode)
{
	ptrmap0node* const pn = (ptrmap0node*)malloc(sizeof(ptrmap0node));
	if (NULL == pn)
		return GRET_MALLOC;
	pn->key = key;
	pn->val = val;
	pn->id = PTRMAP1_ID0(map, key);
	pn->next = NULL;
	pn->prev = NULL;
	pn->nextBuk = NULL;
	*pnode = pn;
	return GRET_SUCCEED;
}

int32_t _PtrMap1CopyNode(const ptrmap1* const map, ptrmap0node* const pnode, ptrmap0node** pnodeo)
{
	if (NULL == map->info.fCopyCtorKey || NULL == map->info.fCopyCtorVal)
		return GRET_NULL;
	void* keyCopy = NULL;
	void* valCopy = NULL;
	int ret = map->info.fCopyCtorKey(pnode->key, &keyCopy);
	RET_ON_NEG(ret);
	ret = map->info.fCopyCtorVal(pnode->val, &valCopy);
	RET_ON_NEG(ret);
	ret = _PtrMap1MakeNode(map, keyCopy, valCopy, pnodeo);
	RET_ON_NEG(ret);
	(*pnodeo)->id = pnode->id;
	return GRET_SUCCEED;
}

int32_t _PtrMap1ReleaseNode(ptrmap1* const map, ptrmap0node* const pnode)
{
	if (NULL == pnode)
		return GRET_NONE;
	SAFECALL1(map->info.fReleaseKey, pnode->key);
	SAFECALL1(map->info.fReleaseVal, pnode->val);
	free(pnode);
	return GRET_SUCCEED;
}

void _PtrMap1AttachNode(ptrmap1* const map, ptrmap0node* const pn)
{
	if (NULL == pn)
		return;

	pn->prev = NULL;
	pn->nextBuk = NULL;
	pn->next = NULL;

	const map0id_t iMap = PTRMAP1_ID(map, pn->key);
	ptrmap0node* pnBuk = map->pm[iMap];
	if (NULL == pnBuk)
	{
		map->pm[iMap] = pn;
		if (NULL != map->plast)
		{
			map->plast->next = pn;
			map->plast->nextBuk = pn;
			pn->prev = map->plast;
		}
		map->plast = pn;
	}
	else
	{
		while (NULL != pnBuk->next)
			pnBuk = pnBuk->next;
		pnBuk->next = pn;
		pn->prev = pnBuk;
		pn->nextBuk = pnBuk->nextBuk;
		//pnBuk->nextBuk = NULL;
		pn->next = pnBuk->next;
		if (NULL != pn->next)
			pn->next->prev = pn;

		// plast
		if (pnBuk == map->plast)
		{
			map->plast = pn;
		}
	}
	// change strategy for ordered map
	if (NULL == map->pfirst)
	{
		map->pfirst = pn;
	}
	map->len++;
}

ptrmap0node* _PtrMap1GetPrevBuk(ptrmap1* const map, ptrmap0node* const pnode, size_t limit)
{
	if (NULL == pnode)
		return NULL;
	ptrmap0node* pprev = pnode->prev;
	for (size_t i = 0; i < limit; ++i)
	{
		if (NULL == pprev)
			return pprev;
		if (pprev->next == pprev->nextBuk)
			return pprev;
		pprev = pprev->prev;
	}
	return NULL;
}

void _PtrMap1DetachNode(ptrmap1* const map, ptrmap0node* const pn)
{
	if (NULL == pn)
		return;

	ptrmap0node* const pprev = pn->prev;
	ptrmap0node* const pnextBuk = pn->nextBuk;
	ptrmap0node* const pnext = pn->next;
	const bool bHeadOfBuk = (NULL == pprev) || (NULL != pprev && pn == pprev->nextBuk);
	if (NULL != pprev)
	{
		pprev->next = pnext;
		if (bHeadOfBuk)
		{
			pprev->nextBuk = pnext;
			ptrmap0node* pprevBuk = _PtrMap1GetPrevBuk(map, pn);
			if (NULL != pprevBuk)
				pprevBuk->nextBuk = pnext;
		}
	}
	if (NULL != pnext)
		pnext->prev = pprev;
	if (map->pfirst == pn)
		map->pfirst = pnext;
	if (map->plast == pn)
		map->plast = pprev;
	if (bHeadOfBuk)
	{
		ptrmap0node* const pnextInBuk = pn->nextBuk == pn->next ? NULL : pn->next;
		const map0id_t iMap = PTRMAP1_ID1(pn->id, map->cap);
		map->pm[iMap] = pnextInBuk;
	}
	map->len--;
	pn->next = NULL;
	pn->prev = NULL;
	pn->nextBuk = NULL;
}

int32_t _PtrMap1RemoveNode(ptrmap1* const map, ptrmap0node* const pn)
{
	if (NULL == pn)
		return GRET_NULL;
	_PtrMap1DetachNode(map, pn);
	_PtrMap1ReleaseNode(map, pn);
	return GRET_SUCCEED;
}

int32_t _PtrMap1AddNode(ptrmap1* const map, void* const key, void* const val)
{
	ptrmap0node* pnNew = NULL;
	const int32_t ret = _PtrMap1MakeNode(map, key, val, &pnNew);
	if (ret < 0)
		return ret;
	_PtrMap1AttachNode(map, pnNew);
	return GRET_SUCCEED;
}

ptrmap0node* PtrMap1Begin(ptrmap1* const map)
{
	if (PTRMAP1_ISNULLP(map))
		return NULL;
	return map->pfirst;
}

const ptrmap0node* PtrMap1BeginC(const ptrmap1* const map)
{
	if (PTRMAP1_ISNULLP(map))
		return NULL;
	return map->pfirst;
}

ptrmap0node* PtrMap1Next(const ptrmap0node* node)
{
	if (NULL == node)
		return NULL;
	return node->next;
}

int32_t PtrMap1Add(ptrmap1* const map, void* const key, void* const val)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	//if (NULL == key || NULL == val)
	//	return GRET_NULLARG;

	if (_PtrMap1HasKey(map, key))
		return GRET_ERROR;// TODO

	if (map->len > map->cap * PTRMAP1_RATESTD)
	{
		size_t capNew = (size_t)(map->cap * PTRMAP1_CAPEXTRATEDEF);
		if (capNew <= map->cap)
		{
			capNew = map->cap + PTRMAP1_CAPEXTNDEF;
			if (capNew <= map->cap) capNew = map->cap + 2;
			if (capNew <= map->cap) capNew = map->cap + 1;
			if (capNew <= map->cap) return GRET_OVERFLOW;
		}
		const int32_t ret = PtrMap1ReCap(map, capNew);
		if (ret < 0)
		{
			//if (map->cap - 1 < map->len)// NONEED
			//	return ret;
		}
	}

	return _PtrMap1AddNode(map, key, val);
}

int32_t PtrMap1GetByKey(ptrmap1* const map, void* const key, void** pval)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == key || NULL == pval)
		return GRET_NULLARG;

	const map0id_t iMap = PTRMAP1_ID(map, key);
	const ptrmap0node* pn = map->pm[iMap];
	while (NULL != pn)
	{
		if (pn->key == key) //map->fEqualsKey(pn->key, key)
		{
			*pval = pn->val;
			return GRET_SUCCEED;
		}
		if (pn->next == pn->nextBuk)
			break;
		pn = pn->next;
	}
	return GRET_NOTFOUND;
}

int32_t PtrMap1GetByVal(ptrmap1* const map, void* const val, void** pkey)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == val || NULL == pkey)
		return GRET_NULLARG;

	const ptrmap0node* pn = map->pfirst;
	while (NULL != pn)
	{
		const ptrmap0node* pInBuk = pn;
		while (NULL != pInBuk)
		{
			if (pInBuk->val == val) //map->fEqualsVal(pInBuk->val, val)
			{
				*pkey = pInBuk->val;
				return GRET_SUCCEED;
			}
			if (pInBuk->next == pInBuk->nextBuk)
				break;
			pInBuk = pInBuk->next;
		}
		pn = pn->next;
	}
	return GRET_NOTFOUND;
}

int32_t PtrMap1RemoveKey(ptrmap1* const map, void* const key)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == key)
		return GRET_NULLARG;

	const map0id_t id = PTRMAP1_ID(map, key);

	ptrmap0node* pn = map->pm[id];
	while (NULL != pn)
	{
		if (pn->key == key) //map->fEqualsKey(pn->key, key)
		{
			return _PtrMap1RemoveNode(map, pn);
		}
		if (pn->next == pn->nextBuk)
			break;
		pn = pn->next;
	}
	return GRET_NONE;
}

int32_t PtrMap1RemoveVal(ptrmap1* const map, void* const val)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == val)
		return GRET_NULLARG;

	ptrmap0node* pn = map->pfirst;
	while(NULL != pn)
	{
		ptrmap0node* pInBuk = pn;
		while (NULL != pInBuk)
		{
			if (pInBuk->val == val) //map->fEqualsKey(pInBuk->val, val)
			{
				return _PtrMap1RemoveNode(map, pInBuk);
			}
			if (pInBuk->next == pInBuk->nextBuk)
				break;
			pInBuk = pInBuk->next;
		}
		pn = pn->next;
	}

	return GRET_NONE;
}

int32_t PtrMap1Empty(ptrmap1* const map, bool* const bempty)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == bempty)
		return GRET_NULLARG;

	*bempty = 0 == map->len;
	return GRET_SUCCEED;
}

int32_t PtrMap1HasKey(ptrmap1* const map, void* const key, bool* const bhas)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == key || NULL == bhas)
		return GRET_NULLARG;

	*bhas = _PtrMap1HasKey(map, key);
	return GRET_SUCCEED;
}

bool _PtrMap1HasKey(ptrmap1* const map, void* const key)
{
	const map0id_t iMap = PTRMAP1_ID(map, key);
	const ptrmap0node* pn = map->pm[iMap];
	while (NULL != pn)
	{
		if (pn->key == key) //map->fEqualsKey(pn->key, key)
			return true;
		if (pn->next == pn->nextBuk)
			break;
		pn = pn->next;
	}
	return false;
}

int32_t PtrMap1HasVal(ptrmap1* const map, void* const val, bool* const bhas)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == bhas)
		return GRET_NULLARG;

	const ptrmap0node* pn = map->pfirst;
	while (NULL != pn)
	{
		const ptrmap0node* pInBuk = pn;
		while (NULL != pInBuk)
		{
			if (pInBuk->val == val) //map->fEqualsVal(pInBuk->val, val)
			{
				*bhas = true;
				return GRET_SUCCEED;
			}
			if (pInBuk->next == pInBuk->nextBuk)
				break;
			pInBuk = pInBuk->next;
		}
		pn = pn->next;
	}
	*bhas = false;
	return GRET_SUCCEED;
}

int32_t PtrMap1Len(ptrmap1* const map, size_t* const plen)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == plen)
		return GRET_NULLARG;
	*plen = map->len;
	return GRET_SUCCEED;
}

int32_t PtrMap1Cap(ptrmap1* const map, size_t* const pcap)
{
	if (NULL == map)
		return GRET_NULL;
	if (NULL == map->pm)
		return GRET_NONE;
	if (NULL == pcap)
		return GRET_NULLARG;
	*pcap = map->cap;
	return GRET_SUCCEED;
}

int32_t PtrMap1ReCap(ptrmap1* const map, const size_t capNew)
{
	if (NULL == map)
		return GRET_NULL;
	if (NULL == map->pm)
		return GRET_NONE;
	if (capNew <= 0)
		return GRET_INVARG;

	ptrmap0node** const pmNew = (ptrmap0node**)malloc(capNew * sizeof(ptrmap0node*));
	if (NULL == pmNew)
		return GRET_MALLOC;
	memset(pmNew, 0, capNew * sizeof(ptrmap0node*));

	ptrmap0node* const pnOld = map->pfirst;
	if (NULL == pnOld)
		return GRET_SUCCEED;

	for (ptrmap0node* pn = pnOld; pn != NULL;)
	{
		const map0id_t idOld = PTRMAP1_ID1(pn->id, map->cap);
		const map0id_t idNew = PTRMAP1_ID1(pn->id, capNew);
		map->pm[idOld] = NULL;
		pmNew[idNew] = pn;
		pn = pn->next;
	}

	safefree(map->pm);
	map->pm = pmNew;
	map->cap = capNew;

	return GRET_SUCCEED;
}

int32_t PtrMap1Clear(ptrmap1* const map)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;

	ptrmap0node* pBuk = map->pfirst;
	while (NULL != pBuk)
	{
		ptrmap0node* const pNextBuk = pBuk->nextBuk;
		ptrmap0node* pInBuk = pBuk;
		while (NULL != pInBuk)
		{
			ptrmap0node* pNextInBuk = pInBuk->next;
			const bool bFinalInBuk = pInBuk->next == pInBuk->nextBuk;
			_PtrMap1ReleaseNode(map, pInBuk);
			if (bFinalInBuk)
				break;
			pInBuk = pNextInBuk;
		}
		const map0id_t iMap = PTRMAP1_ID1(pBuk->id, map->cap);
		map->pm[iMap] = NULL;
		pBuk = pNextBuk;
	}
	map->pfirst = NULL;
	map->plast = NULL;
	map->len = 0;
	return GRET_SUCCEED;
}

int32_t PtrMap1Abandon(ptrmap1* const map)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;

	ptrmap0node* pBuk = map->pfirst;
	while (NULL != pBuk)
	{
		ptrmap0node* pNextInBuk = pBuk->next;
		ptrmap0node* pInBuk = pBuk;
		while (NULL != pInBuk)
		{
			ptrmap0node* pNextInBuk = pInBuk->next;
			const bool bFinalInBuk = pInBuk->next == pInBuk->nextBuk;
			safefree(pInBuk);
			if (bFinalInBuk)
				break;
			pInBuk = pNextInBuk;
		}
		const map0id_t iMap = PTRMAP1_ID1(pBuk->id, map->cap);
		map->pm[iMap] = NULL;
		pBuk = pNextInBuk;
	}
	map->pfirst = NULL;
	map->plast = NULL;
	map->len = 0;
	return GRET_SUCCEED;
}

int32_t PtrMap1Release(ptrmap1* const map)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_NONE;

	const int32_t ret = PtrMap1Clear(map);
	map->cap = 0;
	free(map->pm);
	map->pm = NULL;
	PtrMap1InfoInit(&(map->info));
	return GRET_SUCCEED;
}

int32_t PtrMap1ReleaseAbandon(ptrmap1* const map)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_NONE;

	const int32_t ret = PtrMap1Abandon(map);
	return PtrMap1Release(map);
}

int32_t PtrMap1Copy(const ptrmap1* const map, ptrmap1* const map2)
{
	if (NULL == map || NULL == map2)
		return GRET_NULL;
	int ret = PtrMap1MakeBy(map2, map->cap);
	RET_ON_NEG(ret);

	ptrmap0node* pBuk = map->pfirst;
	while (NULL != pBuk)
	{
		ptrmap0node* const pNextBuk = pBuk->nextBuk;
		ptrmap0node* pInBuk = pBuk;
		ptrmap0node* pPreInBuk = NULL;
		while (NULL != pInBuk)
		{
			ptrmap0node* pNextInBuk = pInBuk->next;
			const bool bFinalInBuk = pInBuk->next == pInBuk->nextBuk;
			const bool bFirstInBuk = pBuk == pInBuk;
			ptrmap0node* pCopy = NULL;
			int ret = _PtrMap1CopyNode(map, pInBuk, &pCopy);
			RET_ON_NEG(ret);

			_PtrMap1AttachNode(map2, pCopy);
			//if (bFirstInBuk)
			//{
			//	const map0id_t iMap = PTRMAP1_ID1(pCopy->id, map2->cap);
			//	map2->pm[iMap] = pCopy;
			//}
			//if (NULL == map2->pfirst)
			//	map2->pfirst = pCopy;
			//pPreInBuk = pInBuk;

			if (bFinalInBuk)
				break;
			pInBuk = pNextInBuk;
		}
		pBuk = pNextBuk;
	}
	return GRET_SUCCEED;
}

int32_t PtrMap1Move(ptrmap1* const map, ptrmap1* const map2)
{
	if (NULL == map || NULL == map2)
		return GRET_NULL;

	const int32_t ret = PtrMap1Release(map);
	map->cap			= map2->cap;
	map->len			= map2->len;
	map->pm				= map2->pm;
	map->pfirst			= map2->pfirst;
	map->plast			= map2->plast;
	map->info			= map2->info;
	PtrMap1Init(map2);
	return GRET_SUCCEED;
}

int32_t PtrMap1Swap(ptrmap1* const map,  ptrmap1* const map2)
{
	if (NULL == map || NULL == map2)
		return GRET_NULL;

	ptrmap0node** const tpm = map->pm;
	map->pm = map2->pm;
	map2->pm = tpm;
	ptrmap0node* const tpfirst = map->pfirst;
	map->pfirst = map2->pfirst;
	map2->pfirst = tpfirst;
	ptrmap0node* const  tplast = map->plast;
	map->plast = map2->plast;
	map2->plast = tplast;
	size_t const tcap = map->cap;
	map->cap = map2->cap;
	map2->cap = tcap;
	size_t const tlen = map->len;
	map->len = map2->len;
	map2->len = tlen;
	ptrmap1info infoTmp = map->info;
	map->info = map2->info;
	map2->info = infoTmp;
	return GRET_SUCCEED;
}

int32_t PtrMap1AddMap(ptrmap1* const map, const ptrmap1* const map2)
{
	if (PTRMAP1_ISNULLP(map) || PTRMAP1_ISNULLP(map2))
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;

	const ptrmap0node* node = PtrMap1BeginC(map2);
	while (NULL != node)
	{
		if (_PtrMap1HasKey(map, node->key))
			return GRET_DUPKEY;
		node = PtrMap1Next(node);
	}
	node = PtrMap1BeginC(map2);
	while (NULL != node)
	{
		int32_t ret = PtrMap1Add(map, node->key, node->val);
		node = PtrMap1Next(node);
	}
	return GRET_SUCCEED;
}

int32_t PtrMap1Info(const ptrmap1* const map, char** str)
{
	if (NULL == str)
		return GRET_NULLARG;
	if (NULL == map)
	{
		strcpy_s(*str, 128, "NULL OBJ");
		return GRET_SUCCEED;
	}
	else if (PTRMAP1_ISNULL(map))
	{
		strcpy_s(*str, 128, "NULL MAP");
		return GRET_SUCCEED;
	}
	char* buf0 = (char*)malloc(128 * sizeof(char));
	if (NULL == buf0)
		return GRET_MALLOC;
	char* buf = buf0;

	size_t nRemain = 128;
	int ret = sprintf_s(buf, nRemain, "len:%llu,cap:%llu.", map->len, map->cap);
	buf += ret;
	nRemain -= ret;
	const ptrmap0node* p = PtrMap1BeginC(map);
	for (size_t i = 0; i < map->len; ++i)
	{
		char* buf0k = (char*)"null";
		char* buf0v = (char*)"null";
		size_t sz0k = 0;
		if (NULL != map->info.fDispKey)
		{
			sz0k = map->info.fDispKey(p->key, &buf0k);
		}
		strcat_s(buf, nRemain, "\nKey:");
		buf += 4;
		if (4 > nRemain)
			break;
		nRemain -= 4;
		strcat_s(buf, nRemain, buf0k);
		buf += sz0k;
		if (sz0k > nRemain)
			break;
		nRemain -= sz0k;
		size_t sz0v = 0;
		if (NULL != map->info.fDispVal)
		{
			sz0v = map->info.fDispVal(p->val, &buf0v);
		}
		strcat_s(buf, nRemain, ",Val:");
		buf += 5;
		if (5 > nRemain)
			break;
		nRemain -= 5;
		strcat_s(buf, nRemain, buf0v);
		buf += sz0v;
		if (sz0v > nRemain)
			break;
		nRemain -= sz0v;
		p = PtrMap1Next(p);
		if (NULL == p)
			break;
	}
	*str = buf0;
	return GRET_SUCCEED;
}