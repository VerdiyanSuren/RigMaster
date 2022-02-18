#include <dataCollectors/vufMatrixListSpring.h>
#include <vufMayaGlobalIncludes.h>
#include <data/vufMayaDataList.h>
#include <math/vufMatrix.h>

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

MObject vufMatrixListSpringNode::g_data_in_deformed_attr;
MObject vufMatrixListSpringNode::g_data_in_rest_attr;

MObject	vufMatrixListSpringNode::g_in_enable_attr;
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
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_stiffness_attr, "stiffnessT", "stiffnessT", kDouble, 1);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(1));
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
		// stiffness ramp
		g_in_translation_stiffness_ramp_attr = MCurveAttribute::createCurveAttr("stiffnessCurveT", "stiffnessCurveT", &l_status);		
		CHECK_MSTATUS_AND_RETURN_IT(l_status);

		//  mass
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_mass_attr, "massT", "massT", kDouble, 1.0);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
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
		VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_stiffness_attr, "stiffnessR", "stiffnessR", kDouble, 1);
		CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(1));
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
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_data_in_deformed_attr, g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_data_in_rest_attr, g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_enable_attr, g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_init_frame_attr, g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_time_attr, g_out_data_attr));
		CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_cache_mode_attr, g_out_data_attr));

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
		auto& l_in_deformed_array	= l_in_deformed_data->m_internal_data->m_array_v;
		auto& l_in_rest_array		= l_in_deformed_data->m_internal_data->m_array_v;
		auto& l_out_array			= l_out_data->m_internal_data->m_array_v;
		auto	l_in_deform_sz	= l_in_deformed_array.size();
		auto	l_in_rest_sz	= l_in_rest_array.size();
		auto	l_out_sz		= l_out_array.size();
		vufFakeSpringSolver<double>& l_engine = m_engine;
		l_engine.set_lists(l_in_rest_array, l_in_deformed_array, l_out_array);
		l_engine.init(); // To Do avoid this initialization in each frame
		if (l_in_deform_sz < 1)
		{
			VF_MAYA_LOG_ERR("Defomed matrix list has zero size");
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (l_in_rest_sz != l_in_deform_sz)
		{
			VF_MAYA_LOG_ERR("Defomed matrix list and rest matrix list has to have the same size");
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (l_out_sz != l_in_deform_sz)
		{
			l_out_array = l_in_deformed_array;
		}
		// read engine
		// To Do implemetn later

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
		// read inputs
		{
			l_engine.m_init_frame	= p_data.inputValue(g_in_init_frame_attr).asInt();
			l_engine.set_time(p_data.inputValue(g_in_time_attr).asTime().as(MTime::kSeconds));
			l_engine.m_cache_mode	= (int)p_data.inputValue(g_in_cache_mode_attr).asShort();

			l_engine.m_lin_iter		= p_data.inputValue(g_in_translation_iteration_attr).asInt();
			double l_lin_stiff		= p_data.inputValue(g_in_translation_stiffness_attr).asDouble();
			double l_lin_mass		= p_data.inputValue(g_in_translation_mass_attr).asDouble();
			double l_lin_dump		= p_data.inputValue(g_in_translation_dump_attr).asDouble();
			auto l_lin_amp			= p_data.inputValue(g_in_translation_amplitude_attr).asDouble3();

			l_engine.m_rot_iter		= p_data.inputValue(g_in_rotation_iteration_attr).asInt();
			double l_rot_master		= p_data.inputValue(g_in_rotation_inherit_attr).asDouble();
			double l_rot_stiff		= p_data.inputValue(g_in_rotation_stiffness_attr).asDouble();
			double l_rot_mass		= p_data.inputValue(g_in_rotation_mass_attr).asDouble();
			double l_rot_dump		= p_data.inputValue(g_in_rotation_dump_attr).asDouble();
			auto l_rot_amp			= p_data.inputValue(g_in_rotation_amplitude_attr).asDouble3();

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
				l_engine.m_knots_lin_stif_arr[i]	= l_lin_stiff	* m_knots_lin_stif_array[i];
				l_engine.m_knots_lin_mass_arr[i]	= l_lin_mass	* m_knots_lin_mass_array[i];
				l_engine.m_knots_lin_damp_arr[i]	= l_lin_dump	* m_knots_lin_dump_array[i];
				l_engine.m_knots_lin_ampl_arr[i]	= l_lin_amp[0]	* m_knots_lin_ampl_array[i];
				l_engine.m_knots_lin_ampl_arr[i]	= l_lin_amp[1]	* m_knots_lin_ampl_array[i];
				l_engine.m_knots_lin_ampl_arr[i]	= l_lin_amp[2]	* m_knots_lin_ampl_array[i];


				l_engine.m_knots_rot_inhr_arr[i] = l_rot_master * m_knots_rot_inhrt_array[i];
				l_engine.m_knots_rot_stif_arr[i] = l_rot_stiff	* m_knots_rot_stif_array[i];
				l_engine.m_knots_rot_mass_arr[i] = l_rot_mass	* m_knots_rot_mass_array[i];
				l_engine.m_knots_rot_damp_arr[i] = l_rot_dump	* m_knots_rot_dump_array[i];
				l_engine.m_knots_rot_ampl_arr[i] = l_rot_amp[0] * m_knots_rot_ampl_array[i];
				l_engine.m_knots_rot_ampl_arr[i] = l_rot_amp[1] * m_knots_rot_ampl_array[i];
				l_engine.m_knots_rot_ampl_arr[i] = l_rot_amp[2] * m_knots_rot_ampl_array[i];
				//std::cout << " curve rot_inhr param: " << l_engine.m_knots_rot_inhr_arr[i] << std::endl;
			}
		}
		//l_engine.solve

		p_data.setClean(p_plug);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
