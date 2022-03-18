#include <dataCollectors/vufMatrixListSpring.h>
#include <vufMayaGlobalIncludes.h>
#include <data/vufMayaDataList.h>
#include <math/vufMatrix.h>
#include <math/vufQuaternion.h>

#include <maya/MRampAttribute.h>
#include <maya/MCurveAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFloatArray.h>
#include <maya/MTime.h>

using namespace vufRM;
using namespace vufMath;

#pragma region SPRING_MAYA_NODE
MObject vufMatrixListSpringNode::g_data_in_deformed_attr;
MObject vufMatrixListSpringNode::g_data_in_rest_attr;

MObject	vufMatrixListSpringNode::g_in_enable_attr;
MObject	vufMatrixListSpringNode::g_in_enable_lin_attr;
MObject	vufMatrixListSpringNode::g_in_enable_rot_attr;

MObject	vufMatrixListSpringNode::g_in_init_frame_attr;
MObject	vufMatrixListSpringNode::g_in_time_attr;
MObject	vufMatrixListSpringNode::g_in_cache_mode_attr;

//MObject	vufMatrixListSpringNode::g_in_translation_compound_attr;
MObject	vufMatrixListSpringNode::g_in_translation_iteration_attr;
MObject	vufMatrixListSpringNode::g_in_translation_stiffness_attr;
MObject	vufMatrixListSpringNode::g_in_translation_stiffness_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_translation_mass_attr;
MObject	vufMatrixListSpringNode::g_in_translation_mass_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_translation_dump_attr;
MObject	vufMatrixListSpringNode::g_in_translation_dump_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_translation_amplitude_attr;
MObject	vufMatrixListSpringNode::g_in_translation_amplitude_ramp_attr;

//MObject	vufMatrixListSpringNode::g_in_rotation_compound_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_inherit_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_inherit_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_iteration_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_stiffness_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_stiffness_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_mass_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_mass_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_dump_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_dump_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_amplitude_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_amplitude_ramp_attr;

MObject	vufMatrixListSpringNode::g_out_data_attr;
MObject	vufMatrixListSpringNode::g_data_cache_attr;//cache

