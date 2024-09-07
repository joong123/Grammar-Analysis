#pragma once

#include "grammardef.h"
#include <map>

using std::map;


enum GRM_BASECHAR
{
	GRCB_UNKNOWN		= 0x00000001,
	GRCB_NONDISPLAY		= 0x00000002,
	GRCB_WHITESPACE		= 0x00000004,
	GRCB_SIGN			= 0x00000008,
	GRCB_DIGIT			= 0x00000010,
	GRCB_LANGCHAR		= 0x00000020,

	GRCB_ALL = (GRCB_UNKNOWN | GRCB_NONDISPLAY | GRCB_WHITESPACE
		| GRCB_SIGN | GRCB_DIGIT | GRCB_LANGCHAR)
};

enum GRM_EXTCHAR
{
	GRCE_SPACE			= 0x0000040,
	GRCE_TAB			= 0x0000080,
	GRCE_NEWLINE		= 0x0000100,
	GRCE_DIGITP			= 0x0000200,
	GRCE_LANGCHARLO		= 0x0000400,
	GRCE_LANGCHARHI		= 0x0000800,

	GRCE_SPEACTAB = (GRCE_SPACE | GRCE_TAB),
	GRCE_ALL = (GRCE_SPACE | GRCE_TAB | GRCE_NEWLINE
		| GRCE_DIGITP | GRCE_LANGCHARLO | GRCE_LANGCHARHI)
};

enum GRM_BASEWORD
{
	GRWB_UNKNOWN		= 0x0001000,
	GRWB_NONDISPLAYS	= 0x0002000,
	GRWB_WHITESPACES	= 0x0004000,
	GRWB_SIGNS			= 0x0008000,
	GRWB_DIGITS			= 0x0010000,
	GRWB_LANGWORD		= 0x0020000,

	GRWB_ALL = (GRWB_UNKNOWN | GRWB_NONDISPLAYS | GRWB_WHITESPACES
		| GRWB_SIGNS | GRWB_DIGITS | GRWB_LANGWORD)
};

enum GRM_EXTWORD
{
	GRWE_SQUOTED		= 0x0040000,
	GRWE_DQUOTED		= 0x0080000,
	GRWE_BRACKETED		= 0x0100000,
	GRWE_SBRACKETED		= 0x0200000,
	GRWE_BRACED			= 0x0400000,
	GRWE_INT			= 0x0800000,
	GRWE_DECIMAL		= 0x1000000,
	GRWE_SCIENTIFIC		= 0x2000000,
	//GRWE_BINARY		= 0x4000000,
	//GRWE_OCTONARY		= 0x8000000,
	//GRWE_HEXIDECIMAL	= 0x10000000,
	GRWE_IDENTIFIER		= 0x4000000,
	GRWE_ESCAPED		= 0x8000000,

	GRWE_ALLWRAPPED = (GRWE_SQUOTED | GRWE_DQUOTED | GRWE_BRACKETED | GRWE_SBRACKETED | GRWE_BRACED),
	GRWE_REAL = (GRWE_INT | GRWE_DECIMAL),
	GRWE_ALLNUMBER = (GRWE_INT | GRWE_DECIMAL | GRWE_SCIENTIFIC),
	GRWE_ALL = (GRWE_ALLWRAPPED | GRWE_ALLNUMBER | GRWE_IDENTIFIER | GRWE_ESCAPED)
};

enum GRM_BASEELEM
{
	GREB_NONE			= 0,

	GREB_UNKNOWN1		= GRCB_UNKNOWN,
	GREB_NONDISPLAY1	= GRCB_NONDISPLAY,
	GREB_WHITESPACE1	= GRCB_WHITESPACE,
	GREB_SIGN1			= GRCB_SIGN,
	GREB_DIGIT			= GRCB_DIGIT,
	GREB_LANGCHAR		= GRCB_LANGCHAR,
	GREB_ALLBASECHAR = (GRCB_ALL),

	GREB_SPACE			= GRCE_SPACE,
	GREB_TAB			= GRCE_TAB,
	GREB_NEWLINE		= GRCE_NEWLINE,
	GREB_DIGITP			= GRCE_DIGITP,
	GREB_LANGCHARLO		= GRCE_LANGCHARLO,
	GREB_LANGCHARHI		= GRCE_LANGCHARHI,
	GREB_SPACETAB = (GRCE_SPEACTAB),

	GREB_UNKNOWNWORD	= GRWB_UNKNOWN,
	GREB_NONDISPLAYWORD	= GRWB_NONDISPLAYS,
	GREB_WHITESPACEWORD	= GRWB_WHITESPACES,
	GREB_SIGNWORD			= GRWB_SIGNS,
	GREB_DIGITWORD			= GRWB_DIGITS,
	GREB_LANGWORD		= GRWB_LANGWORD,
	GREB_ALLBASEWORD = (GRWB_ALL),
	
