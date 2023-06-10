#pragma once

#include "grammardef.h"
#include "grammarbase.h"

#include "str0.h"
#include "list0.h"
#include "map0.h"



int32_t AcceptOprGrammar(const ch* p, size_t len, ptrlist1);


struct stt_elem_mgr
{
	ptrmap0			elems;
};

struct grammar
{
	ptrmap0			mapEvaluate;// map<idx2_t, funEvaluate>

	stt_elem_mgr	_elemMgr;

	grammar_base_token_manager _grmTokMgr;

	base_grammar	_grm;
};

struct grammar_MathExp : grammar
{
};