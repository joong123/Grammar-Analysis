#pragma once

#include "def0.h"
#include <stdint.h>
#include <string.h>

// TODO:
// cap * size overflow
// EXTENDBY, dadd first
// list0c* is what?


typedef size_t				list0size;
typedef int32_t				list0status;


enum CTNSTATUS
{
	CTNS_NULL	= 0,
	CTNS_OWN	= 1,
	CTNS_REF	= 2,
	CTNS_LOCAL	= 3
};

#define CTN0ISNULL(status)			(CTNSTATUS::CTNS_NULL == (status))
#define CTN0ISOWN(status)			(CTNSTATUS::CTNS_OWN == (status))
#define CTN0ISREF(status)			(CTNSTATUS::CTNS_REF == (status))
#define CTN0ISLOCAL(status)			(CTNSTATUS::CTNS_LOCAL == (status))

#define LISTPOS(ptr, idx, size)		((void*)((int8_t*)(ptr) + (idx)*(size)))

#define VALIDATE_PTR(pls)			if (ISNULLPTR(pls)) return GRET_NULL;
#define VALIDATE_SIZEDATA(size)		if ((size) <= 0) return GRET_INVARG;

int32_t ListExtendDLen(const size_t len, const size_t dlen, size_t* const cap);


#pragma region list0

//
// ***list0*** 
//
// contain POD, deepcopy/dtor/linq/equals is optional

typedef void*				list0;
typedef const void*			clist0;
typedef void* const			list0c;
typedef const void* const	clist0c;


typedef bool (*funLS0ElemEquals)(void*, void*);
typedef bool (*funLS0ElemSatisfy)(void*);
typedef int32_t(*funLS0ElemConstructorDef)(void*);
typedef int32_t(*funLS0ElemDestructor)(void*);
typedef int32_t(*funLS0ElemCopyConstructor)(void*, void*);
typedef int32_t(*funLS0ElemAssignCopy)(void*, void*);
typedef int32_t(*funLS0ElemMoveConstructor)(void*, void*);
typedef int32_t(*funLS0ElemAssignMove)(void*, void*);
typedef int32_t(*funLS0ElemSwap)(void*, void*);

typedef funLS0ElemEquals		funPLS0ElemEquals;
typedef funLS0ElemSatisfy		funPLS0ElemSatisfy;
typedef funLS0ElemDestructor	funPLS0ElemDestructor;
typedef funLS0ElemAssignCopy	funPLS0ElemAssignCopy;
typedef funLS0ElemAssignMove	funPLS0ElemAssignMove;
typedef int32_t(*funPLS0ElemConstructorDef)(void**);
typedef int32_t(*funPLS0ElemCopyConstructor)(void*, void**);
typedef int32_t(*funPLS0ElemMoveConstructor)(void*, void**);
typedef int32_t(*funPLS0ElemSwap)(void**, void**);


typedef struct list0info
{
	size_t						sizeData;
	size_t						sizeUnit;

	funLS0ElemEquals			fEquals;

	funLS0ElemConstructorDef	fConstructorDef;
	funLS0ElemDestructor		fDestructor;
	funLS0ElemCopyConstructor	fConstructorCopy;
	funLS0ElemAssignCopy		fAssignCopy;
	funLS0ElemMoveConstructor	fConstructorMove;
	funLS0ElemAssignMove		fAssignMove;
} list0info;

typedef struct ptrlist0info
{
	size_t						sizeData;
	size_t						sizeUnit;

	funPLS0ElemEquals			fEquals;

	funPLS0ElemConstructorDef	fConstructorDef;
	funPLS0ElemDestructor		fDestructor;
	funPLS0ElemCopyConstructor	fConstructorCopy;
	funPLS0ElemAssignCopy		fAssignCopy;
	funPLS0ElemMoveConstructor	fConstructorMove;
	funPLS0ElemAssignMove		fAssignMove;
} ptrlist0info;

void InitList0Info(list0info* const pinfo);


