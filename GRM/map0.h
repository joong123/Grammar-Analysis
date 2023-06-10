#pragma once

#include <stdint.h>
#include "list0.h"


#pragma region ptrmap0

typedef uint64_t	map0id_t;
typedef size_t		map0idx_t;

typedef struct ptrmap0node
{
	map0id_t		id;
	void*			key;
	void*			val;
	ptrmap0node*	next;
	ptrmap0node*	prev;
	ptrmap0node*	nextInBuk;
} ptrmap0node;

typedef ptrmap0node** ptrmap0;

#pragma endregion ptrmap0


#pragma region ptrmap1

typedef map0id_t(*funMap0ID)(void*);
typedef bool(*funMap0EqualsKey)(void*, void*);
typedef bool(*funMap0EqualsVal)(void*, void*);
typedef int32_t(*funMap0KeyCtorDef)(void**);
typedef int32_t(*funMap0ValCtorDef)(void**);
typedef int32_t(*funMap0KeyCopyCtorDef)(void*, void**);
typedef int32_t(*funMap0ValCopyCtorDef)(void*, void**);
typedef int32_t(*funMap0KeyMoveCtorDef)(void*, void**);
typedef int32_t(*funMap0ValMoveCtorDef)(void*, void**);
typedef int32_t(*funMap0CopyKey)(void*, void*);
typedef int32_t(*funMap0CopyVal)(void*, void*);
typedef int32_t(*funMap0MoveKey)(void*, void*);
typedef int32_t(*funMap0MoveVal)(void*, void*);
typedef int32_t(*funMap0ReleaseKey )(void*);
typedef int32_t(*funMap0ReleaseVal)(void*);

typedef struct ptrmap1
{
	size_t			cap;
	size_t			len;
	ptrmap0node**	pm;
	ptrmap0node*	pfirst;
	ptrmap0node*	plast;

	funMap0ID			fID;
	funMap0EqualsKey	fEqualsKey;
	//funMap0EqualsVal	fEqualsVal;

	funMap0KeyCopyCtorDef	fCopyCtorKey;
	funMap0ValCopyCtorDef	fCopyCtorVal;
	funMap0CopyKey	fCopyKey;
	funMap0CopyVal	fCopyVal;
	funMap0ReleaseKey	fReleaseKey;
	funMap0ReleaseVal	fReleaseVal;
} ptrmap1;

inline map0id_t PtrMapIDDef(void* key)
{
	return (map0id_t)key;
}

#define PTRMAP1_ISNULLP(map)		((NULL == (map)) || (NULL == (map)->pm))
#define PTRMAP1_ISNULL(map)			(NULL == (map)->pm)
#define PTRMAP1_ID0(map, key)		((map)->fID ? (map)->fID(key) : PtrMapIDDef(key))
#define PTRMAP1_ID1(id0, cap)		((id0) % (cap))
#define PTRMAP1_ID1MAP(map, id0)	PTRMAP1_ID1(id0, (map)->cap)
#define PTRMAP1_ID(map, key)		PTRMAP1_ID1MAP(map, PTRMAP1_ID0((map), (key)))
#define PTRMAP1_CAP0				(16)
#define PTRMAP1_CAPEXTRATEDEF		(2.0)
#define PTRMAP1_CAPEXTNDEF			(8)
#define PTRMAP1_RATESTD				(0.75)


void		PtrMap1Init(ptrmap1* const map);
int32_t		PtrMap1Make(ptrmap1* const map);
int32_t		PtrMap1MakeBy(ptrmap1* const map, size_t cap);
int32_t		PtrMap1MakeByPure(ptrmap1* const map, size_t cap);
int32_t		_PtrMap1MakeNode(ptrmap1* const map, void* const key, void* const val, ptrmap0node** pnode);
int32_t		_PtrMap1ReleaseNode(ptrmap1* const map, ptrmap0node* const pnode);
void		_PtrMap1AttachNode(ptrmap1* const map, ptrmap0node* const pnode);
void		_PtrMap1DetachNode(ptrmap1* const map, ptrmap0node* const pnode);
int32_t		_PtrMap1AddNode(ptrmap1* const map, void* const key, void* const val);
void		_PtrMap1RemoveNode(ptrmap1* const map, ptrmap0node* const pnode);

ptrmap0node* PtrMap1Begin(ptrmap1* const map);
const ptrmap0node* PtrMap1BeginC(const ptrmap1* const map);
ptrmap0node* PtrMap1Next(const ptrmap0node* node);

int32_t		PtrMap1Insert(ptrmap1* const map, void* const key, void* const val);
int32_t		PtrMap1GetByKey(ptrmap1* const map, void* const key, void** pval);
int32_t		PtrMap1GetByVal(ptrmap1* const map, void* const val, void** pkey);
int32_t		PtrMap1RemoveKey(ptrmap1* const map, void* const key);
int32_t		PtrMap1RemoveVal(ptrmap1* const map, void* const val);

int32_t		PtrMap1Empty(ptrmap1* const map, bool* const bempty);
int32_t		PtrMap1HasKey(ptrmap1* const map, void* const key, bool* const bhas);
bool		_PtrMap1HasKey(ptrmap1* const map, void* const key);
//int32_t	PtrMap1HasVal(ptrmap1* const map, void* const val, bool* const bhas);
int32_t		PtrMap1Len(ptrmap1* const map, size_t* const len);
int32_t		PtrMap1Cap(ptrmap1* const map, size_t* const cap);
int32_t		PtrMap1ReCap(ptrmap1* const map, const size_t cap);

int32_t		PtrMap1Clear(ptrmap1* const map);
int32_t		PtrMap1Abandon(ptrmap1* const map);
int32_t		PtrMap1Release(ptrmap1* const map);
int32_t		PtrMap1ReleaseAbandon(ptrmap1* const map);
int32_t		PtrMap1Fit(ptrmap1* const map);

int32_t		PtrMap1Copy(ptrmap1* const map, const ptrmap1* const map2);
int32_t		PtrMap1Move(ptrmap1* const map, ptrmap1* const map2);
int32_t		PtrMap1Swap(ptrmap1* const map, ptrmap1* const map2);
int32_t		PtrMap1InsertMap(ptrmap1* const map, const ptrmap1* const map2);
int32_t		PtrMap1AllKey(ptrmap1* const map, ptrlist1* const ls);
int32_t		PtrMap1AllValue(ptrmap1* const map, ptrlist1* const ls);

int32_t		PtrMap1Test(const ptrmap1* const map, char* str);

#pragma endregion ptrmap1