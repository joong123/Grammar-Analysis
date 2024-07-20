#include "pch.h"
#define LIB_EXPORT
#include "grammarbase.h"


const size_t sizeGrmWordDef = sizeof(grm_word_def);
const size_t sizeGrmRelationDef = sizeof(grm_relation_def);
const size_t sizeGrmElemDef = sizeof(grm_elem_def);

const size_t sizeGrmWordData = sizeof(grm_word_data);
const size_t sizeGrmRelationData = sizeof(grm_relation_data);
const size_t sizeGrmElemData = sizeof(grm_elem);

static grammar_baseelem_w_def GRM_BASEELEMW_INFO[] =
{
	// TODO funptr
	{ GRM_BASEELEM::GREB_NONE, L"B_NONE", NULL },
	{ GRM_BASEELEM::GREB_UNKNOWN1, L"B_UNK1", NULL },
	{ GRM_BASEELEM::GREB_NONDISPLAY1, L"B_ND1", NULL },
	{ GRM_BASEELEM::GREB_WHITESPACE1, L"B_WS1", NULL },
	{ GRM_BASEELEM::GREB_SIGN1, L"B_SIGN1", NULL },
	{ GRM_BASEELEM::GREB_DIGIT, L"B_DIGIT", NULL },
	{ GRM_BASEELEM::GREB_LANGCHAR, L"B_CHAR", NULL },
	//{ GRM_BASEELEM::GREB_ALLBASECHAR, L"B_ALLBCHAR", NULL },
	{ GRM_BASEELEM::GREB_SPACE, L"B_SPACE", NULL },
	{ GRM_BASEELEM::GREB_TAB, L"B_TAB", NULL },
	{ GRM_BASEELEM::GREB_NEWLINE, L"B_LF", NULL },
	{ GRM_BASEELEM::GREB_DIGITP, L"B_DGTP", NULL },
	{ GRM_BASEELEM::GREB_LANGCHARLO, L"B_CHARL", NULL },
	{ GRM_BASEELEM::GREB_LANGCHARHI, L"B_CHARH", NULL },
	//{ GRM_BASEELEM::GREB_SPACETAB, L"GREB_SPACETAB", NULL },
	{ GRM_BASEELEM::GREB_UNKNOWNWORD, L"B_UNKW", NULL },
	{ GRM_BASEELEM::GREB_NONDISPLAYWORD, L"B_NDW", NULL },
	{ GRM_BASEELEM::GREB_WHITESPACEWORD, L"B_WSS", NULL },
	{ GRM_BASEELEM::GREB_SIGNWORD, L"B_SIGNS", NULL },
	{ GRM_BASEELEM::GREB_DIGITWORD, L"B_DGTS", NULL },
	{ GRM_BASEELEM::GREB_LANGWORD, L"B_WORD", NULL },
	//{ GRM_BASEELEM::GREB_ALLBASEWORD, L"GREB_ALLBASEWORD", NULL },
	{ GRM_BASEELEM::GREB_SQUOTED, L"B_SQT", NULL },
	{ GRM_BASEELEM::GREB_DQUOTED, L"B_DQT", NULL },
	{ GRM_BASEELEM::GREB_BRACKETED, L"B_BKT", NULL },
	{ GRM_BASEELEM::GREB_SBRACKETED, L"B_SBKT", NULL },
	{ GRM_BASEELEM::GREB_BRACED, L"B_BRC", NULL },
	{ GRM_BASEELEM::GREB_INTEGER, L"B_INT", NULL },
	{ GRM_BASEELEM::GREB_DECIMAL, L"B_DEC", NULL },
	{ GRM_BASEELEM::GREB_SCIENTIFIC, L"B_SCI", NULL },
	{ GRM_BASEELEM::GREB_IDENTIFIER, L"B_IDF", NULL },
	{ GRM_BASEELEM::GREB_ESCAPED, L"B_ESCP", NULL },
	//{ GRM_BASEELEM::GREB_ALLWRAPPED, L"GREB_ALLWRAPPED", NULL },
	//{ GRM_BASEELEM::GREB_REAL, L"GREB_REAL", NULL },
	//{ GRM_BASEELEM::GREB_ALLEXTWORD, L"GREB_ALLEXTWORD", NULL },
	//{ GRM_BASEELEM::GREB_ALLWORD, L"GREB_ALLWORD", NULL },
};
static const size_t nGrmBaseElemInfo = sizeof(GRM_BASEELEMW_INFO) / sizeof(grammar_baseelem_w_def);

static size_t GetGRMBASEELEMKEYIDX(const GRM_BASEELEM key)
{
	size_t idx = 0;
	for (idx = 0; idx < nGrmBaseElemInfo; ++idx)
	{
		if (key == GRM_BASEELEMW_INFO[idx].key)
		{
			return idx;
		}
	}
	return idx;// overflow
}

int32_t ConvertGrmBaseElemType(const GRM_BASEELEM type, wstr1* const key)
{
	const size_t idx = GetGRMBASEELEMKEYIDX(type);
	if (idx < 0 || idx >= GRM_BASEELEM::GREB_COUNT)
		return GRMR_INVINDEX;
	return WStr1MakeCS(key, GRM_BASEELEMW_INFO[idx].nameKey);
}

int32_t ConvertGrmBaseElemTypeR(wstr1* const key, GRM_BASEELEM* const type)
{
	if (WStr1IsNullP(key) != 0 || NULL == type)
		return GRMR_NULLARG;

	for (size_t idx = 0; idx < nGrmBaseElemInfo; ++idx)
	{
		if (WStr1EqualsCS(key, GRM_BASEELEMW_INFO[idx].nameKey))
		{
			*type = GRM_BASEELEMW_INFO[idx].key;
			return GRMR_SUCCEED;
		}
	}

	return GRMR_FAIL;
}

int32_t GREBAcceptOnceW(const wch* p, const size_t len
	, const GRM_BASEELEM key, size_t* const lena, GRM_BASEELEM* const keyo)
{
	if (NULL == p || len <= 0 || NULL == lena || NULL == keyo)
		return GRET_NULLARG;

	for (size_t idx = nGrmBaseElemInfo; idx > 0;)
	{
		--idx;
		const fAcceptGREBXW pfAccept = GRM_BASEELEMW_INFO[idx].fun;
		const GRM_BASEELEM keyn = GRM_BASEELEMW_INFO[idx].key;
		if (!(keyn & key))
			continue;

		int32_t ret = 0;
		size_t lena2 = 0;
		if (pfAccept)
			ret = pfAccept(p, len, &lena2);
		if (ret > 0)
		{
			*lena = lena2;
			*keyo = keyn;
			return ret;
		}
	}
	return GRET_NONE;
}