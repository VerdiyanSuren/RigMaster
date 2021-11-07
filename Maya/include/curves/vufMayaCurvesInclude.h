#ifndef VF_RM_CRV_INCLUDE_H
#define VF_RM_CRV_INCLUDE_H

#include <vufMayaGlobalIncludes.h>
#pragma region REBUILD_ROUTINES 

#define VF_RM_CRV_NODE_DECLARE_REBUILD_ATTR()		\
	static MObject  g_rebuild_compound_attr;		\
	static MObject	g_rebuild_mode_attr;			\
	static MObject	g_rebuild_samples_attr;			\
	static MObject	g_rebuild_store_attr;			\

#define VF_RM_CRV_NODE_DEFINE_REBUILD_ATTR(CLASS_NAME)	\
	MObject CLASS_NAME::g_rebuild_compound_attr;		\
	MObject	CLASS_NAME::g_rebuild_mode_attr;			\
	MObject	CLASS_NAME::g_rebuild_samples_attr;			\
	MObject	CLASS_NAME::g_rebuild_store_attr;


#define VF_RM_CRV_NODE_INIT_REBUILD_ATTR()																\
	/** Assume 	l_status l_enum_attr_fn l_compound_attr_fn	l_numeric_attr_fn is already declared*/		\
	/* Mode */																							\
	g_rebuild_mode_attr = l_enum_attr_fn.create("applyRebuild", "arb", 0, &l_status);					\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Compute",	0));											\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Keep",		1));											\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Disable",	2));											\
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));													\
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(0));														\
																										\
	/* Samples */																						\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_rebuild_samples_attr,	"samples", "smp", kInt, 5);				\
	CHECK_MSTATUS(l_numeric_attr_fn.setMin(1));															\
	/* Rebuild Compound */																				\
	g_rebuild_compound_attr = l_compound_attr_fn.create("rebuild", "rb", &l_status);					\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_rebuild_samples_attr));									\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_rebuild_mode_attr));									\
	l_status = addAttribute(g_rebuild_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);			\
	/* Rebuild hidden attribute*/																		\
	g_rebuild_store_attr = l_typed_attr_fn.create("hRebuild", "hrb", mpxCurveRebuildWrapper::g_id, MObject::kNullObj, &l_status);	\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																		\
	l_typed_attr_fn.setStorable(true);																			\
	l_typed_attr_fn.setConnectable(false);																		\
	l_typed_attr_fn.setHidden(true);																			\
	l_status = addAttribute(g_rebuild_store_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_RM_CRV_NODE_REBUILD_ATTR_AFFECT_TO(ATTR)															\
	l_status = attributeAffects(g_rebuild_compound_attr,	ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_rebuild_mode_attr,		ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_rebuild_samples_attr,		ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

/** We generate new variables 
l_rebuild_mode 
l_rbld_samples 
l_rbld_pin_start
l_rbld_pin_end
l_rbld_pin_start_value
l_rbld_pin_end_value
l_rbld_offset
*/
#define VF_RM_CRV_NODE_READ_REBUILD_ATTR()														\
	short	l_rebuild_mode = p_data.inputValue(g_rebuild_mode_attr).asShort();					\
	int		l_rbld_samples = p_data.inputValue(g_rebuild_samples_attr).asInt();					\

#pragma endregion
#pragma region QUATERNIONS ROUTINES
#define VF_RM_CRV_NODE_DECLARE_QUATERNIONS_ATTR()		\
	static MObject	g_quaternion_mode_attr;

#define VF_RM_CRV_NODE_DEFINE_QUATERNIONS_ATTR(CLASS_NAME)	\
	MObject	CLASS_NAME::g_quaternion_mode_attr;

#define VF_RM_CRV_NODE_INIT_QUATERNIONS_ATTR()																		\
	/** Assume 	l_status l_enum_attr_fn l_compound_attr_fn	l_numeric_attr_fn is already declared*/					\
	g_quaternion_mode_attr = l_enum_attr_fn.create("applyRotation", "ar", 0, &l_status);							\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																			\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Compute",	0));														\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Keep",		1));														\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Disable",	2));														\
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(false));																\
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));																\
	l_status = addAttribute(g_quaternion_mode_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_RM_CRV_NODE_QUATERNIONS_ATTR_AFFECT_TO(ATTR)																\
	l_status = attributeAffects(g_quaternion_mode_attr,				ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

/** We generate new variables
l_quat_mode
l_quat_pin_start
l_quat_pin_start_value
l_rbld_pin_end
l_quat_pin_end_value
l_quat_offset_value
*/
#define VF_RM_CRV_NODE_READ_QUATERNIONS_ATTR()															\
	short	l_quat_mode				= p_data.inputValue(g_quaternion_mode_attr).asShort();

#pragma endregion
#pragma region SCALE_ROUTINES
#define VF_RM_CRV_NODE_DECLARE_SCALE_ATTR()				\
	static MObject	g_scale_mode_attr;					\

#define VF_RM_CRV_NODE_DEFINE_SCALE_ATTR(CLASS_NAME)		\
	MObject	CLASS_NAME::g_scale_mode_attr;					\

#define VF_RM_CRV_NODE_INIT_SCALE_ATTR()																			\
	/** Assume 	l_status l_enum_attr_fn l_compound_attr_fn	l_numeric_attr_fn is already declared*/					\
	g_scale_mode_attr = l_enum_attr_fn.create("applyScale", "as", 0, &l_status);									\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																			\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Compute",	0));														\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Keep",		1));														\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Disable",	2));														\
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(false));																\
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));																\
																													\
	l_status = addAttribute(g_scale_mode_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_RM_CRV_NODE_SCALE_ATTR_AFFECT_TO(ATTR)																\
	l_status = attributeAffects(g_scale_mode_attr,				ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\

#define VF_RM_CRV_NODE_READ_SCALE_ATTR()															\
	short	l_scale_mode			= p_data.inputValue(g_scale_mode_attr).asShort();				\

#pragma endregion

#endif // !VF_RM_CRV_INCLUDE_H