typedef void**				ptrlist0;
typedef void* const*		cptrlist0;
typedef void** const		ptrlist0c;
typedef void* const* const	cptrlist0c;

typedef void*	ptrlist0elem0;

typedef void*				itptrlist0;
typedef const void*			citptrlist0;

// Structure
#define LIST0HEADUNIT				(8)
#define LIST0SIZELEN				(LIST0HEADUNIT)//(max(sizeof(list0size), LIST0HEADUNIT))
#define LIST0SIZECAP				(LIST0HEADUNIT)//(max(sizeof(list0size), LIST0HEADUNIT))
#define LIST0SIZESTATUS				(LIST0HEADUNIT)//(max(sizeof(list0status), LIST0HEADUNIT))
#define LIST0SIZEPTR				(LIST0HEADUNIT)//(max(sizeof(list0size), LIST0HEADUNIT))
#define LIST0HEADSIZE				((LIST0SIZELEN) + (LIST0SIZECAP) + (LIST0SIZESTATUS) + (LIST0SIZEPTR))
#define LIST0OFFLEN					(0)
#define LIST0OFFCAP					((LIST0OFFLEN) + (LIST0SIZELEN))
#define LIST0OFFSATUS				((LIST0OFFCAP) + (LIST0SIZECAP))
#define LIST0OFFPTR					((LIST0OFFSATUS) + (LIST0SIZESTATUS))

// Info
#define LIST0LEN(ls)				(*((list0size*)((int8_t*)(ls) + (LIST0OFFLEN))))
#define LIST0CAP(ls)				(*((list0size*)((int8_t*)(ls) + (LIST0OFFCAP))))
#define LIST0STATUSVAL(ls)			(*((list0status*)((int8_t*)(ls) + (LIST0OFFSATUS))))
#define LIST0STATUS(ls)				((CTNSTATUS)(LIST0STATUSVAL(ls)))
#define LIST0PTR(ls)				(*((void**)((int8_t*)(ls) + (LIST0OFFPTR))))
#define _LIST0ELEMADDR(ls)			((void*)((int8_t*)(ls) + (LIST0HEADSIZE)))
#define LIST0_SETSTATUS(ls, status)	((LIST0STATUSVAL(ls)) = ((list0status)(status)))
#define LIST0_SETLENCAPSTATUS(ls, len, cap, status)	\
		{ (LIST0LEN(ls)) = (len); (LIST0CAP(ls)) = (cap); LIST0_SETSTATUS((ls), (status)); }

// Status
#define LIST0ISNULL(ls)				CTN0ISNULL(LIST0STATUS(ls))
#define LIST0ISOWN(ls)				CTN0ISOWN(LIST0STATUS(ls))
#define LIST0ISREF(ls)				CTN0ISREF(LIST0STATUS(ls))
#define LIST0ISLOCAL(ls)			CTN0ISLOCAL(LIST0STATUS(ls))
#define LIST0ISEMPTY(ls)			(0 == (LIST0LEN(ls)))
#define LIST0ISNULLP(ls)			((NULL == (ls)) || (LIST0ISNULL(ls)))
#define LIST0ISNOTNULL(ls)			(!LIST0ISNULLP(ls))
#define LIST0ISFULL(ls)				((LIST0LEN(ls)) == (LIST0CAP(ls)))

// Addr
#define _LIST0ADDR(ls, idx, size)	((void*)((int8_t*)(ls) + (LIST0HEADSIZE) + (idx)*(size)))
#define LIST0ELEMADDR(ls)			(((decltype(ls))(_LIST0ELEMADDR(ls))))
#define LIST0POS(ls, idx)			(LIST0ELEMADDR(ls) + (idx))
#define LIST0AT(ls, idx)			(*(LIST0POS(ls, (idx))))
#define LIST0BEGIN(ls)				(LIST0POS(ls, 0))
#define LIST0END(ls)				(LIST0POS(ls, (LIST0LEN(ls))))

// info
#define LIST0PINFO(ls)				((list0info*)(LIST0PTR(ls)))