vufMatrixListSpringNode::vufMatrixListSpringNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufMatrixListSpringNode::creator()
{
	return new vufMatrixListSpringNode();
}
void 	vufMatrixListSpringNode::postConstructor()
{
	MStatus l_status;
	MFloatArray l_pos_array;
	l_pos_array.append(0.0f);
	l_pos_array.append(0.33f);
	l_pos_array.append(0.66f);
	l_pos_array.append(1.0f);

	MFloatArray l_val_array;
	l_val_array.append(1.0f);
	l_val_array.append(1.0f);
	l_val_array.append(1.0f);
	l_val_array.append(1.0f);

	MPlug l_trn_stfn_plug(thisMObject(), vufMatrixListSpringNode::g_in_translation_stiffness_ramp_attr);
	MPlug l_trn_mass_plug(thisMObject(), vufMatrixListSpringNode::g_in_translation_mass_ramp_attr);
	MPlug l_trn_dump_plug(thisMObject(), vufMatrixListSpringNode::g_in_translation_dump_ramp_attr);
	MPlug l_trn_ampl_plug(thisMObject(), vufMatrixListSpringNode::g_in_translation_amplitude_ramp_attr);

	MPlug l_rot_ihrt_plug(thisMObject(), vufMatrixListSpringNode::g_in_rotation_inherit_ramp_attr);
	MPlug l_rot_stfn_plug(thisMObject(), vufMatrixListSpringNode::g_in_rotation_stiffness_ramp_attr);
	MPlug l_rot_mass_plug(thisMObject(), vufMatrixListSpringNode::g_in_rotation_mass_ramp_attr);
	MPlug l_rot_dump_plug(thisMObject(), vufMatrixListSpringNode::g_in_rotation_dump_ramp_attr);
	MPlug l_rot_ampl_plug(thisMObject(), vufMatrixListSpringNode::g_in_rotation_amplitude_ramp_attr);
	
	MCurveAttribute l_trn_stfn_crv(l_trn_stfn_plug);
	MCurveAttribute l_trn_mass_crv(l_trn_mass_plug);
	MCurveAttribute l_trn_dump_crv(l_trn_dump_plug);
	MCurveAttribute l_trn_ampl_crv(l_trn_ampl_plug);

	MCurveAttribute l_rot_ihrt_crv(l_rot_ihrt_plug);
	MCurveAttribute l_rot_stfn_crv(l_rot_stfn_plug);
	MCurveAttribute l_rot_mass_crv(l_rot_mass_plug);
	MCurveAttribute l_rot_dump_crv(l_rot_dump_plug);
	MCurveAttribute l_rot_ampl_crv(l_rot_ampl_plug);

	CHECK_MSTATUS(l_trn_stfn_crv.setCurve(l_val_array, l_pos_array));
	CHECK_MSTATUS(l_trn_mass_crv.setCurve(l_val_array, l_pos_array));
	CHECK_MSTATUS(l_trn_dump_crv.setCurve(l_val_array, l_pos_array));
	CHECK_MSTATUS(l_trn_ampl_crv.setCurve(l_val_array, l_pos_array));

	CHECK_MSTATUS(l_rot_ihrt_crv.setCurve(l_val_array, l_pos_array));
	CHECK_MSTATUS(l_rot_stfn_crv.setCurve(l_val_array, l_pos_array));
	CHECK_MSTATUS(l_rot_mass_crv.setCurve(l_val_array, l_pos_array));
	CHECK_MSTATUS(l_rot_dump_crv.setCurve(l_val_array, l_pos_array));
	CHECK_MSTATUS(l_rot_ampl_crv.setCurve(l_val_array, l_pos_array));

	setExistWithoutOutConnections(true);
	setExistWithoutInConnections(true);
}
MStatus vufMatrixListSpringNode::shouldSave(const MPlug& plug, bool& result)
{
	//translate curves
	if (plug == g_in_translation_stiffness_ramp_attr	|| plug.parent() == g_in_translation_stiffness_ramp_attr) 
	{
		result = true; // required to ensure curve attribute saved correctly
		return MS::kSuccess;
	}
	if (plug == g_in_translation_mass_ramp_attr			|| plug.parent() == g_in_translation_mass_ramp_attr)
	{
		result = true;
		return MS::kSuccess;
	}
	if (plug == g_in_translation_dump_ramp_attr			|| plug.parent() == g_in_translation_dump_ramp_attr)
	{
		result = true;
		return MS::kSuccess;
	}
	if (plug == g_in_translation_amplitude_ramp_attr	|| plug.parent() == g_in_translation_amplitude_ramp_attr)
	{
		result = true;
		return MS::kSuccess;
	}
	// rotate curves
	if (plug == g_in_rotation_inherit_ramp_attr		|| plug.parent() == g_in_rotation_inherit_ramp_attr)
	{
		result = true;
		return MS::kSuccess;
	}
	if (plug == g_in_rotation_stiffness_ramp_attr	|| plug.parent() == g_in_rotation_stiffness_ramp_attr)
	{
		result = true; // required to ensure curve attribute saved correctly
		return MS::kSuccess;
	}
	if (plug == g_in_rotation_mass_ramp_attr		|| plug.parent() == g_in_rotation_mass_ramp_attr)
	{
		result = true;
		return MS::kSuccess;
	}
	if (plug == g_in_rotation_dump_ramp_attr		|| plug.parent() == g_in_rotation_dump_ramp_attr)
	{
		result = true;
		return MS::kSuccess;
	}
	if (plug == g_in_rotation_amplitude_ramp_attr	|| plug.parent() == g_in_rotation_amplitude_ramp_attr)
	{
		result = true;
		return MS::kSuccess;
	}
	return MPxNode::shouldSave(plug, result);
}
MStatus	vufMatrixListSpringNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	
	// Input Data
	// deformed
	g_data_in_deformed_attr = l_typed_attr_fn.create(g_in_mlist_long_s, g_in_mlist_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// rest to get initial offsets
	g_data_in_rest_attr = l_typed_attr_fn.create(g_in_mlist_rest_long_s, g_in_mlist_rest_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	
	// enable
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_enable_attr, "enable", "en", kBoolean, true);
	// enable lin
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_enable_lin_attr, "enableT", "ent", kBoolean, true);
	// enable rot
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_enable_rot_attr, "enableR", "enr", kBoolean, true);
	//init frame
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_init_frame_attr, "startSim", "startSim", kInt, 1);
	// time
	g_in_time_attr = l_unit_attr_fn.create("time", "time", MFnUnitAttribute::kTime, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_unit_attr_fn.setStorable(true));
	//cache mode
	g_in_cache_mode_attr = l_enum_attr_fn.create("cacheMode", "cacheMode", 0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("pass", 0));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("toMemory", 1));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("fromMemory", 2));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setStorable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setChannelBox(true));
	//-----------------------------------------------------------------------
	// translation part
	{
		// iteration count
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_iteration_attr, "iterationT", "itt", kInt, 10);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(1));

		// stiffness
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_stiffness_attr, "stiffnessT", "stiffnessT", kDouble, 1.0);
		//CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setDefault(1));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// stiffness ramp
		g_in_translation_stiffness_ramp_attr = MCurveAttribute::createCurveAttr("stiffnessCurveT", "stiffnessCurveT", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		//  mass
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_mass_attr, "massT", "massT", kDouble, 1.0);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0001));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// mass ramp
		g_in_translation_mass_ramp_attr = MCurveAttribute::createCurveAttr("massCurveT", "massCurveT", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		// dump
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_dump_attr, "dumpT", "dumpT", kDouble, 1.0);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// dump ramp
		g_in_translation_dump_ramp_attr = MCurveAttribute::createCurveAttr("dumpCurveT", "dumpCurveT", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		// amplitude
		g_in_translation_amplitude_attr = l_numeric_attr_fn.create("amplitudeT", "amplitudeT", MFnNumericData::k3Double, 1.0, &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setDefault(1.0,1.0,1.0));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// amplitude ramp
		g_in_translation_amplitude_ramp_attr = MCurveAttribute::createCurveAttr("amplitudeCurveT", "amplitudeCurveT", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		// compound
		/*g_in_translation_compound_attr = l_compound_attr_fn.create("translatePart", "translatePart", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_iteration_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_stiffness_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_stiffness_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_mass_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_mass_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_dump_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_dump_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_amplitude_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_amplitude_ramp_attr));
		*/
	}
	//-----------------------------------------------------------------------
	// rotation part
	{
		// iteration count
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_iteration_attr, "iterationR", "itr", kInt, 10);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(1));

		// inherit translation
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_inherit_attr, "inheritTranslate", "inheritTranslate", kDouble, 1.0);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);
		l_numeric_attr_fn.setDefault(1.);
		l_numeric_attr_fn.setMin(.0);
		// inherit translation ramp
		g_in_rotation_inherit_ramp_attr = MCurveAttribute::createCurveAttr("inheritCurveR", "inheritCurveR", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		// stiffness
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_stiffness_attr, "stiffnessR", "stiffnessR", kDouble, 1.0);
		//CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(1));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// stiffness ramp
		g_in_rotation_stiffness_ramp_attr = MCurveAttribute::createCurveAttr("stiffnessCurveR", "stiffnessCurveR", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		//  mass
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_mass_attr, "massR", "massR", kDouble, 1.0);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// mass ramp
		g_in_rotation_mass_ramp_attr = MCurveAttribute::createCurveAttr("massCurveR", "massCurveR", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		// dump
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_dump_attr, "dumpR", "dumpR", kDouble, 1.0);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// dump ramp
		g_in_rotation_dump_ramp_attr = MCurveAttribute::createCurveAttr("dumpCurveR", "dumpCurveR", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		// amplitude
		g_in_rotation_amplitude_attr = l_numeric_attr_fn.create("amplitudeR", "amplitudeR", MFnNumericData::k3Double, 1.0, &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setDefault(1.0, 1.0, 1.0));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// amplitude ramp
		g_in_rotation_amplitude_ramp_attr = MCurveAttribute::createCurveAttr("amplitudeCurveR", "amplitudeCurveR", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		// compound
		/*
		g_in_rotation_compound_attr = l_compound_attr_fn.create("rotatePart", "rotatePart", &l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_status);
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_iteration_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_inherit_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_inherit_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_stiffness_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_stiffness_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_mass_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_mass_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_dump_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_dump_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_amplitude_attr));
		CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_amplitude_ramp_attr));
		*/
	}
	//output and cache
	// Output Data
	g_out_data_attr = l_typed_attr_fn.create(g_out_mlist_long_s, g_out_mlist_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(false);
	l_typed_attr_fn.setWritable(false);

	// cache data
	//g_data_cache_attr = l_typed_attr_fn.create("hiddenCache", "hiddenCache", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	//CHECK_MSTATUS_AND_RETURN_IT(l_status);
	//l_typed_attr_fn.setStorable(true);
	//l_typed_attr_fn.setConnectable(false);
	//l_typed_attr_fn.setHidden(true);

	//--------------------------------------------------------------------
	// add attributes
	{
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_enable_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_enable_lin_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_enable_rot_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_init_frame_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_time_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_cache_mode_attr));
		//CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_compound_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_iteration_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_stiffness_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_stiffness_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_mass_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_mass_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_dump_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_dump_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_amplitude_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_amplitude_ramp_attr));

		//CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_compound_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_iteration_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_inherit_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_inherit_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_stiffness_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_stiffness_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_mass_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_mass_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_dump_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_dump_ramp_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_amplitude_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_amplitude_ramp_attr));

		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_data_in_deformed_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_data_in_rest_attr));
		CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_out_data_attr));
		//CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_data_cache_attr));
	}
	//--------------------------------------------------------------------
	// attributes effects
	{
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_data_in_deformed_attr,	g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_data_in_rest_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_enable_attr,			g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_enable_lin_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_enable_rot_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_init_frame_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_time_attr,			g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_cache_mode_attr,		g_out_data_attr));

		//CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_compound_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_iteration_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_stiffness_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_stiffness_ramp_attr,	g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_mass_attr,			g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_mass_ramp_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_dump_attr,			g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_dump_ramp_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_amplitude_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_translation_amplitude_ramp_attr,	g_out_data_attr));

		//CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_compound_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_iteration_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_inherit_ramp_attr,	g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_inherit_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_stiffness_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_stiffness_ramp_attr, g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_mass_attr,			g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_mass_ramp_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_dump_attr,			g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_dump_ramp_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_amplitude_attr,		g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_rotation_amplitude_ramp_attr, g_out_data_attr));
	}

	return MS::kSuccess;
}
MStatus	vufMatrixListSpringNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_out_data_attr)
	{
		MStatus l_status;


		std::shared_ptr<vufMatrixListData>	l_out_data;
		std::shared_ptr<vufMatrixListData>	l_in_deformed_data;
		std::shared_ptr<vufMatrixListData>	l_in_rest_data;
		// read in deformed
		VF_RM_GET_DATA_FROM_IN(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_in_deformed_attr, l_in_deformed_data);
		if (l_in_deformed_data == nullptr || l_in_deformed_data->m_internal_data == nullptr)
		{
			VF_MAYA_NODE_LOG_ERR(" in data is null");
			p_data.setClean(g_out_data_attr);
			return MS::kSuccess;
		}
		// read in rest
		VF_RM_GET_DATA_FROM_IN(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_in_rest_attr, l_in_rest_data);
		if (l_in_rest_data == nullptr || l_in_rest_data->m_internal_data == nullptr)
		{
			VF_MAYA_NODE_LOG_ERR(" in rest data is null");
			p_data.setClean(g_out_data_attr);
			return MS::kSuccess;
		}
		// read out data
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_out_data_attr, l_out_data, m_gen_id);
		VF_CHECK_AND_CREATE_INTERNAL_DATA(l_out_data, vufObjectArray<vufMatrix_4d>);
		// set arrays
		auto&	l_in_deformed_array	= l_in_deformed_data->m_internal_data->m_array_v;
		auto&	l_in_rest_array		= l_in_deformed_data->m_internal_data->m_array_v;
		auto&	l_out_array			= l_out_data->m_internal_data->m_array_v;
		auto	l_in_deform_sz		= l_in_deformed_array.size();
		auto	l_in_rest_sz		= l_in_rest_array.size();
		auto	l_out_sz			= l_out_array.size();
		
		if (l_in_deform_sz < 1)
		{
			VF_MAYA_LOG_ERR("Defomed matrix list has zero size");
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (l_in_rest_sz != l_in_deform_sz)
		{
			VF_MAYA_LOG_ERR("Rest matrix list has zero size");
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (l_out_sz != l_in_deform_sz)
		{
			l_out_array = l_in_deformed_array;
		}
		
		m_engine.set_lists( l_in_deformed_array, l_out_array, l_in_rest_array);
		m_engine.init();
		
		// read attributes
		bool l_enable = p_data.inputValue(g_in_enable_attr).asBool();
		if (l_enable == false)
		{
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (m_knots_t_array.length() != l_in_deform_sz)
		{
			m_knots_t_array.setLength((unsigned int)l_in_deform_sz);
			double l_step = 1.0 / (double)(l_in_deform_sz - 1);
			for (int i = 0; i < (int)l_in_deform_sz; ++i)
			{
				m_knots_t_array[i] = (double)i * l_step;
			}
		}
		m_engine.set_enable_lin(p_data.outputValue(g_in_enable_lin_attr).asBool());
		m_engine.set_enable_rot(p_data.outputValue(g_in_enable_rot_attr).asBool());
		// read inputs
		{
			m_engine.set_init_time((double)p_data.inputValue(g_in_init_frame_attr).asInt());
			m_engine.set_time(p_data.inputValue(g_in_time_attr).asTime().value());
			m_engine.set_cache_mode((int)p_data.inputValue(g_in_cache_mode_attr).asShort());

			m_engine.set_lin_iter(p_data.inputValue(g_in_translation_iteration_attr).asInt());
			double	l_lin_stiff		= p_data.inputValue(g_in_translation_stiffness_attr).asDouble();
			double	l_lin_mass		= p_data.inputValue(g_in_translation_mass_attr).asDouble();
			double	l_lin_dump		= p_data.inputValue(g_in_translation_dump_attr).asDouble();
			auto&	l_lin_amp			= p_data.inputValue(g_in_translation_amplitude_attr).asDouble3();

			m_engine.set_rot_iter( p_data.inputValue(g_in_rotation_iteration_attr).asInt());
			double	l_rot_master		= p_data.inputValue(g_in_rotation_inherit_attr).asDouble();
			double	l_rot_stiff		= p_data.inputValue(g_in_rotation_stiffness_attr).asDouble();
			double	l_rot_mass		= p_data.inputValue(g_in_rotation_mass_attr).asDouble();
			double	l_rot_dump		= p_data.inputValue(g_in_rotation_dump_attr).asDouble();
			auto&	l_rot_amp			= p_data.inputValue(g_in_rotation_amplitude_attr).asDouble3();

			// read curves per knots
			MObject l_this = thisMObject();
			MCurveAttribute l_lin_stfn_crv(l_this, g_in_translation_stiffness_ramp_attr);
			MCurveAttribute l_lin_mass_crv(l_this, g_in_translation_mass_ramp_attr);
			MCurveAttribute l_lin_dump_crv(l_this, g_in_translation_dump_ramp_attr);
			MCurveAttribute l_lin_ampl_crv(l_this, g_in_translation_amplitude_ramp_attr);

			MCurveAttribute l_rot_ihrt_crv(l_this, g_in_rotation_inherit_ramp_attr);
			MCurveAttribute l_rot_stfn_crv(l_this, g_in_rotation_stiffness_ramp_attr);
			MCurveAttribute l_rot_mass_crv(l_this, g_in_rotation_mass_ramp_attr);
			MCurveAttribute l_rot_dump_crv(l_this, g_in_rotation_dump_ramp_attr);
			MCurveAttribute l_rot_ampl_crv(l_this, g_in_rotation_amplitude_ramp_attr);
			// read curve attributes
			l_lin_stfn_crv.getValuesAtPositions(m_knots_t_array, m_knots_lin_stif_array);
			l_lin_mass_crv.getValuesAtPositions(m_knots_t_array, m_knots_lin_mass_array);
			l_lin_dump_crv.getValuesAtPositions(m_knots_t_array, m_knots_lin_dump_array);
			l_lin_ampl_crv.getValuesAtPositions(m_knots_t_array, m_knots_lin_ampl_array);

			l_rot_ihrt_crv.getValuesAtPositions(m_knots_t_array, m_knots_rot_inhrt_array);
			l_rot_stfn_crv.getValuesAtPositions(m_knots_t_array, m_knots_rot_stif_array);
			l_rot_mass_crv.getValuesAtPositions(m_knots_t_array, m_knots_rot_mass_array);
			l_rot_dump_crv.getValuesAtPositions(m_knots_t_array, m_knots_rot_dump_array);
			l_rot_ampl_crv.getValuesAtPositions(m_knots_t_array, m_knots_rot_ampl_array);

			for (int i = 0; i < (int)l_in_deform_sz; ++i)
			{
				m_engine.m_lin_stif[i]		= l_lin_stiff	* m_knots_lin_stif_array[i];
				m_engine.m_lin_mass[i]		= l_lin_mass	* m_knots_lin_mass_array[i];
				m_engine.m_lin_damp[i]		= l_lin_dump	* m_knots_lin_dump_array[i];
				m_engine.m_lin_ampl[i].x	= l_lin_amp[0]	* m_knots_lin_ampl_array[i];
				m_engine.m_lin_ampl[i].y	= l_lin_amp[1]	* m_knots_lin_ampl_array[i];
				m_engine.m_lin_ampl[i].z	= l_lin_amp[2]	* m_knots_lin_ampl_array[i];


				m_engine.m_rot_inhr[i]		= l_rot_master	* m_knots_rot_inhrt_array[i];
				m_engine.m_rot_stif[i]		= l_rot_stiff	* m_knots_rot_stif_array[i];
				m_engine.m_rot_mass[i]		= l_rot_mass		* m_knots_rot_mass_array[i];
				m_engine.m_rot_damp[i]		= l_rot_dump		* m_knots_rot_dump_array[i];
				m_engine.m_rot_ampl[i].x	= l_rot_amp[0]	* m_knots_rot_ampl_array[i];
				m_engine.m_rot_ampl[i].y	= l_rot_amp[1]	* m_knots_rot_ampl_array[i];
				m_engine.m_rot_ampl[i].z	= l_rot_amp[2]	* m_knots_rot_ampl_array[i];
				//std::cout << " curve rot_inhr param: " << l_engine.m_knots_rot_inhr_arr[i] << std::endl;
			}
		}
		m_engine.solve();

		p_data.setClean(p_plug);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
#pragma endregion SPRING_MAYA_NODE

#pragma region SPRING_FAKE_SOLVER
bool vufFakeSpringSolver::init()
{
	if (m_in_ptr == nullptr || m_out_ptr == nullptr)
	{
		m_valid = false;
		return false;
	}
	auto l_size = m_in_ptr->size();
	if (l_size != m_lin_stif.size())
	{
		//init linear
		// linear part intitialize
		VF_MAYA_LOG_INFO("INIT");
		m_lin_stif.resize(l_size);
		m_lin_mass.resize(l_size);
		m_lin_damp.resize(l_size);
		m_lin_ampl.resize(l_size);

		m_lin_mstr_prev.resize(l_size);
		m_lin_p_curr.resize(l_size); 
		m_lin_p_prev.resize(l_size); 
		m_lin_res_offset.resize(l_size);
		for (int i = 0; i < (int)l_size; ++i)
		{
			m_lin_mstr_prev[i] = (*m_in_ptr)[i].get_translation_4();
			m_lin_p_curr[i] = m_lin_mstr_prev[i];
			m_lin_p_prev[i] = m_lin_mstr_prev[i];
		}
		// init rotaion part
		m_rot_inhr.resize(l_size);
		m_rot_stif.resize(l_size);
		m_rot_mass.resize(l_size);
		m_rot_damp.resize(l_size);
		m_rot_ampl.resize(l_size);
		m_rot_res_axis_offset.resize(l_size);
		m_rot_res_angle_offset.resize(l_size);

		m_rot_mstr_prev.resize(l_size);
		m_rot_mstr_vel.resize(l_size);
		m_rot_mstr_axis_prev.resize(l_size);
		m_x_axis_curr.resize(l_size);
		m_x_axis_prev.resize(l_size);
		for (int i = 0; i < (int)l_size; ++i)
		{
			m_rot_mstr_prev[i] = (*m_in_ptr)[i].get_translation_4();
			m_rot_mstr_vel[i].set();
			m_rot_mstr_axis_prev[i] = (*m_in_ptr)[i].get_axis_x_4();
			m_x_axis_curr[i] = m_rot_mstr_axis_prev[i];
			m_x_axis_prev[i] = m_rot_mstr_axis_prev[i];
		}

		m_valid = true;
		return true;
	}
	compute_radiuses();
	return m_valid;
}
void vufFakeSpringSolver::solve()
{
	if (m_valid == false)
	{
		return;
	}
	double l_delta_time = m_current_time - m_prev_time;
	//std::cout << "delta time " << l_delta_time << " init time " << m_init_time << std::endl;
	if (l_delta_time <= 0.0 || l_delta_time > 2.0 || m_current_time < m_init_time)
	{
		//std::memcpy(&(*m_out_ptr)[0][0], &(*m_mstr_ptr)[0][0], m_mstr_prev.size() * sizeof(double));
		int l_size = (int)m_lin_mstr_prev.size();
		for (int i = 0; i < l_size; ++i)
		{
			//linear part
			m_lin_mstr_prev[i] = (*m_in_ptr)[i].get_translation_4();
			m_lin_p_curr[i] = m_lin_mstr_prev[i];
			m_lin_p_prev[i] = m_lin_mstr_prev[i];
			// rotation part
			m_rot_mstr_prev[i] = (*m_in_ptr)[i].get_translation_4();
			m_rot_mstr_vel[i].set();
			m_rot_mstr_axis_prev[i] = (*m_in_ptr)[i].get_axis_x_4();
			m_x_axis_curr[i] = m_rot_mstr_axis_prev[i];
			m_x_axis_prev[i] = m_rot_mstr_axis_prev[i];

			(*m_out_ptr)[i] = (*m_in_ptr)[i];
		}
		m_prev_time = m_current_time;
		return;
	}
	if (l_delta_time > 0.0)
	{
		// time is changed. lets update state variable
		int l_size = (int)m_lin_mstr_prev.size();
		if (m_enable_lin == true)
		{
			solveVerletLinear(l_delta_time);
		}
		else
		{
			for (int i = 0; i < l_size; ++i)
			{
				m_lin_res_offset[i].set(); // set relative offset to zero
			}
		}
		if (m_enable_rot == true)
		{
			solveVerletRot(l_delta_time);
		}
		else
		{
			for (int i = 0; i < l_size; ++i)
			{
				m_rot_res_angle_offset[i] = 0.0;
				//(*m_out_ptr)[i].set_axis_x((*m_in_ptr)[i].get_axis_x_4());
				//(*m_out_ptr)[i].set_axis_y((*m_in_ptr)[i].get_axis_y_4());
				//(*m_out_ptr)[i].set_axis_z((*m_in_ptr)[i].get_axis_z_4());
			}
		}
		// apply result
		vufQuaternion_d l_quat_accum = vufQuaternion_d();
		for (int i = 0; i < l_size; ++i)
		{
			vufVector_4d  l_x = (*m_in_ptr)[i].get_axis_x_4().get_normalized();
			vufVector_4d  l_y = (*m_in_ptr)[i].get_axis_y_4().get_normalized();
			vufVector_4d  l_z = (*m_in_ptr)[i].get_axis_z_4().get_normalized();
			vufVector_4d  l_p = (*m_in_ptr)[i].get_translation_4();
			
			vufVector_4d l_pos_offset = m_lin_res_offset[i].get_mult_components(m_lin_ampl[i]);
			l_pos_offset = l_p + (l_x * l_pos_offset.x) + (l_y * l_pos_offset.y) + (l_z + l_pos_offset.z);
			
			(*m_out_ptr)[i].set_translation(l_pos_offset);
		}
	}
	m_prev_time = m_current_time;
	//int l_size = (int)m_lin_mstr_prev.size();
	/*
	for (int i = 0; i < l_size; ++i)
	{
		(*m_out_ptr)[i].set_translation(m_mstr_prev[i]);
	}
	*/
}
void vufFakeSpringSolver::solveVerletLinear(double p_delta_time)
{
	auto l_size = m_lin_mstr_prev.size();
	double l_dt = p_delta_time / (double)m_lin_iter;
	double l_inverse_iter = 1.0 / (double)m_lin_iter;

	for (int i = 0; i < (int)l_size; ++i)
	{
		auto l_mstr_pos = (*m_in_ptr)[i].get_translation_4();
		vufVector_4d l_dp = (l_mstr_pos - m_lin_mstr_prev[i] )* l_inverse_iter;
		for (int iter = 1; iter <= m_lin_iter; ++iter)
		{
			vufVector_4d l_matr_tmp = m_lin_mstr_prev[i] + l_dp * (double)iter;
			vufVector_4d l_a_hook = (l_matr_tmp - m_lin_p_curr[i]) * m_lin_stif[i] / m_lin_mass[i];
			vufVector_4d l_a_dump = (m_lin_p_prev[i] - m_lin_p_curr[i]) * m_lin_damp[i] / (l_dt * m_lin_mass[i]);
			vufVector_4d l_tmp_res = 2.0 * m_lin_p_curr[i] - m_lin_p_prev[i] + (l_a_hook  + l_a_dump) * l_dt * l_dt;
			m_lin_p_prev[i] = m_lin_p_curr[i];
			m_lin_p_curr[i] = l_tmp_res;
		}
		m_lin_mstr_prev[i] = l_mstr_pos;
		vufVector_4d  l_x = (*m_in_ptr)[i].get_axis_x_4().get_normalized();
		vufVector_4d  l_y = (*m_in_ptr)[i].get_axis_y_4().get_normalized();
		vufVector_4d  l_z = (*m_in_ptr)[i].get_axis_z_4().get_normalized();
		vufVector_4d  l_rel_pos = m_lin_p_curr[i] - l_mstr_pos;
		m_lin_res_offset[i].set(l_x.dot(l_rel_pos), l_y.dot(l_rel_pos), l_z.dot(l_rel_pos));
		//l_mstr_pos += (m_lin_p_curr[i] - l_mstr_pos).get_mult_components(m_lin_ampl[i]);
		//std::cout << "Amplitude: " << m_lin_ampl[i] << std::endl;
		//(*m_out_ptr)[i].set_translation(l_mstr_pos);
		//(*m_out_ptr)[i].set_translation(m_lin_p_curr[i]);
	}
}
void vufFakeSpringSolver::solveVerletRot(double p_delta_time)
{
	vufVector_4d l_x(1.0);
	auto l_size = m_rot_mstr_prev.size();
	double l_dt = p_delta_time / (double)m_rot_iter;
	double l_inverse_iter = 1.0 / (double)m_rot_iter;


	for (int i = 0; i < (int)l_size; ++i)
	{
		vufVector_4d	l_mstr_pos = (*m_in_ptr)[i].get_translation_4();
		vufVector_4d	l_master_x_curr		= (*m_in_ptr)[i].get_axis_x_4();
		//vufVector_4d	l_dp = (l_mstr_pos - m_rot_mstr_prev[i]) * l_inverse_iter;
		
		// compute inertial acceleration
		vufVector_4d	l_a_inertia =  2.0 * (l_mstr_pos - m_rot_mstr_prev[i] - m_rot_mstr_vel[i] * p_delta_time) / (p_delta_time * p_delta_time);
		l_a_inertia *= m_rot_inhr[i];
		m_rot_mstr_vel[i] = (l_mstr_pos - m_rot_mstr_prev[i]) / p_delta_time;
		m_rot_mstr_prev[i] = l_mstr_pos;
		// compute incremental quaternion to rotate master x axis from prev ro current
		double			l_angle				= m_rot_mstr_axis_prev[i].get_angle(l_master_x_curr)* l_inverse_iter;
		
		vufVector_4d	l_quat_axis			= -m_rot_mstr_axis_prev[i].get_cross(l_master_x_curr);
		l_quat_axis.normalize_in_place();
		
		vufQuaternion_d l_quat_incr(l_angle, l_quat_axis);
		if (std::abs(l_quat_axis.length()) < VF_MATH_EPSILON)		l_quat_incr = vufQuaternion_d();

		for (int iter = 1; iter <= m_rot_iter; ++iter)
		{
			//rotate master x axis by dq(delta quaternion) 
			m_rot_mstr_axis_prev[i] = l_quat_incr.rotate_vector_by_quaternion(m_rot_mstr_axis_prev[i]);
			
			// hooke acceleration
			double l_a_hooke_mag	= m_x_axis_curr[i].get_angle(m_rot_mstr_axis_prev[i]) * m_rot_stif[i] / m_rot_mass[i];
			vufVector_4d l_a_hook	= m_rot_mstr_axis_prev[i] - m_x_axis_curr[i];
			l_a_hook.make_ortho_to_in_place(m_x_axis_curr[i]);
			l_a_hook.normalize_in_place();
			l_a_hook *= l_a_hooke_mag;
			
			// damp acceleration
			vufVector_4d l_a_dump = (m_x_axis_prev[i] - m_x_axis_curr[i]) * m_rot_damp[i] / (l_dt);// *m_rot_mass[i]);
			l_a_dump.make_ortho_to_in_place(m_x_axis_curr[i]);
			// get_tangential plane
			vufVector_4d l_inert = l_a_inertia.get_ortho_to(m_x_axis_curr[i]);
			
			// verlet
			vufVector_4d l_tmp_res = 2.0 * m_x_axis_curr[i] - m_x_axis_prev[i] + (l_a_hook + l_a_dump /* + l_inert*/) * l_dt * l_dt;
			l_tmp_res.normalize_in_place();
			// wind up result on the circle
			l_angle = l_tmp_res.get_angle(m_x_axis_curr[i]);
			l_quat_axis = -m_x_axis_curr[i].get_cross(l_tmp_res);
			vufQuaternion_d l_quat(l_angle, l_quat_axis);
			l_tmp_res = l_quat.rotate_vector_by_quaternion(m_x_axis_curr[i]);

			m_x_axis_prev[i] = m_x_axis_curr[i];
			m_x_axis_curr[i] = l_tmp_res;
		}
		
		vufQuaternion_d l_quat = vufQuaternion_d::rotate_arc(l_master_x_curr, m_x_axis_curr[i]);
		m_rot_res_axis_offset[i] = l_quat.get_axis_as_v4();
		m_rot_res_angle_offset[i] = l_quat.get_angle();
		//vufQuaternion_d l_res()
		//(*m_out_ptr)[i].set_axis_x(m_x_axis_curr[i]);
		//(*m_out_ptr)[i].set_axis_y(m_rot_mstr_axis_prev[i]);
	}
}
void vufFakeSpringSolver::compute_radiuses()
{
	//compute radiuses
	auto& l_arr = *m_rest_ptr;
	int l_size = (int)l_arr.size();
	// if array size less than 2 elements
	if (l_size < 2) 
	{
		for (int i = 0; i < (int)l_arr.size(); ++i)
		{
			m_radius[i] = 1.0;
		}
		return;
	}
	
	
	for (int i = 0; i < (int)l_arr.size() - 1; ++i)
	{
		m_radius[i] = (l_arr[i + 1].get_translation_4() - l_arr[i].get_translation_4()).length();
	}
	m_radius.back() = m_radius[l_size - 2];
}
#pragma endregion SPRING_FAKE_SOLVER