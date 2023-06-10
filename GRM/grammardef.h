#pragma once

#include "def0.h"
#include "str0.h"
#include "list0.h"


typedef i64			id_t;


enum GRM_ELEM_TYPE
{
	STTELT_UNKNOWN	= 0,
	STTELT_RELATION	= 1, // EXPELT_OPERATOR
	STTELT_WORD		= 2 // EXPELT_OPERAND
};

//enum GRM_WORD_TYPE
//{
//	STTWDT_UNKNOWN		= 0,
//	STTWDT_NUMBER		= 1,
//	STTWDT_WORD			= 2,
//	STTWDT_CUSTOMIZED	= 3
//};

enum GRM_RELATION_TYPE
{
	STTRLT_UNKNOWN		= 0,
	STTRLT_NORMAL		= 1,
	STTRLT_BRACKET		= 2,
	STTRLT_CUSTOMIZED	= 4
};

enum GRM_RELATION_PART
{
	STTRLP_UNKNOWN	= 0,
	STTRLP_WHOLE	= 1,
	STTRLP_PART		= 2
};

enum GRM_TOKEN_TYPE
{
	STTTKT_UNKNOWN		= 0,
	STTTKT_TERMINAL		= 1,
	STTTKT_NONTERMINAL	= 2
};

enum GRM_TERMINAL_TYPE
{
	STTTMT_UNKNOWN		= 0,
	STTTMT_EMPTYCHAR	= 1,
	STTTMT_WORD			= 2,
	STTTMT_CHARS		= 3,
	STTTMT_FUN			= 4
};

enum GRM_RETURN
{
	GRMR_FAIL			= -1,
	GRMR_NULL		= -2,
	GRMR_NULLARG		= -3,
	GRMR_ALLOC			= -4,
	GRMR_OVERFLOW		= -5,
	GRMR_INVSTATUS		= -6,
	GRMR_INVINDEX		= -7,

	GRMR_NONE			= 0,
	GRMR_SUCCEED		= 1
};
typedef GRM_RETURN	GRM_RET;




// relation funEvaluate
// word type, get data
// FunAcceptWord serialize
// elem id
// 
// 6+2*3  6*2+3  6*2+3*5
// 623*+  62*3+  62*35*+


typedef struct grm_terminal_def
{
	GRM_TERMINAL_TYPE	type;
} stt_tmnl_u;

typedef struct grm_nonterminal_def
{

} stt_nontmnl_u;

typedef struct grm_token_def
{
	//id_t			id;
	wstr1			name;

	GRM_TOKEN_TYPE	type;
	union
	{
		grm_terminal_def	t;
		grm_nonterminal_def	nt;
	};
} stt_tok_def;

typedef struct grm_token
{
	//id_t			id;
	wstr1			name;
} stt_tok;

typedef struct grm_derive
{
	list1			toks;
} grm_drv;

typedef struct grm_formula
{
	grm_token		tok0;
	grm_derive		drv;
} grm_fml;

//typedef struct grm_grammar
//{
//	grm_token		tok0;
//	grm_derive		drv;
//} grm_fml;



typedef struct grm_relation_def
{
	GRM_RELATION_TYPE	type;

	bool				bEntity;

	//size_t			iPart;
	size_t				nPart;
	list1				parts;
} stt_rela_def_u;

typedef struct grm_relation
{
	size_t				iPart;
	GRM_RELATION_PART	part;
} stt_rela_u;


typedef struct grm_word_def
{
	//id_t			id;
	wstr1			name;

	wstr1			idtype;
} grm_word_def;

typedef union grm_word
{
	/*int32_t		i;
	int64_t		l;
	float		f;
	double		d;*/
	void*		p;
} grm_word;


struct grm_elem;
typedef int (*FunAcceptChar)(ch c, grm_elem* elem);
typedef int (*FunAcceptWord)(const ch* p, size_t len, grm_elem* elem);
typedef int (*FunAcceptAllWord)(const ch* p, size_t len, grm_elem** elem);

typedef struct grm_word_type_def
{
	//GRM_WORD_TYPE		type;

	bool				bHasDynamic;

	FunAcceptChar		func;
	FunAcceptWord		funw;
	FunAcceptAllWord	funa;
} grm_word_type_def;

typedef struct grm_elem_def
{
	//id_t			id;
	wstr1			name;
	wstr1			idType;

	wstr1			desc;

	GRM_ELEM_TYPE	type;
	union
	{
		grm_relation_def	relation;
		grm_word_type_def	word;
	};
} grm_elem_def;

typedef struct grm_elem_base
{
	id_t			id;
	wstr1			name;
	wstr1			idType;
} grm_elem_base;

typedef struct grm_elem
{
	id_t			id;
	wstr1			name;

	wstr1			idtype;

	GRM_ELEM_TYPE	type;
	union
	{
		grm_relation	relation;
		grm_word		word;
	};
} grm_elem;