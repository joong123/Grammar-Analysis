#pragma once

#include "def0.h"
#include "str0.h"
#include "list0.h"


typedef int64_t			id_t;


enum GRM_ELEM_TYPE
{
	STTELT_UNKNOWN		= 0,
	STTELT_RELATION		= 1, // EXPELT_OPERATOR
	STTELT_WORD			= 2 // EXPELT_OPERAND
};

enum GRM_WORD_CLASS
{
	GRMWCL_UNKNOWN		= 0,
	GRMWCL_WORD			= 1,
	GRMWCL_TYPE			= 2
};

//enum GRM_WORD_TYPE
//{
//	STTWDT_UNKNOWN		= 0,
//	STTWDT_NUMBER		= 1,
//	STTWDT_WORD			= 2,
//	STTWDT_CUSTOMIZED	= 4
//};

enum GRM_RELATION_TYPE
{
	STTRLT_UNKNOWN		= 0,
	STTRLT_RELATION		= 1,
	STTRLT_NORMAL		= 2,
	STTRLT_BRACKET		= 4,
	STTRLT_CUSTOMIZED	= 8
};

enum GRM_RELATION_CLASS
{
	STTRCL_UNKNOWN		= 0,
	STTRCL_RELATION		= 1,
	STTRCL_TYPE			= 2
};

enum GRM_RELATION_PART
{
	STTRLP_UNKNOWN		= 0,
	STTRLP_WHOLE		= 1,
	STTRLP_PART			= 2,
	STTRLP_PARTS		= 3
};

enum GRM_RELATION_BIND
{
	STTRBT_UNKNOWN		= 0,
	STTRBT_NONE			= 1,
	STTRBT_LEFT			= 2,
	STTRBT_RIGHT		= 4
};

// commutative law
enum GRM_RELATION_COMMUTATIVE
{
	STTRCM_UNKNOWN		= 0,
	STTRCM_NO			= 1,
	STTRCM_YES			= 2
};

// associative law
enum GRM_RELATION_ASSOCIATIVE
{
	STTRAS_UNKNOWN		= 0,
	STTRAS_NO			= 1,
	STTRAS_YES			= 2
};

enum GRM_RELATION_POSITION
{
	STTRPS_UNKNOWN		= 0,
	STTRPS_NONE			= 0,
	STTRPS_FRONT		= 0x01,
	STTRPS_MIDDLE		= 0x02,
	STTRPS_REAR			= 0x04,
	STTRPS_FM			= 0x03,
	STTRPS_FR			= 0x05,
	STTRPS_MR			= 0x06,
	STTRPS_FMR			= 0x07
};


// derived formula system definitions

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
	STTTMT_CHARS		= 4,
	STTTMT_FUN			= 8
};

// return
enum GRM_RETURN
{
	GRMR_FAIL			= -1,
	GRMR_NULL			= -2,
	GRMR_NULLARG		= -3,
	GRMR_ALLOC			= -4,
	GRMR_OVERFLOW		= -5,
	GRMR_INVSTATUS		= -6,
	GRMR_INVINDEX		= -7,

	GRMR_NONE			= 0,
	GRMR_SUCCEED		= 1
};
typedef GRM_RETURN	GRM_RET;



// TODOs:
// relation funEvaluate
// word type, get data
// FunAcceptWord serialize
// elem id
// 
// 6+2*3  6*2+3  6*2+3*5
// 623*+  62*3+  62*35*+

typedef struct grm_relation_def
{
	//id_t				id;
	wstr1				idtype;		// unique id. control position in 'normalized expression'
	wstr1				name;
	wstr1				desc;

	GRM_RELATION_TYPE	type;
	GRM_RELATION_CLASS	cls;
	GRM_RELATION_BIND	bind;		// bind
	GRM_RELATION_COMMUTATIVE comm;
	GRM_RELATION_ASSOCIATIVE asso;
	int32_t				priority;	// priority
	list1				parts;		// store #operand before each part operator

	wstr1				grammar;

	// redendunt

} grm_rela_def;


struct grm_elem;
typedef size_t (*FunAcceptChar)(ch c, grm_elem* elem);
typedef size_t (*FunAcceptWord)(const ch* p, size_t len, grm_elem* elem);
typedef size_t (*FunAcceptAllWord)(const ch* p, size_t len, list1* elems);

typedef struct grm_word_def
{
	//id_t				id;
	wstr1				idType;
	wstr1				name;
	wstr1				desc;

	//GRM_WORD_TYPE		type;
	GRM_WORD_CLASS		cls;

	FunAcceptChar		func;
	FunAcceptWord		funw;
	FunAcceptAllWord	funa;
} grm_word_def;

typedef struct grm_elem_def
{
	GRM_ELEM_TYPE	type;
	union
	{
		grm_relation_def	relation;
		grm_word_def		word;
	};
} grm_elem_def;


typedef struct grm_relation_data
{
	size_t		iPart;
	list1		parts;
	void*		p;
	uint64_t	padding;
} grm_rela;

typedef union grm_word_data
{
	wstr1		str;
	void*		p;
} grm_word;

typedef struct grm_elem
{
	GRM_ELEM_TYPE	type;
	union
	{
		grm_relation_data	relation;
		grm_word_data		word;
	};
} grm_elem;



//
// Derived-Formula Grammar System
//

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
	list2			toks;
} grm_drv;

typedef struct grm_formula
{
	grm_token		tok0;
	grm_derive		drv;
} grm_fml;

typedef struct grm_system
{
	grm_token		tok0;
	list2			fmls;
} grm_sys;