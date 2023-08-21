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
	size_t						size;

	funPLS0ElemEquals			fEquals;

	funPLS0ElemConstructorDef	fConstructorDef;
	funPLS0ElemDestructor		fDestructor;
	funPLS0ElemCopyConstructor	fConstructorCopy;
	funPLS0ElemAssignCopy		fAssignCopy;
	funPLS0ElemMoveConstructor	fConstructorMove;
	funPLS0ElemAssignMove		fAssignMove;
} ptrlist0info;

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

	ptrlist0info	info;
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


inline void		_List1MoveTo(list1* const pls, size_t from, size_t to, size_t len);
inline void		_List1MoveToBuf(list1* const pls, size_t from, void* pDst, size_t to, size_t len);
inline void		_List1MoveFromBuf(list1* const pls, size_t to, void* pSrc, size_t from, size_t len);
inline void		_List1CopyToList(list1* const pls, size_t from, list1* const pls2, size_t to, size_t len);
inline void		_List1Make(list1* const pls, size_t from, size_t num);
inline void		_List1Clear(list1* const pls, size_t from, size_t num);
inline void		_List1MakeAtCopy(list1* const pls, size_t idx, void* pElem);
inline void		_List1MakeAtMove(list1* const pls, size_t idx, void* pElem);
inline void		_List1SetAtCopy(list1* const pls, size_t idx, void* pElem);
inline void		_List1SetAtMove(list1* const pls, size_t idx, void* pElem);
inline void		_List1GetAtCopy(list1* const pls, size_t idx, void* pElem);
inline void		_List1GetAtMove(list1* const pls, size_t idx, void* pElem);

void			List1Init(list1* const pls);
int32_t			List1Make(list1* const pls, size_t size, const list0info* const pinfo = NULL);
int32_t			List1MakePure(list1* const pls, size_t size, const list0info* const pinfo = NULL);
int32_t			List1MakeBy(list1* const pls, size_t cap, size_t size, const list0info* const pinfo = NULL);
int32_t			List1MakeByPure(list1* const ls, size_t cap, size_t size, const list0info* const pinfo = NULL);
int32_t			List1Clear(list1* const pls);
int32_t			List1Abandon(list1* const pls);
int32_t			List1Release(list1* const pls);
int32_t			List1ReleaseAbandon(list1* const pls);

// operation
int32_t			List1Add(list1* const pls, void* pElem);
int32_t			List1AddMove(list1* const pls, void* pElem);
int32_t			List1AddFront(list1* const pls, void* pElem);
int32_t			List1AddFrontMove(list1* const pls, void* pElem);
int32_t			_List1RemoveIn(list1* const pls, size_t idx, size_t num);
int32_t			List1RemoveAt(list1* const pls, size_t idx);
int32_t			List1RemoveFront(list1* const pls, size_t num = 1);
int32_t			List1RemoveBack(list1* const pls, size_t num = 1);
int32_t			List1RemoveIn(list1* const pls, size_t idx, size_t num);
int32_t			List1RemoveFrom(list1* const pls, size_t idx);
int32_t			List1PopAt(list1* const pls, size_t idx, void* pElem);
int32_t			List1PopFront(list1* const pls, void* pElem);
int32_t			List1PopBack(list1* const pls, void* pElem);
int32_t			List1Insert(list1* const pls, void* pElem, size_t idx);
int32_t			List1InsertMove(list1* const pls, void* pElem, size_t idx);
int32_t			List1GetAt(list1* const pls, size_t idx, void* pElem);
int32_t			List1GetFront(list1* const pls, void* pElem);
int32_t			List1GetBack(list1* const pls, void* pElem);
int32_t			List1SetAt(list1* const pls, void* pElem, size_t idx);
int32_t			List1SetAtMove(list1* const pls, void* pElem, size_t idx);
int32_t			List1SetFront(list1* const pls, void* pElem);
int32_t			List1SetFrontMove(list1* const pls, void* pElem);
int32_t			List1SetBack(list1* const pls, void* pElem);
int32_t			List1SetBackMove(list1* const pls, void* pElem);
int32_t			List1FindFirst(list1* const pls, void* pElem, size_t* pIdx, funLS0ElemEquals fe = NULL);
int32_t			List1FindLast(list1* const pls, void* pElem, size_t* pIdx, funLS0ElemEquals fe = NULL);

