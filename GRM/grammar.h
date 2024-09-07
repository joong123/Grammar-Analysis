#pragma once

#include "grammardef.h"
#include "grammarbase.h"

#include "str0.h"
#include "list0.h"
#include "map0.h"



// THIS+NEXT	IDENTIFIER, SIGNS --- NDS, WSS, DIGITS, UNKNOWNS
int32_t AcceptOprGrammar(const ch* p, size_t len, ptrlist1*);


typedef struct grammar_elem_mgr
{
	ptrmap0			elemsDef;	// definition (general elems)
	ptrmap0			elemsItem;	// specific elems
} grm_elem_mgr;

struct grammar
{
	grm_elem_mgr	_elemMgr;

	// extra info
	ptrmap0			_mapEvaluate;// map<idx2_t, funEvaluate>
	list1			_lsComm;	// opr, opa, comm
	list1			_lsAsso;	// opa, opr, asso
	list1			_lsDist;	// opa, opr, opa, opr, opa, dir, dist
	list1			_lsComm2;	// opa, opr, opa, comm
	list1			_lsAsso2;	// opa, opr, opa, opr, opa, dir, asso

	// Derived-Formula Grammar System
	grammar_base_token_manager _grmTokMgr;
	base_grammar	_grm;
};

struct grammar_MathExp : grammar
{
};