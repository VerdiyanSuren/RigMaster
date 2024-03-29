#ifndef VF_TP_GLBL_INCLDS_H
#define VF_TP_GLBL_INCLDS_H
#include <iostream>
#include <cstring>

//#define	VF_TP_OUTSOURCE
//#define	VF_TP_MAYA

namespace vufRM
{
	extern uint64_t g_unique_id;
}
//---------------------------------------------------------------------------------
// Register node and data
//---------------------------------------------------------------------------------
#define VF_REGISTER_DATA(DATA_TYPE)																								\
	l_status = l_plugin.registerData(	DATA_TYPE::g_type_name, DATA_TYPE::g_id, DATA_TYPE::creator);							\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_REGISTER_NODE(NODE_TYPE)																								\
	l_status = l_plugin.registerNode( NODE_TYPE::g_type_name, NODE_TYPE::g_id, & NODE_TYPE::creator, & NODE_TYPE::initialize);	\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_DEREGISTER_DATA(DATA_TYPE)																							\
	l_status = l_plugin.deregisterData(DATA_TYPE::g_id);																		\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_DEREGISTER_NODE(NODE_TYPE)																							\
	l_status = l_plugin.deregisterNode( NODE_TYPE::g_id);	\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_REGISTER_COMMAND(CMD_TYPE)	\
	l_status = l_plugin.registerCommand( CMD_TYPE::g_type_name, CMD_TYPE::creator, CMD_TYPE::newSyntax);	\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_DEREGISTER_COMMAND(CMD_TYPE)	\
	l_status = l_plugin.deregisterCommand( CMD_TYPE::g_type_name );	\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

//---------------------------------------------------------------------------------
// Attribete Creation macroces
//---------------------------------------------------------------------------------
// Lock Attribute
#define VF_RM_CREATE_AND_ADD_LOCK_ATTR() \
	g_lock_attr =	l_numeric_attr_fn.create("lock", "lc", MFnNumericData::kBoolean, false, &l_status); \
					CHECK_MSTATUS_AND_RETURN_IT(l_status);				\
					CHECK_MSTATUS(l_numeric_attr_fn.setDefault(false)); \
					CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));	\
					CHECK_MSTATUS(l_numeric_attr_fn.setChannelBox(true));\
	l_status = addAttribute(g_lock_attr);								\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

// Pass Attribute 
#define VF_RM_CREATE_AND_ADD_PASS_ATTR() \
	g_pass_attr =	l_numeric_attr_fn.create("pass", "ps", MFnNumericData::kBoolean, false, &l_status); \
					CHECK_MSTATUS_AND_RETURN_IT(l_status);				\
					CHECK_MSTATUS(l_numeric_attr_fn.setDefault(false)); \
					CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));	\
					CHECK_MSTATUS(l_numeric_attr_fn.setChannelBox(true));\
	l_status = addAttribute(g_pass_attr);								\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

// Create Numeric Attribute
#define VF_RM_CREATE_STORABLE_NUMERIC_ATTR(ATTR, LONG_NAME, SHORT_NAME, TYPE, DEFAULT_VALUE)					\
	ATTR = l_numeric_attr_fn.create( LONG_NAME, SHORT_NAME, MFnNumericData::##TYPE, DEFAULT_VALUE, &l_status);	\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																		\
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(DEFAULT_VALUE));													\
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));


//---------------------------------------------------------------------------------
// conversion macroces
//---------------------------------------------------------------------------------
#define VF_RM_FROM_MAYA_MATRIX_TO_VUFMATRIX_4X4( VUF_MATR, MAYA_MATR ) \
	std::memcpy( (void*)&VUF_MATR, (void*)&MAYA_MATR, 16*sizeof(double));

//---------------------------------------------------------------------------------
// Output macroces
//---------------------------------------------------------------------------------
#define VF_ASSERT_AND_RETURN_FALSE(...)									\
{																		\
std::cout, __VA_ARGS__, " ", __FILE__, " line ", __LINE__, std::endl;	\
return false;															\
}																		\

template <typename T>
std::ostream& operator,(std::ostream& out, const T& t)
{
	out << t;
	return out;
}
std::ostream& operator,(std::ostream& out, std::ostream& (*f)(std::ostream&));

//---------------------------------------------------------------------------------
// Plug names
//---------------------------------------------------------------------------------
// Input MMatrix (array builder)
extern const char* g_in_mbld_long_s,	*g_in_mbld_s;
// Output MMatrix (array builder)
extern const char* g_out_mbld_long_s,	*g_out_mbld_s;
// Input vufMatrixList
extern const char* g_in_mlist_long_s,		*g_in_mlist_s;
extern const char* g_in_mlist2_long_s,		*g_in_mlist2_s;
extern const char* g_in_mlist_rest_long_s,	*g_in_mlist_rest_s;

// Outt vufMatrixList
extern const char* g_out_mlist_long_s,	*g_out_mlist_s;
// Input Curve Data
extern const char* g_in_crv_long_s,		*g_in_crv_s;
// Output Curve Data
extern const char* g_out_crv_long_s,	*g_out_crv_s;
// Input Curve Data A B C D
extern const char* g_in_crvA_long_s, * g_in_crvA_s;
extern const char* g_in_crvB_long_s, * g_in_crvB_s;
extern const char* g_in_crvC_long_s, * g_in_crvC_s;
extern const char* g_in_crvD_long_s, * g_in_crvD_s;
extern const char* g_in_fcrv_long_s, * g_in_fcrv_s;
//---------------------------------------------------------------------------------
// Command flags 
//---------------------------------------------------------------------------------
#endif // !VF_TP_GLBL_INCLDS_H