// capacity
int32_t			_List1ExtendDLen(list1* const pls, size_t dLen);
int32_t			_List1ReCap(list1* const pls, size_t cap);
int32_t			List1ReCap(list1* const pls, size_t cap);
int32_t			List1ReLen(list1* const pls, size_t len);

// list
int32_t			List1MakeType(list1* const pls, const list1* const plsf);
int32_t			List1MakeTypeBy(list1* const pls, const list1* const plsf, size_t cap);
int32_t			List1CopyTo(list1* const pls, list1* const plsr);
//int32_t			List1Copy(list1* const pls, list1* const plsr, funLS0ElemAssignCopy fd);
int32_t			List1MoveTo(list1* const pls, list1* const plsr);
//int32_t			List1Move(list1* const pls, list1* const plsr, funLS0ElemAssignMove fd);
int32_t			List1Swap(list1* const pls, list1* const plsr);
//int32_t			List1Swap(list1* const pls, list1* const plsr, funLS0ElemSwap fd);
int32_t			List1CatList(list1* const pls, list1* const pls2);
int32_t			List1CatListMove(list1* const pls, list1* const pls2);
int32_t			List1InsertList(list1* const pls, list1* const pls2, size_t idx);
int32_t			List1InsertListMove(list1* const pls, list1* const pls2, size_t idx);
//int32_t		List1Attach(list1* const pls, list1* const pls2);
//int32_t		List1Detach(list1* const pls);

#pragma endregion list1

#define LIST1MAKE(pls, type) List1Make((pls), sizeof(type));
#define LIST1MAKEPURE(pls, type) List1MakePure((pls), sizeof(type));
#define LIST1GETAT(pls, idx, item) List1GetAt((pls), (idx), (void*)&(item));
#define LIST1ADD(pls, item) List1Add((pls), (void*)&(item));



#pragma region ptrlist1

#define PTRLIST1POS(pls, idx)		((void**)((char*)(pls)->ptr + idx * sizeof(void*)))
#define PTRLIST1AT(pls, idx)		(*(PTRLIST1POS(pls, idx)))

inline void		_PtrList1MoveTo(ptrlist1* const pls, size_t from, size_t to, size_t len);
inline void		_PtrList1CopyToList(ptrlist1* const pls, size_t from, ptrlist1* const pls2, size_t to, size_t len);
inline void		_PtrList1DeepCopyToList(ptrlist1* const pls, size_t from, ptrlist1* const pls2, size_t to, size_t len);
inline void		_PtrList1Make(ptrlist1* const pls, size_t from, size_t num);
inline void		_PtrList1Clear(ptrlist1* const pls, size_t from, size_t num);
int32_t			_PtrList1DeepCopy(ptrlist1* const pls, void* pElem, void** ppElemCopy);

void			PtrList1Init(ptrlist1* const pls);
int32_t			PtrList1Make(ptrlist1* const pls, const ptrlist0info* const pinfo = NULL);
int32_t			PtrList1MakeBy(ptrlist1* const pls, size_t cap, const ptrlist0info* const pinfo = NULL);
int32_t			PtrList1MakeByPure(ptrlist1* const pls, size_t cap, const ptrlist0info* const pinfo = NULL);
int32_t			PtrList1Clear(ptrlist1* const pls);
int32_t			PtrList1Abandon(ptrlist1* const pls);
int32_t			PtrList1Release(ptrlist1* const pls);
int32_t			PtrList1ReleaseAbandon(ptrlist1* const pls);

