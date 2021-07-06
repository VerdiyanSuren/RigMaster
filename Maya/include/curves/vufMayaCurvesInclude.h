#ifndef VF_RM_CRV_INCLUDE_H
#define VF_RM_CRV_INCLUDE_H

#include <vufMayaGlobalIncludes.h>
#pragma region REBUILD_ROUTINES 

#define VF_RM_CRV_NODE_DECLARE_REBUILD_ATTR()		\
	static MObject  g_rebuild_compound_attr;		\
	static MObject	g_rebuild_mode_attr;			\
	static MObject	g_samples_attr;					\
	static MObject	g_clamp_start_attr;				\
	static MObject	g_clamp_start_value_attr;		\
	static MObject	g_clamp_end_attr;				\
	static MObject	g_clamp_end_value_attr;			\
	static MObject	g_rebuild_offset_attr;

#define VF_RM_CRV_NODE_DEFINE_REBUILD_ATTR(CLASS_NAME)	\
	MObject CLASS_NAME::g_rebuild_compound_attr;		\
	MObject	CLASS_NAME::g_rebuild_mode_attr;			\
	MObject	CLASS_NAME::g_samples_attr;					\
	MObject	CLASS_NAME::g_clamp_start_attr;				\
	MObject	CLASS_NAME::g_clamp_start_value_attr;		\
	MObject	CLASS_NAME::g_clamp_end_attr;				\
	MObject	CLASS_NAME::g_clamp_end_value_attr;			\
	MObject	CLASS_NAME::g_rebuild_offset_attr;

#define VF_RM_CRV_NODE_INIT_REBUILD_ATTR()																\
	/** Assume 	l_status l_enum_attr_fn l_compound_attr_fn	l_numeric_attr_fn is already declared*/		\
	/* Mode */																							\
	g_rebuild_mode_attr = l_enum_attr_fn.create("applyRebuild", "arb", 0, &l_status);					\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Compute",	0));											\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Keep",		1));											\
	CHECK_MSTATUS(l_enum_attr_fn.addField("Disable",	2));											\
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(false));													\
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));													\
																										\
	/* Samples */																						\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_samples_attr,			"samples", "smp", kInt, 5);				\
	CHECK_MSTATUS(l_numeric_attr_fn.setMin(2));															\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_clamp_start_attr,		"clampStart",		"bst", kBoolean, false);\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_clamp_start_value_attr,"clampStartValue",	"bsv", kDouble, 0.0);	\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_clamp_end_attr,		"clampEnd",			"bnd", kBoolean, false);\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_clamp_end_value_attr,	"clampEndValue",	"bnv", kDouble, 0.0);	\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_rebuild_offset_attr,	"rebuildOffset",	"bof", kDouble, 0.0);	\
	/* Rebuild Compound */																				\
	g_rebuild_compound_attr = l_compound_attr_fn.create("rebuild", "rb", &l_status);					\
	CHECK_MSTATUS_AND_RETURN_IT(l_status);																\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_rebuild_mode_attr));									\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_samples_attr));											\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_clamp_start_attr));										\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_clamp_start_value_attr));								\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_clamp_end_attr));										\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_clamp_end_value_attr));									\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_rebuild_offset_attr));									\
	l_status = addAttribute(g_rebuild_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_RM_CRV_NODE_REBUILD_ATTR_AFFECT_TO(ATTR)															\
	l_status = attributeAffects(g_rebuild_compound_attr,	ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_rebuild_mode_attr,		ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_samples_attr,				ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_clamp_start_attr,			ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_clamp_start_value_attr,	ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_clamp_end_attr,			ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_clamp_end_value_attr,		ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_rebuild_offset_attr,		ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

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
	int		l_rbld_samples = p_data.inputValue(g_samples_attr).asInt();							\
	bool	l_rbld_pin_start = p_data.inputValue(g_clamp_start_attr).asBool();					\
	bool	l_rbld_pin_end = p_data.inputValue(g_clamp_end_attr).asBool();						\
	double	l_rbld_pin_start_value = p_data.inputValue(g_clamp_start_value_attr).asDouble();	\
	double	l_rbld_pin_end_value = p_data.inputValue(g_clamp_end_value_attr).asDouble();		\
	double	l_rbld_offset = p_data.inputValue(g_rebuild_offset_attr).asDouble();

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
	static MObject	g_scale_compound_attr;				\
	static MObject	g_scale_mode_attr;					\
	static MObject	g_scale_pin_start_attr;				\
	static MObject	g_scale_pin_start_value_attr;		\
	static MObject	g_scale_pin_end_attr;				\
	static MObject	g_scale_pin_end_value_attr;			\
	static MObject	g_scale_offset_attr;

#define VF_RM_CRV_NODE_DEFINE_SCALE_ATTR(CLASS_NAME)		\
	MObject	CLASS_NAME::g_scale_compound_attr;				\
	MObject	CLASS_NAME::g_scale_mode_attr;					\
	MObject	CLASS_NAME::g_scale_pin_start_attr;				\
	MObject	CLASS_NAME::g_scale_pin_start_value_attr;		\
	MObject	CLASS_NAME::g_scale_pin_end_attr;				\
	MObject	CLASS_NAME::g_scale_pin_end_value_attr;			\
	MObject	CLASS_NAME::g_scale_offset_attr;

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
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_pin_start_attr,			"scalePinStart",	"ssp", kBoolean, false);\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_pin_start_value_attr,	"scaleinStartValue","ssv", kDouble, 0.0);	\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_pin_end_attr,			"scalePinEnd",		"snp", kBoolean, false);\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_pin_end_value_attr,		"scalePinEndValue", "snv", kDouble, 1.0);	\
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_offset_attr,				"scaleOffset",		"sof", kDouble, 0.0);	\
																													\
	g_scale_compound_attr = l_compound_attr_fn.create("scale", "scl", &l_status);									\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_scale_mode_attr));													\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_scale_pin_start_attr));												\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_scale_pin_start_value_attr));										\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_scale_pin_end_attr));												\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_scale_pin_end_value_attr));											\
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_scale_offset_attr));												\
	l_status = addAttribute(g_scale_compound_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_RM_CRV_NODE_SCALE_ATTR_AFFECT_TO(ATTR)																\
	l_status = attributeAffects(g_scale_compound_attr,			ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_scale_mode_attr,				ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_scale_pin_start_attr,			ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_scale_pin_start_value_attr,	ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_scale_pin_end_attr,			ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_scale_pin_end_value_attr,		ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);	\
	l_status = attributeAffects(g_scale_offset_attr,			ATTR);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

#define VF_RM_CRV_NODE_READ_SCALE_ATTR()															\
	short	l_scale_mode			= p_data.inputValue(g_scale_mode_attr).asShort();				\
	bool	l_scale_pin_start		= p_data.inputValue(g_scale_pin_start_attr).asBool();			\
	double	l_scale_pin_start_value = p_data.inputValue(g_scale_pin_start_value_attr).asDouble();	\
	bool	l_scale_pin_end			= p_data.inputValue(g_scale_pin_end_attr).asBool();				\
	double	l_scale_pin_end_value	= p_data.inputValue(g_scale_pin_end_value_attr).asBool();		\
	double	l_scale_offset_value	= p_data.inputValue(g_scale_offset_attr).asBool();

#pragma endregion

#endif // !VF_RM_CRV_INCLUDE_H
