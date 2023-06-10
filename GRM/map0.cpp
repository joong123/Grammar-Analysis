#include <stdlib.h>

#include "map0.h"
#include "def0.h"


void PtrMap1Init(ptrmap1* const map)
{
	if (NULL != map)
	{
		map->cap		= 0;
		map->len		= 0;
		map->pm			= NULL;
		map->pfirst		= NULL;
		map->plast		= NULL;
		map->fID		= NULL;
		map->fEqualsKey		= NULL;
		map->fCopyCtorKey	= NULL;
		map->fCopyCtorVal	= NULL;
		map->fReleaseKey	= NULL;
		map->fReleaseVal	= NULL;
	}
}

int32_t PtrMap1Make(ptrmap1* const map)
{
	if (NULL == map)
		return GRET_NULL;

	const int32_t ret = PtrMap1Release(map);

	return PtrMap1MakeByPure(map, PTRMAP1_CAP0);
}

int32_t PtrMap1MakeBy(ptrmap1* const map, size_t cap)
{
	if (NULL == map)
		return GRET_NULL;

	const int32_t ret = PtrMap1Release(map);

	return PtrMap1MakeByPure(map, cap);
}

int32_t PtrMap1MakeByPure(ptrmap1* const map, size_t cap)
{
	if (NULL == map)
		return GRET_NULL;

	ptrmap0node** const pm = (ptrmap0node**)malloc(cap * sizeof(ptrmap0node*));
	if (NULL == pm)
		return GRET_MALLOC;
	cap = max(cap, 1);
	PtrMap1Init(map);
	map->cap = cap;

	return GRET_SUCCEED;
}

int32_t _PtrMap1MakeNode(ptrmap1* const map, void* const key, void* const val, ptrmap0node** pnode)
{
	ptrmap0node* const pn = (ptrmap0node*)malloc(sizeof(ptrmap0node));
	if (NULL == pn)
		return GRET_MALLOC;

	pn->key = key;
	pn->val = val;
	const map0id_t id0 = PTRMAP1_ID0(map, key);
	pn->id = id0;
	pn->next = NULL;
	pn->prev = NULL;
	pn->nextInBuk = NULL;
	*pnode = pn;

	return GRET_SUCCEED;
}

int32_t _PtrMap1ReleaseNode(ptrmap1* const map, ptrmap0node* const pnode)
{
	if (NULL == pnode)
		return GRET_NONE;

	if(NULL != map->fReleaseKey)
		map->fReleaseKey(pnode->key);
	if (NULL != map->fReleaseVal)
		map->fReleaseVal(pnode->val);
	free(pnode);
	return GRET_SUCCEED;
}