// mem
#define LIST0MALLOC(cap, size)		((list0)malloc((cap) * (size) + LIST0HEADSIZE))
#define LIST0REALLOC(ls, cap, size)	((list0)realloc((ls), (cap) * (size) + LIST0HEADSIZE))

// Specification
#define LIST0INITCAP				(4)
#define LIST0EXTENDRATE				(1.5)

// Interface
#define LIST0MAKE(type)				((type*)_List0Make(sizeof(type)))
#define LIST0MAKEBY(type, cap)		((type*)_List0MakeBy((cap), sizeof(type)))
#define LIST0ADD(ls, val)			_List0Add((list0*)&(ls), &(val), sizeof(*(ls)))

typedef int32_t(*funFreeElem)(void*);

// info
int32_t			List0Len(clist0c ls, size_t* const pLen);
int32_t			List0Cap(clist0c ls, size_t* const pCap);
int32_t			List0Status(clist0c ls, CTNSTATUS* const pStatus);
int32_t			List0SetStatus(clist0c ls, const CTNSTATUS status);

// lifecycle
list0 			_List0Make(const size_t size);
list0 			_List0MakeBy(const size_t cap, const size_t size);
int32_t 		_List0MakePure(list0* const pls, const size_t size);
int32_t 		_List0MakeByPure(list0* const pls, const size_t cap, const size_t size);
int32_t			List0Release(list0c ls);

// operation
int32_t			_List0Add(list0* const pls, void* const pElem, const size_t size);
int32_t			_List0AddFront(list0* pls, void* const pElem, const size_t size);
int32_t			_List0RemoveAt(list0* pls, const size_t idx, const size_t size);
int32_t			_List0RemoveFront(list0* pls, const size_t size);
int32_t			_List0RemoveBack(list0* pls, const size_t size);
int32_t			_List0RemoveIn(list0* pls, const size_t idx, const size_t len, const size_t size);
int32_t			_List0RemoveFrom(list0* pls, const size_t idx, const size_t size);
int32_t			_List0PopAt(list0* pls, const size_t idx, void* const pElem, const size_t size);
int32_t			_List0PopFront(list0* pls, void* const pElem, const size_t size);
int32_t			_List0PopBack(list0* pls, void* const pElem, const size_t size);
//int32_t		_List0PopIn(list0* pls, const size_t idx, const size_t len, const size_t size);
//int32_t		_List0PopFrom(list0* pls, const size_t idx, const size_t size);
int32_t			_List0Insert(list0* pls, void* const pElem, const size_t idx, const size_t size);
int32_t			_List0GetAt(list0c ls, const size_t idx, void* const pElem, const size_t size);
int32_t			_List0GetFront(list0c ls, void* const pElem, const size_t size);
int32_t			_List0GetBack(list0c ls, void* const pElem, const size_t size);
int32_t			_List0SetAt(list0c ls, void* const pElem, const size_t idx, const size_t size);
int32_t			_List0SetFront(list0c ls, void* const pElem, const size_t idx, const size_t size);
int32_t			_List0SetBack(list0c ls, void* const pElem, const size_t idx, const size_t size);
int32_t			_List0FindFirst(list0c ls, void* const pElem, size_t* const pIdx, const funLS0ElemEquals fe, size_t size);
int32_t			_List0FindLast(list0c ls, void* const pElem, size_t* const pIdx, const funLS0ElemEquals fe, size_t size);
int32_t			List0Clear(clist0c ls);

//
int32_t			_List0Where(clist0c ls, list0* const plsr, const funLS0ElemSatisfy fs);

// capacity
int32_t			_List0ExtendBy(list0* const pls, const size_t dCap, const size_t size);
int32_t			__List0ExtendBy(list0* const pls, const size_t len, const size_t size);
int32_t			__List0ReCap(list0* const pls, const size_t cap, const size_t size);
int32_t			_List0ReCap(list0* const pls, const size_t cap, const size_t size);
int32_t			_List0ReLen(list0* const pls, const size_t len, const size_t size);

