#pragma once

#include <stdint.h>
#include <string.h>


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
typedef int32_t (*funLS0ElemDestructor)(void*);
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
	size_t						size;

	//funLS0ElemEquals			fEqualsKey;\

	funLS0ElemConstructorDef	fConstructorDef;
	funLS0ElemDestructor		fDestructor;
	funLS0ElemCopyConstructor	fConstructorCopy;
	funLS0ElemAssignCopy		fAssignCopy;
	funLS0ElemMoveConstructor	fConstructorMove;
	funLS0ElemAssignMove		fAssignMove;
} list0info;

void InitList0Info(list0info* const pinfo);

// CTNS_LOCAL is not implemented
typedef struct list1
{
	CTNSTATUS	status;
	void*		ptr;
	size_t		len;
	size_t		cap;

	list0info	info;
} list1;

typedef struct ptrlist1
{
	CTNSTATUS	status;
	void**		ptr;
	size_t		len;
	size_t		cap;

	list0info	info;
} plist1;


#pragma region list0

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
#define LIST0SIZEPTR					(LIST0HEADUNIT)//(max(sizeof(list0size), LIST0HEADUNIT))
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


// info
int32_t			List0Len(clist0c ls, size_t* pLen);
int32_t			List0Cap(clist0c ls, size_t* pCap);
int32_t			List0Status(clist0c ls, CTNSTATUS* pStatus);
int32_t			List0SetStatus(clist0c ls, CTNSTATUS status);

// lifecycle
list0 			_List0Make(size_t size);
list0 			_List0MakeBy(size_t cap, size_t size);
int32_t			List0Release(list0c ls);

// operation
int32_t			_List0Add(list0* pls, void* pElem, size_t size);
int32_t			_List0AddFront(list0* pls, void* pElem, size_t size);
int32_t			_List0RemoveAt(list0* pls, size_t idx, size_t size);
int32_t			_List0RemoveFront(list0* pls, size_t size);
int32_t			_List0RemoveBack(list0* pls, size_t size);
int32_t			_List0RemoveIn(list0* pls, size_t idx, size_t len, size_t size);
int32_t			_List0RemoveFrom(list0* pls, size_t idx, size_t size);
int32_t			_List0PopAt(list0* pls, size_t idx, void* pElem, size_t size);
int32_t			_List0PopFront(list0* pls, void* pElem, size_t size);
int32_t			_List0PopBack(list0* pls, void* pElem, size_t size);
//int32_t		_List0PopIn(list0* pls, size_t idx, size_t len, size_t size);
//int32_t		_List0PopFrom(list0* pls, size_t idx, size_t size);
int32_t			_List0Insert(list0* pls, void* pElem, size_t idx, size_t size);
int32_t			_List0GetAt(list0c ls, size_t idx, void* pElem, size_t size);
int32_t			_List0GetFront(list0c ls, void* pElem, size_t size);
int32_t			_List0GetBack(list0c ls, void* pElem, size_t size);
int32_t			_List0SetAt(list0c ls, void* pElem, size_t idx, size_t size);
int32_t			_List0SetFront(list0c ls, void* pElem, size_t idx, size_t size);
int32_t			_List0SetBack(list0c ls, void* pElem, size_t idx, size_t size);
int32_t			_List0FindFirst(list0c ls, void* pElem, size_t* pIdx, funLS0ElemEquals fe, size_t size);
int32_t			_List0FindLast(list0c ls, void* pElem, size_t* pIdx, funLS0ElemEquals fe, size_t size);
int32_t			List0Clear(clist0c ls);

//
int32_t			_List0Where(clist0c ls, list0* plsr, funLS0ElemSatisfy fs);

// capacity
int32_t			_List0ExtendBy(list0* pls, size_t dCap, size_t size);
int32_t			__List0TryExtendBy(list0* pls, size_t len, size_t size);
int32_t			__List0ReCap(list0* const pls, size_t cap, size_t size);
int32_t			_List0ReCap(list0* const pls, size_t cap, size_t size);
int32_t			_List0ReLen(list0* const pls, size_t len, size_t size);

// list
int32_t			_List0Copy(clist0c ls, list0* const plsr, size_t size);
int32_t			_List0DeepCopy(clist0c ls, list0* const plsr, size_t size, funLS0ElemAssignCopy fd);
int32_t			_List0Cat(list0 ls, clist0c ls2, size_t size);
int32_t			_List0InsertLs(list0 ls, clist0c ls2, size_t idx, size_t size);
int32_t			List0Attach(list0 ls, list0c ls2);
int32_t			List0Detach(list0 ls);

#pragma endregion list0


#pragma region list1

// list1
#define LIST1_ISNULL(pls)		(CTN0ISNULL((pls)->status))
#define LIST1_ISOWN(pls)		(CTN0ISOWN((pls)->status))
#define LIST1_ISREF(pls)		(CTN0ISREF((pls)->status))
#define LIST1_ISLOCAL(pls)		(CTN0ISLOCAL((pls)->status))
#define LIST1_ISNULLP(pls)		(NULL == (pls) || LIST1_ISNULL(pls))
#define LIST1POS(pls, idx)		((void*)((char*)(pls)->ptr + (pls)->info.size * idx))