	GREB_SQUOTED		= GRWE_SQUOTED,
	GREB_DQUOTED		= GRWE_DQUOTED,
	GREB_BRACKETED		= GRWE_BRACKETED,
	GREB_SBRACKETED		= GRWE_SBRACKETED,
	GREB_BRACED			= GRWE_BRACED,
	GREB_INTEGER			= GRWE_INT,
	GREB_DECIMAL		= GRWE_DECIMAL,
	GREB_SCIENTIFIC		= GRWE_SCIENTIFIC,
	GREB_IDENTIFIER		= GRWE_IDENTIFIER,
	GREB_ESCAPED		= GRWE_ESCAPED,

	GREB_ALLWRAPPED = (GRWE_ALLWRAPPED),
	GREB_REAL = (GRWE_REAL),
	GREB_ALLEXTWORD = (GRWE_ALL),
	GREB_ALLWORD = (GREB_ALLBASEWORD | GREB_ALLEXTWORD),
	//GREB_BINARY,
	//GREB_OCTONARY,
	//GREB_HEXIDECIMAL

	GREB_COUNT = (29)
};

enum GRM_EXTELEM
{

};


//typedef int32_t(*fAcceptGREBX)(const ch* p, const size_t len, size_t* const lena);
typedef int32_t(*fAcceptGREBXW)(const wch* p, const size_t len, size_t* const lena);

typedef struct grammar_baseelem_w_def
{
	GRM_BASEELEM	key;
	const wch*		nameKey;
	fAcceptGREBXW	fun;
} grammar_baseelem_w_def;

int32_t ConvertGrmBaseElemType(const GRM_BASEELEM type, wstr1* const key);
int32_t ConvertGrmBaseElemTypeR(wstr1* const key, GRM_BASEELEM* const type);



// *******************************************************************
// Derived-Formula Grammar System
// *******************************************************************
typedef struct grammar_baseelem
{
	id_t			id;
	wstr1			name;
	wstr1			type;
} grm_baseelem;


typedef struct base_grammar
{
	grm_token		start;
	list1			fmls;
} base_grammar;

typedef map<id_t, grm_token_def*>	mapTokDef;
typedef map<id_t, base_grammar*>	mapTokGrammar;

typedef struct grammar_base_token_manager
{
	mapTokDef		baseTMNs;
	mapTokDef		baseNONTMNs;

	mapTokGrammar	nonTMN2Grm;
} grm_basetok_mgr;


int32_t GREBAcceptOnce(const ch* p, const size_t len, const GRM_BASEELEM key, size_t* const lena, GRM_BASEELEM* const keyo);
int32_t GREBAcceptOnceW(const wch* p, const size_t len, const GRM_BASEELEM key, size_t* const lena, GRM_BASEELEM* const keyo);

int32_t Accept_Unknown1(const ch* p, const size_t len, size_t* const lena);
int32_t Accept_NonDisplay1(const ch* p, const size_t len, size_t* lena);
int32_t Accept_WhiteSpace1(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Sign1(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Digit1(const ch* p, const size_t len, size_t* lena);
int32_t Accept_LangChar1(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Space(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Tab(const ch* p, const size_t len, size_t* lena);
int32_t Accept_NewLine(const ch* p, const size_t len, size_t* lena);
int32_t Accept_DigitP(const ch* p, const size_t len, size_t* lena);
int32_t Accept_LangCharLo(const ch* p, const size_t len, size_t* lena);
int32_t Accept_LangCharHi(const ch* p, const size_t len, size_t* lena);
int32_t Accept_UnknownWord(const ch* p, const size_t len, size_t* lena);
int32_t Accept_NonDisplayWord(const ch* p, const size_t len, size_t* lena);
int32_t Accept_WhiteSpaceWord(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Sign(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Digits(const ch* p, const size_t len, size_t* lena);
int32_t Accept_LangWord(const ch* p, const size_t len, size_t* lena);
int32_t Accept_SQuoted(const ch* p, const size_t len, size_t* lena);
int32_t Accept_DQuoted(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Bracketed(const ch* p, const size_t len, size_t* lena);
int32_t Accept_SBracketed(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Braced(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Integer(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Decimal(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Scientific(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Identifier(const ch* p, const size_t len, size_t* lena);
int32_t Accept_Escaped(const ch* p, const size_t len, size_t* lena);

int32_t Accept_BaseElem(const ch* p, const size_t len, grm_baseelem* elem, GRM_BASEELEM key);
int32_t Accept_BaseElemExtend(const ch* p, const size_t len, grm_baseelem* elem, wstr1* key);
int32_t Accept_BaseElemStt(const ch* p, const size_t len, list1* ls, GRM_BASEELEM keys);
int32_t Accept_BaseElemSttExtend(const ch* p, const size_t len, list1* ls, list1* keys);