// list
int32_t			_List0Copy(clist0c ls, list0* const plsr, const size_t size);
int32_t			_List0DeepCopy(clist0c ls, list0* const plsr, const size_t size, const funLS0ElemAssignCopy fd);
int32_t			_List0Cat(list0 ls, clist0c ls2, const size_t size);
int32_t			_List0InsertLs(list0 ls, clist0c ls2, const  size_t idx, const size_t size);
int32_t			List0Attach(list0 ls, list0c ls2);
int32_t			List0Detach(list0 ls);

#pragma endregion list0


#pragma region list1

typedef struct list1
{
	void*		ptr;
	size_t		len;
	size_t		cap;
	size_t		szData;
} list1;

typedef struct ptrlist1
{
	void*		ptr;
	size_t		len;
	size_t		cap;
} ptrlist1;

int32_t		_List1ExtendDLen(list1* const pls, const size_t dLen);
int32_t		_List1ReCap(list1* const pls, const size_t cap);
int32_t		_List1FromPure(list1* const pls, const size_t size, void* const pls2, const size_t len);
void		List1Init0(list1* const pls);
int32_t		List1Make(list1* const pls, const size_t size);
int32_t		List1MakePure(list1* const pls, const size_t size);
int32_t		List1MakePureBy(list1* const pls, const size_t cap, const size_t size);
int32_t		List1From(list1* const pls, const size_t size, void* const pls2, const size_t len);
int32_t		List1FromPure(list1* const pls, const size_t size, void* const pls2, const size_t len);
int32_t		List1Add(list1* const pls, void* const pElem);
int32_t		List1Pop(list1* const pls, void* const pElem);
int32_t		List1PopFront(list1* const pls, void* const pElem);
int32_t		List1Remove(list1* const pls);
int32_t		List1RemoveFront(list1* const  pls);
int32_t		List1Clear(list1* const pls);// clear abandon
int32_t		List1ClearFree(list1* const pls, const funFreeElem funFree);
int32_t		List1Release(list1* const pls);// release abandon
int32_t		List1ReleaseFree(list1* const pls, const funFreeElem funFree);

#pragma endregion list1


#pragma region list2

// CTNS_LOCAL is not implemented
typedef struct list2
{
	CTNSTATUS	status;
	void*		ptr;
	size_t		len;
	size_t		cap;

	list0info	info;
} list2;

typedef struct ptrlist2
{
	CTNSTATUS	status;
	void** ptr;
	size_t		len;
	size_t		cap;

	ptrlist0info	info;
} plist2;


#define LIST2_ISNULL(pls)		(CTN0ISNULL((pls)->status))
#define LIST2_ISOWN(pls)		(CTN0ISOWN((pls)->status))
#define LIST2_ISREF(pls)		(CTN0ISREF((pls)->status))
#define LIST2_ISLOCAL(pls)		(CTN0ISLOCAL((pls)->status))
#define LIST2_ISNULLP(pls)		(NULL == (pls) || LIST2_ISNULL(pls))
#define LIST2POS(pls, idx)		((void*)((char*)(pls)->ptr + (pls)->info.sizeUnit * idx))
#define LIST2BEGIN(pls)			((void*)((pls)->ptr))

#define LIST2INIT(pls, type)		List2Init((pls), sizeof(type));
#define LIST2INITPURE(pls, type)	List2InitPure((pls), sizeof(type));
#define LIST2GETAT(pls, idx, item)	List2GetAt((pls), (idx), (void*)&(item));
#define LIST2ADD(pls, item)			List2Add((pls), (void*)&(item));
#define LIST2AT(pls, idx, type)		((type)*)List2At((pls), (idx));