inline void		_ListMemMoveTo(list1* const pls, size_t from, size_t to, size_t len)
{
	memmove(LIST1POS(pls, to), LIST1POS(pls, from), len * pls->info.size);
}
inline void		_List1AddCopy(list1* const pls, size_t idx, void* pElem)
{
	if (NULL != pls->info.fConstructorCopy)
		pls->info.fConstructorCopy(pElem, LIST1POS(pls, idx));
	else
		memmove(LIST1POS(pls, pls->len), pElem, pls->info.size);
}
inline void		_List1AddMove(list1* const pls, size_t idx, void* pElem)
{
	if (NULL != pls->info.fConstructorMove)
		pls->info.fConstructorMove(pElem, LIST1POS(pls, idx));
	else
	{
		memmove(LIST1POS(pls, pls->len), pElem, pls->info.size);
		memset(pElem, 0, pls->info.size);
	}
}

void			List1Init(list1* const pls);
int32_t			List1Make(list1* const pls, size_t size, list0info* const pinfo = NULL);
int32_t			List1MakeBy(list1* const pls, size_t cap, size_t size, list0info* const pinfo = NULL);
int32_t			List1Clear(list1* const pls);
int32_t			List1Abandon(list1* const pls);
int32_t			List1Release(list1* const pls);
int32_t			List1ReleaseAbandon(list1* const pls);

// operation
int32_t			List1Add(list1* const pls, void* pElem);
int32_t			List1AddMove(list1* const pls, void* pElem);
int32_t			List1AddFront(list1* const pls, void* pElem);
int32_t			List1AddFrontMove(list1* const pls, void* pElem);
int32_t			List1RemoveAt(list1* const pls, size_t idx);
int32_t			List1RemoveFront(list1* const pls);
int32_t			List1RemoveBack(list1* const pls);
int32_t			List1RemoveIn(list1* const pls, size_t idx, size_t len);
int32_t			List1RemoveFrom(list1* const pls, size_t idx);
int32_t			List1PopAt(list1* const pls, size_t idx, void* pElem);
int32_t			List1PopFront(list1* const pls, void* pElem);
int32_t			List1PopBack(list1* const pls, void* pElem);
//int32_t		List1PopIn(list1* const pls, size_t idx, size_t len);
//int32_t		List1PopFrom(list1* const pls, size_t idx);
int32_t			List1Insert(list1* const pls, void* pElem, size_t idx);
int32_t			List1InsertMove(list1* const pls, void* pElem, size_t idx);
int32_t			List1GetAt(list1* const pls, size_t idx, void* pElem);
int32_t			List1GetFront(list1* const pls, void* pElem);
int32_t			List1GetBack(list1* const pls, void* pElem);
int32_t			List1SetAt(list1* const pls, void* pElem, size_t idx);
int32_t			List1SetAtMove(list1* const pls, void* pElem, size_t idx);
int32_t			List1SetFront(list1* const pls, void* pElem, size_t idx);
int32_t			List1SetFrontMove(list1* const pls, void* pElem, size_t idx);
int32_t			List1SetBack(list1* const pls, void* pElem, size_t idx);
int32_t			List1SetBackMove(list1* const pls, void* pElem, size_t idx);
int32_t			List1FindFirst(list1* const pls, void* pElem, size_t* pIdx, funLS0ElemEquals fe);
int32_t			List1FindLast(list1* const pls, void* pElem, size_t* pIdx, funLS0ElemEquals fe);

//
//int32_t		List1Where(list1* const pls, list1* plsr, funLS0ElemSatisfy fs);
//int32_t		List1Where(list1* const pls, ptrlist0* plsr, funLS0ElemSatisfy fs);

// capacity
int32_t			List1ExtendBy(list1* const pls, size_t dCap);
int32_t			_List1TryDLenExtend(list1* const pls, size_t dLen);
int32_t			_List1ReCap(list1* const pls, size_t cap);
int32_t			List1ReCap(list1* const pls, size_t cap);
int32_t			List1ReLen(list1* const pls, size_t len);

// list
int32_t			List1Copy(list1* const pls, list0* const plsr);
int32_t			List1DeepCopy(list1* const pls, list0* const plsr, funLS0ElemAssignCopy fd);
int32_t			List1Move(list1* const pls, list0* const plsr, funLS0ElemAssignMove fd);
int32_t			List1Swap(list1* const pls, list0* const plsr, funLS0ElemSwap fd);
int32_t			List1Cat(list1* const pls, list1* const pls2);
int32_t			List1CatMove(list1* const pls, list1* const pls2);
int32_t			List1InsertLs(list1* const pls, list1* const pls2, size_t idx);
int32_t			List1InsertLsMove(list1* const pls, list1* const pls2, size_t idx);
//int32_t		List1Attach(list1* const pls, list1* const pls2);
//int32_t		List1Detach(list1* const pls);

#pragma endregion list1


#pragma region ptrlist0

#define PLIST0ADDR(ls, idx)		((decltype(ls))(_LIST0ELEMADDR(ls)) + (idx)*sizeof(ptrlist0elem0))


ptrlist0 PtrList0Make();
ptrlist0 PtrList0MakeBy(size_t cap);

#pragma endregion ptrlist0


#pragma region ptrlist1

int32_t			PtrList1Make(ptrlist1* const ls);
int32_t			PtrList1MakeBy(ptrlist1* const ls, size_t cap, size_t len = 0);
int32_t			PtrList1MakeByPure(ptrlist1* const ls, size_t cap, size_t len = 0);
int32_t			PtrList1Release(ptrlist1* const ls);

int32_t			PtrListAdd(ptrlist1* const ls, void* const p);
int32_t			PtrListInsert(ptrlist1* const ls, void* const p, size_t idx);
int32_t			PtrListPrepend(ptrlist1* const ls, void* const p);
int32_t			PtrListRemoveAt(ptrlist1* const ls, size_t idx);

#pragma endregion ptrlist1

