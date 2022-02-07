#include <vufMayaGlobalIncludes.h>
#include <iostream>

//overloaded version to handle all those special std::endl and others...
std::ostream& operator,(std::ostream& out, std::ostream& (*f)(std::ostream&))
{
	out << f;
	return out;
}
//---------------------------------------------------------------------------------
// Plug names
//---------------------------------------------------------------------------------

const char* g_in_mbld_long_s	= "inXForms",			*g_in_mbld_s	= "ixs";
const char* g_out_mbld_long_s	= "outXForms",			*g_out_mbld_s	= "oxs";
const char* g_in_mlist_long_s	= "inMatrixList",		*g_in_mlist_s	= "iml";
const char* g_out_mlist_long_s	= "outMatrixList",		*g_out_mlist_s	= "oml";
const char* g_in_crv_long_s		= "inCurve",			*g_in_crv_s		= "ic";
const char* g_out_crv_long_s	= "outCurve",			*g_out_crv_s	= "oc";
const char* g_in_crvA_long_s	= "inCurveA",			*g_in_crvA_s	= "ica";
const char* g_in_crvB_long_s	= "inCurveB",			*g_in_crvB_s	= "icb";
const char* g_in_crvC_long_s	= "inCurveC",			*g_in_crvC_s	= "icc";
const char* g_in_crvD_long_s	= "inCurveD",			*g_in_crvD_s	= "icd";
const char* g_in_fcrv_long_s	= "inFCurve",			*g_in_fcrv_s	= "ifc";

/*
#pragma comment (lib, "OpenMaya.lib")
#pragma comment (lib, "OpenMayaAnim.lib")
#pragma comment (lib, "OpenMayaRender.lib")
#pragma comment (lib, "OpenMayaUI.lib")
#pragma comment (lib, "Foundation.lib")
*/

uint64_t vufRM::g_unique_id = 0;