inline void		_List2MoveTo(list2* const pls, const size_t from, const size_t to, const size_t len);
inline void		_List2MoveToBuf(list2* const pls, const size_t from, void* const pDst, const size_t to, const size_t len);
inline void		_List2MoveFromBuf(list2* const pls, const size_t to, void* const pSrc, const size_t from, const size_t len);
inline void		_List2CopyToList(list2* const pls, const size_t from, list2* const pls2, const size_t to, const size_t len);
inline void		_List2Make(list2* const pls, const size_t from, const size_t num);
inline void		_List2Free(list2* const pls, const size_t from, const size_t num);
inline void		_List2MakeAtCopy(list2* const pls, const size_t idx, void* const pElem);
inline void		_List2MakeAtMove(list2* const pls, const size_t idx, void* const pElem);
inline void		_List2SetAtCopy(list2* const pls, const size_t idx, void* const pElem);
inline void		_List2SetAtMove(list2* const pls, const size_t idx, void* const pElem);
inline void		_List2GetAtCopy(list2* const pls, const size_t idx, void* const pElem);
inline void		_List2GetAtMove(list2* const pls, const size_t idx, void* const pElem);

void			List2Init0(list2* const pls);
int32_t			List2Init(list2* const pls, const size_t size, const list0info* const pinfo = NULL);
int32_t			List2InitPure(list2* const pls, const size_t size, const list0info* const pinfo = NULL);
int32_t			List2InitBy(list2* const pls, const size_t cap, const size_t size, const list0info* const pinfo = NULL);
int32_t			List2InitByPure(list2* const ls, const size_t cap, const size_t size, const list0info* const pinfo = NULL);
int32_t			List2Clear(list2* const pls);
int32_t			List2Abandon(list2* const pls);
int32_t			List2Release(list2* const pls);
int32_t			List2ReleaseAbandon(list2* const pls);

void*			List2At(list2* const pls, const size_t idx);
void*			List2Begin(list2* const pls);
void*			List2End(list2* const pls);
void*			List2Next(list2* const pls, void* const pElem);
void*			List2SafeNext(list2* const pls, void* const pElem);
// operation
int32_t			List2Add(list2* const pls, void* const pElem);
int32_t			List2AddMove(list2* const pls, void* const pElem);
int32_t			List2AddFront(list2* const pls, void* const pElem);
int32_t			List2AddFrontMove(list2* const pls, void* const pElem);
int32_t			_List2RemoveIn(list2* const pls, const size_t idx, const size_t num);
int32_t			List2RemoveAt(list2* const pls, const size_t idx);
int32_t			List2RemoveFront(list2* const pls, const size_t num = 1);
int32_t			List2RemoveBack(list2* const pls, const size_t num = 1);
int32_t			List2RemoveIn(list2* const pls, const size_t idx, const size_t num);
int32_t			List2RemoveFrom(list2* const pls, const size_t idx);
int32_t			List2PopAt(list2* const pls, const size_t idx, void* const pElem);
int32_t			List2PopFront(list2* const pls, void* const pElem);
int32_t			List2PopBack(list2* const pls, void* const pElem);
int32_t			List2Insert(list2* const pls, void* const pElem, const size_t idx);
int32_t			List2InsertMove(list2* const pls, void* const pElem, const size_t idx);
int32_t			List2GetAt(list2* const pls, const size_t idx, void* const pElem);
int32_t			List2GetFront(list2* const pls, void* const pElem);
int32_t			List2GetBack(list2* const pls, void* const pElem);
int32_t			List2SetAt(list2* const pls, void* const pElem, const size_t idx);
int32_t			List2SetAtMove(list2* const pls, void* const pElem, const size_t idx);
int32_t			List2SetFront(list2* const pls, void* const pElem);
int32_t			List2SetFrontMove(list2* const pls, void* const pElem);
int32_t			List2SetBack(list2* const pls, void* const pElem);
int32_t			List2SetBackMove(list2* const pls, void* const pElem);
int32_t			List2FindFirst(list2* const pls, void* const pElem, size_t* const pIdx, const funLS0ElemEquals fe = NULL);
int32_t			List2FindLast(list2* const pls, void* const pElem, size_t* const pIdx, const funLS0ElemEquals fe = NULL);