int32_t			PtrList1Add(ptrlist1* const ls, void* const p);
int32_t			PtrList1AddDeepCopy(ptrlist1* const pls, void* pElem);
int32_t			PtrList1AddFront(ptrlist1* const ls, void* const p);
int32_t			PtrList1AddFrontDeepCopy(ptrlist1* const pls, void* pElem);
int32_t			_PtrList1RemoveIn(ptrlist1* const pls, size_t idx, size_t num);
int32_t			PtrList1RemoveAt(ptrlist1* const pls, size_t idx);
int32_t			PtrList1RemoveFront(ptrlist1* const pls, size_t num = 1);
int32_t			PtrList1RemoveBack(ptrlist1* const pls, size_t num = 1);
int32_t			PtrList1RemoveIn(ptrlist1* const pls, size_t idx, size_t num);
int32_t			PtrList1RemoveFrom(ptrlist1* const pls, size_t idx);
int32_t			PtrList1PopAt(ptrlist1* const pls, size_t idx, void** pElem);
int32_t			PtrList1PopFront(ptrlist1* const pls, void** pElem);
int32_t			PtrList1PopBack(ptrlist1* const pls, void** pElem);
int32_t			PtrList1Insert(ptrlist1* const ls, void* const p, size_t idx);
int32_t			PtrList1InsertDeepCopy(ptrlist1* const ls, void* const p, size_t idx);
int32_t			PtrList1GetAt(ptrlist1* const pls, size_t idx, void** pElem);
int32_t			PtrList1GetFront(ptrlist1* const pls, void** pElem);
int32_t			PtrList1GetBack(ptrlist1* const pls, void** pElem);
int32_t			PtrList1SetAt(ptrlist1* const pls, void* pElem, size_t idx);
int32_t			PtrList1SetAtDeepCopy(ptrlist1* const pls, void* pElem, size_t idx);
int32_t			PtrList1SetFront(ptrlist1* const pls, void* pElem);
int32_t			PtrList1SetBack(ptrlist1* const pls, void* pElem);
int32_t			PtrList1SetBackDeepCopy(ptrlist1* const pls, void* pElem);
int32_t			PtrList1FindFirst(ptrlist1* const pls, void* pElem, size_t* pIdx, funPLS0ElemEquals fe);
int32_t			PtrList1FindLast(ptrlist1* const pls, void* pElem, size_t* pIdx, funPLS0ElemEquals fe);

int32_t			_PtrList1ExtendDLen(ptrlist1* const pls, size_t dLen);
int32_t			_PtrList1ReCap(ptrlist1* const pls, size_t cap);
int32_t			PtrList1ReCap(ptrlist1* const pls, size_t cap);
int32_t			PtrList1ReLen(ptrlist1* const pls, size_t len);

// list
int32_t			PtrList1CopyTo(ptrlist1* const pls, ptrlist1* const plsr);
int32_t			PtrList1DeepCopyTo(ptrlist1* const pls, ptrlist1* const plsr);
//int32_t			PtrList1Copy(ptrlist1* const pls, ptrlist1* const plsr, funLS0ElemAssignCopy fd);
int32_t			PtrList1MoveTo(ptrlist1* const pls, ptrlist1* const plsr);
//int32_t			List1Move(ptrlist1* const pls, ptrlist1* const plsr, funLS0ElemAssignMove fd);
int32_t			PtrList1Swap(ptrlist1* const pls, ptrlist1* const plsr);
//int32_t			PtrList1Swap(ptrlist1* const pls, ptrlist1* const plsr, funLS0ElemSwap fd);
int32_t			PtrList1CatList(ptrlist1* const pls, ptrlist1* const pls2);
int32_t			PtrList1CatListDeepCopy(ptrlist1* const pls, ptrlist1* const pls2);
int32_t			PtrList1InsertList(ptrlist1* const pls, ptrlist1* const pls2, size_t idx);
int32_t			PtrList1InsertListDeepCopy(ptrlist1* const pls, ptrlist1* const pls2, size_t idx);
//int32_t		PtrList1Attach(ptrlist1* const pls, ptrlist1* const pls2);
//int32_t		PtrList1Detach(ptrlist1* const pls);

#pragma endregion ptrlist1

inline void TestList()
{
	list1 ls0;
	int ret = LIST1MAKE(&ls0, int);

	int item = 123;
	ret = LIST1ADD(&ls0, item);
	item = 454;
	ret = LIST1ADD(&ls0, item);
	ret = LIST1GETAT(&ls0, 0, item);
}