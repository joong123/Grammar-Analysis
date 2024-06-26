#pragma once

#include "grammardef.h"
#include "grammarbase.h"

#include "str0.h"
#include "list0.h"
#include "map0.h"



// THIS+NEXT	IDENTIFIER, SIGNS --- NDS, WSS, DIGITS, UNKNOWNS
int32_t AcceptOprGrammar(const ch* p, size_t len, ptrlist1*);


//
// Derived-Formula Grammar System
//
typedef struct grammar_elem_mgr
{
	ptrmap0			elemsDef;
	ptrmap0			elemsItem;
} grm_elem_mgr;

struct grammar
{
	ptrmap0			mapEvaluate;// map<idx2_t, funEvaluate>

	grm_elem_mgr	_elemMgr;

	grammar_base_token_manager _grmTokMgr;

	base_grammar	_grm;
};

struct grammar_MathExp : grammar
{
};