// capacity
int32_t			_List2ExtendDLen(list2* const pls, const size_t dLen);
int32_t			_List2ReCap(list2* const pls, const size_t cap);
int32_t			List2ReCap(list2* const pls, const size_t cap);
int32_t			List2ReLen(list2* const pls, const size_t len);

// list
int32_t			List2InitType(list2* const pls, const list2* const plsf);
int32_t			List2InitTypeBy(list2* const pls, const list2* const plsf, const size_t cap);
int32_t			List2CopyTo(list2* const pls, list2* const plsr);
//int32_t			List2Copy(list2* const pls, list2* const plsr, const funLS0ElemAssignCopy fd);
int32_t			List2MoveTo(list2* const pls, list2* const plsr);
//int32_t			List2Move(list2* const pls, list2* const plsr, const funLS0ElemAssignMove fd);
int32_t			List2Swap(list2* const pls, list2* const plsr);
//int32_t			List2Swap(list2* const pls, list2* const plsr, const funLS0ElemSwap fd);
int32_t			List2CatList(list2* const pls, list2* const pls2);
int32_t			List2CatListMove(list2* const pls, list2* const pls2);
int32_t			List2InsertList(list2* const pls, list2* const pls2, const size_t idx);
int32_t			List2InsertListMove(list2* const pls, list2* const pls2, const size_t idx);
//int32_t		List2Attach(list2* const pls, list2* const pls2);
//int32_t		List2Detach(list2* const pls);

#pragma endregion list2



#pragma region ptrlist2

#define PTRLIST2POS(pls, idx)		((void**)((char*)(pls)->ptr + idx * sizeof(void*)))
#define PTRLIST2AT(pls, idx)		(*(PTRLIST2POS(pls, idx)))

inline void		_PtrList2MoveTo(ptrlist2* const pls, const size_t from, const size_t to, const size_t len);
inline void		_PtrList2CopyToList(ptrlist2* const pls, const size_t from, ptrlist2* const pls2, const size_t to, const size_t len);
inline void		_PtrList2DeepCopyToList(ptrlist2* const pls, const size_t from, ptrlist2* const pls2, const size_t to, const size_t len);
inline void		_PtrList2Make(ptrlist2* const pls, const size_t from, const size_t num);
inline void		_PtrList2Clear(ptrlist2* const pls, const size_t from, const size_t num);
int32_t			_PtrList2DeepCopy(ptrlist2* const pls, void* const pElem, void** const ppElemCopy);

void			PtrList2Init0(ptrlist2* const pls);
int32_t			PtrList2Init(ptrlist2* const pls, const ptrlist0info* const pinfo = NULL);
int32_t			PtrList2InitBy(ptrlist2* const pls, const size_t cap, const ptrlist0info* const pinfo = NULL);
int32_t			PtrList2InitByPure(ptrlist2* const pls, const size_t cap, const ptrlist0info* const pinfo = NULL);
int32_t			PtrList2Clear(ptrlist2* const pls);
int32_t			PtrList2Abandon(ptrlist2* const pls);
int32_t			PtrList2Release(ptrlist2* const pls);
int32_t			PtrList2ReleaseAbandon(ptrlist2* const pls);