void _PtrMap1AttachNode(ptrmap1* const map, ptrmap0node* const pn)
{
	if (NULL == pn)
		return;

	const map0id_t id = PTRMAP1_ID(map, pn->key);
	ptrmap0node* pnBuk = map->pm[id];
	if (NULL == pnBuk)
	{
		map->pm[id] = pn;
		if (NULL != map->plast)
		{
			map->plast->next = pn;
			pn->prev = map->plast;
			map->plast = pn;
		}
	}
	else
	{
		while (NULL != pnBuk->nextInBuk)
		{
			pnBuk = pnBuk->nextInBuk;
		}
		pnBuk->nextInBuk = pn;
		pnBuk->next = pn;
		pn->prev = pnBuk;
		// TODO: needed? do not need
		//pn->next = pnBuk->next;
		//pnBuk->next = NULL;

		if (pnBuk == map->plast)
		{
			//map->plast->next = pn;
			//pn->prev = map->plast;
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

void _PtrMap1DetachNode(ptrmap1* const map, ptrmap0node* const pn)
{
	if (NULL == pn)
		return;

	ptrmap0node* const pprev = pn->prev;
	ptrmap0node* const pnextInBuk = pn->nextInBuk;
	ptrmap0node* const pnext = pn->next;
	const bool bHeadOfBuk = (NULL == pprev) || (NULL != pprev && pn == pprev->nextInBuk);
	if (NULL != pprev)
	{
		if (pn == pprev->nextInBuk)
		{
			pprev->nextInBuk = pnextInBuk;
			pprev->next = pnext;
		}
		else if (pn == pprev->next)
		{
			pprev->next = (NULL != pnextInBuk) ? pnextInBuk : pnext;
		}
		else
		{
			//return GRET_INVMETA;
		}
	}
	if (NULL != pnext)
		pnext->prev = pprev;
	if (NULL != pnextInBuk)
		pnextInBuk->prev = pprev;

	if (map->pfirst == pn)
		map->pfirst = pnext;
	if (bHeadOfBuk)
	{
		const map0id_t id = PTRMAP1_ID1(pn->id, map->cap);
		map->pm[id] = pnextInBuk;
	}
	map->len--;

	pn->next = NULL;
	pn->prev = NULL;
	pn->nextInBuk = NULL;
	//return GRET_SUCCEED;
}

void _PtrMap1RemoveNode(ptrmap1* const map, ptrmap0node* const pn)
{
	if (NULL == pn)
		return;

	_PtrMap1DetachNode(map, pn);

	_PtrMap1ReleaseNode(map, pn);
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

int32_t PtrMap1Insert(ptrmap1* const map, void* const key, void* const val)
{
	if (NULL == map)
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;
	if (NULL == key || NULL == val)
		return GRET_NULLARG;

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

	const map0id_t id = PTRMAP1_ID(map, key);

	const ptrmap0node* pn = map->pm[id];
	while (NULL != pn)
	{
		if (pn->key == key) //map->fEqualsKey(pn->key, key)
		{
			*pval = pn->val;
			return GRET_SUCCEED;
		}
		pn = pn->nextInBuk;
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
			pInBuk = pInBuk->nextInBuk;
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
			_PtrMap1RemoveNode(map, pn);
			return GRET_SUCCEED;
		}
		pn = pn->nextInBuk;
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
				_PtrMap1RemoveNode(map, pInBuk);
				return GRET_SUCCEED;
			}
			pInBuk = pInBuk->nextInBuk;
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
	const map0id_t id = PTRMAP1_ID(map, key);

	const ptrmap0node* pn = map->pm[id];
	while (NULL != pn)
	{
		if (pn->key == key) //map->fEqualsKey(pn->key, key)
		{
			return true;
		}
		pn = pn->nextInBuk;
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
			pInBuk = pInBuk->nextInBuk;
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

	ptrmap0node** const pmNew = (ptrmap0node**)realloc(map->pm, capNew * sizeof(ptrmap0node*));
	if (NULL == pmNew)
		return GRET_MALLOC;

	ptrmap0node* const pnOld = map->pfirst;
	if (NULL == pnOld)
		return GRET_SUCCEED;

	for (ptrmap0node* pn = pnOld; pn != NULL;)
	{
		const map0id_t idOld = PTRMAP1_ID1(pn->id, map->cap);
		const map0id_t idNew = PTRMAP1_ID1(pn->id, capNew);
		map->pm[idOld] = NULL;
		map->pm[idNew] = pn;
		pn = pn->next;
	}

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

	ptrmap0node* pn = map->pfirst;
	while (NULL != pn)
	{
		if (map->fReleaseKey)
			map->fReleaseKey(pn->key);
		if (map->fReleaseVal)
			map->fReleaseVal(pn->val);

		ptrmap0node* pn2 = pn->next;
		const map0id_t id = PTRMAP1_ID1(pn->id, map->cap);
		map->pm[id] = NULL;

		_PtrMap1ReleaseNode(map, pn);
		pn = pn2;
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

	ptrmap0node* pn = map->pfirst;
	while (NULL != pn)
	{
		ptrmap0node* pn2 = pn->next;
		const map0id_t id = PTRMAP1_ID1(pn->id, map->cap);
		map->pm[id] = NULL;

		//_PtrMap1ReleaseNode(map, pn);
		free(pn);
		pn = pn2;
	}

	map->pfirst = NULL;
	map->plast = NULL;

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
	map->fID			= NULL;
	map->fEqualsKey		= NULL;
	map->fCopyCtorKey	= NULL;
	map->fCopyCtorVal	= NULL;
	map->fCopyKey		= NULL;
	map->fCopyVal		= NULL;
	map->fReleaseKey	= NULL;
	map->fReleaseVal	= NULL;

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
	map->fID			= map2->fID;
	map->fEqualsKey		= map2->fEqualsKey;
	map->fCopyCtorKey	= map2->fCopyCtorKey;
	map->fCopyCtorVal	= map2->fCopyCtorVal;
	map->fCopyKey		= map2->fCopyKey;
	map->fCopyVal		= map2->fCopyVal;
	map->fReleaseKey	= map2->fReleaseKey;
	map->fReleaseVal	= map2->fReleaseVal;
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
	funMap0ID const tfID = map->fID;
	map->fID = map2->fID;
	map2->fID = tfID;
	funMap0EqualsKey const tfEqualsKey = map->fEqualsKey;
	map->fEqualsKey = map2->fEqualsKey;
	map2->fEqualsKey = tfEqualsKey;
	funMap0KeyCopyCtorDef const tfCopyCtorKey = map->fCopyCtorKey;
	map->fCopyCtorKey = map2->fCopyCtorKey;
	map2->fCopyCtorKey = tfCopyCtorKey;
	funMap0ValCopyCtorDef const tfCopyCtorVal = map->fCopyCtorVal;
	map->fCopyCtorVal = map2->fCopyCtorVal;
	map2->fCopyCtorVal = tfCopyCtorVal;
	funMap0CopyKey const tfCopyKey = map->fCopyKey;
	map->fCopyKey = map2->fCopyKey;
	map2->fCopyKey = tfCopyKey;
	funMap0CopyVal const tfCopyVal = map->fCopyVal;
	map->fCopyVal = map2->fCopyVal;
	map2->fCopyVal = tfCopyVal;
	funMap0ReleaseKey const tfReleaseKey = map->fReleaseKey;
	map->fReleaseKey = map2->fReleaseKey;
	map2->fReleaseKey = tfReleaseKey;
	funMap0ReleaseVal const tfReleaseVal = map->fReleaseVal;
	map->fReleaseVal = map2->fReleaseVal;
	map2->fReleaseVal = tfReleaseVal;

	return GRET_SUCCEED;
}

int32_t PtrMap1InsertMap(ptrmap1* const map, const ptrmap1* const map2)
{
	if (PTRMAP1_ISNULLP(map) || PTRMAP1_ISNULLP(map2))
		return GRET_NULL;
	if (PTRMAP1_ISNULL(map))
		return GRET_INVSTATUS;

	const ptrmap0node* node = PtrMap1BeginC(map2);
	while (NULL != node)
	{
		if (_PtrMap1HasKey(map, node->key))
		{
			return GRET_DUPKEY;
		}
		node = PtrMap1Next(node);
	}

	node = PtrMap1BeginC(map2);
	while (NULL != node)
	{
		int32_t ret = PtrMap1Insert(map, node->key, node->val);
		node = PtrMap1Next(node);
	}

	return GRET_SUCCEED;
}

int32_t PtrMap1Test(const ptrmap1* const map, char* str)
{
	if (NULL == str)
	{
		return GRET_NULLARG;
	}
	if (NULL == map)
	{
		//strcpy(str, "NULL");
		return GRET_NULL;
	}

	return GRET_SUCCEED;
}