int32_t			PtrList2Add(ptrlist2* const ls, void* const pElem);
int32_t			PtrList2AddDeepCopy(ptrlist2* const pls, void* const pElem);
int32_t			PtrList2AddFront(ptrlist2* const ls, void* const pElem);
int32_t			PtrList2AddFrontDeepCopy(ptrlist2* const pls, void* const pElem);
int32_t			_PtrList2RemoveIn(ptrlist2* const pls, const size_t idx, const size_t num);
int32_t			PtrList2RemoveAt(ptrlist2* const pls, const size_t idx);
int32_t			PtrList2RemoveFront(ptrlist2* const pls, const size_t num = 1);
int32_t			PtrList2RemoveBack(ptrlist2* const pls, const size_t num = 1);
int32_t			PtrList2RemoveIn(ptrlist2* const pls, const size_t idx, const size_t num);
int32_t			PtrList2RemoveFrom(ptrlist2* const pls, const size_t idx);
int32_t			PtrList2PopAt(ptrlist2* const pls, const size_t idx, void** const pElem);
int32_t			PtrList2PopFront(ptrlist2* const pls, void** const pElem);
int32_t			PtrList2PopBack(ptrlist2* const pls, void** const pElem);
int32_t			PtrList2Insert(ptrlist2* const ls, void* const pElem, const size_t idx);
int32_t			PtrList2InsertDeepCopy(ptrlist2* const ls, void* const pElem, const size_t idx);
int32_t			PtrList2GetAt(ptrlist2* const pls, const size_t idx, void** const pElem);
int32_t			PtrList2GetFront(ptrlist2* const pls, void** const pElem);
int32_t			PtrList2GetBack(ptrlist2* const pls, void** const pElem);
int32_t			PtrList2SetAt(ptrlist2* const pls, void* const pElem, const size_t idx);
int32_t			PtrList2SetAtDeepCopy(ptrlist2* const pls, void* const pElem, const size_t idx);
int32_t			PtrList2SetFront(ptrlist2* const pls, void* const pElem);
int32_t			PtrList2SetBack(ptrlist2* const pls, void* const pElem);
int32_t			PtrList2SetBackDeepCopy(ptrlist2* const pls, void* const pElem);
int32_t			PtrList2FindFirst(ptrlist2* const pls, void* const pElem, size_t* const pIdx, const funPLS0ElemEquals fe);
int32_t			PtrList2FindLast(ptrlist2* const pls, void* const pElem, size_t* const pIdx, const funPLS0ElemEquals fe);

int32_t			_PtrList2ExtendDLen(ptrlist2* const pls, const size_t dLen);
int32_t			_PtrList2ReCap(ptrlist2* const pls, const size_t cap);
int32_t			PtrList2ReCap(ptrlist2* const pls, const size_t cap);
int32_t			PtrList2ReLen(ptrlist2* const pls, const size_t len);

// list
int32_t			PtrList2CopyTo(ptrlist2* const pls, ptrlist2* const plsr);
int32_t			PtrList2DeepCopyTo(ptrlist2* const pls, ptrlist2* const plsr);
//int32_t			PtrList2Copy(ptrlist2* const pls, ptrlist2* const plsr, const funLS0ElemAssignCopy fd);
int32_t			PtrList2MoveTo(ptrlist2* const pls, ptrlist2* const plsr);
//int32_t			List2Move(ptrlist2* const pls, ptrlist2* const plsr, const funLS0ElemAssignMove fd);
int32_t			PtrList2Swap(ptrlist2* const pls, ptrlist2* const plsr);
//int32_t			PtrList2Swap(ptrlist2* const pls, ptrlist2* const plsr, const funLS0ElemSwap fd);
int32_t			PtrList2CatList(ptrlist2* const pls, ptrlist2* const pls2);
int32_t			PtrList2CatListDeepCopy(ptrlist2* const pls, ptrlist2* const pls2);
int32_t			PtrList2InsertList(ptrlist2* const pls, ptrlist2* const pls2, const size_t idx);
int32_t			PtrList2InsertListDeepCopy(ptrlist2* const pls, ptrlist2* const pls2, const size_t idx);
//int32_t		PtrList2Attach(ptrlist2* const pls, ptrlist2* const pls2);
//int32_t		PtrList2Detach(ptrlist2* const pls);

#pragma endregion ptrlist2

#include "stdio.h"

inline void TestList()
{
	printf("TestList Begin..\n");

	list2 ls0;
	int ret = LIST2INIT(&ls0, int);

	int item = 123;
	ret = LIST2ADD(&ls0, item);
	item = 454;
	ret = LIST2ADD(&ls0, item);
	ret = LIST2GETAT(&ls0, 0, item);

	printf("TestList End..\n");
}