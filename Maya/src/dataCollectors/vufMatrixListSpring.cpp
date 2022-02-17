#include <dataCollectors/vufMatrixListSpring.h>
#include <vufMayaGlobalIncludes.h>
#include <data/vufMayaDataList.h>

#include <maya/MRampAttribute.h>
#include <maya/MCurveAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>

using namespace vufRM;
//using namespace vufMath;

MObject vufMatrixListSpringNode::g_data_in_deformed_attr;
MObject vufMatrixListSpringNode::g_data_in_rest_attr;

MObject	vufMatrixListSpringNode::g_in_enable_attr;
MObject	vufMatrixListSpringNode::g_in_init_frame_attr;
MObject	vufMatrixListSpringNode::g_in_time_attr;
MObject	vufMatrixListSpringNode::g_in_cache_mode_attr;

MObject	vufMatrixListSpringNode::g_in_translation_compound_attr;
MObject	vufMatrixListSpringNode::g_in_translation_iteration_attr;
MObject	vufMatrixListSpringNode::g_in_translation_stiffness_attr;
MObject	vufMatrixListSpringNode::g_in_translation_stiffness_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_translation_mass_attr;
MObject	vufMatrixListSpringNode::g_in_translation_mass_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_translation_damp_attr;
MObject	vufMatrixListSpringNode::g_in_translation_damp_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_translation_amplitude_attr;
MObject	vufMatrixListSpringNode::g_in_translation_amplitude_ramp_attr;

MObject	vufMatrixListSpringNode::g_in_rotation_compound_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_inherit_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_inherit_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_iteration_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_stiffness_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_stiffness_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_mass_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_mass_ramp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_damp_attr;
MObject	vufMatrixListSpringNode::g_in_rotation_damp_ramp_attr;
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
	setExistWithoutOutConnections(true);
	setExistWithoutInConnections(true);
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
	g_data_in_deformed_attr = l_typed_attr_fn.create(g_in_mlist_rest_long_s, g_in_mlist_rest_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
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
	// iteration count
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_iteration_attr, "itarationT", "itt", kInt, 10);
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(1));
	
	// stiffness
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_stiffness_attr, "stiffnessT", "stiffnessT", kInt, 1);
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
	
	// damp
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_damp_attr, "dumpT", "dumpT", kDouble, 1.0);
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
	// damp ramp
	g_in_translation_damp_ramp_attr = MCurveAttribute::createCurveAttr("dampCurveT", "dampCurveT", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// amplitude
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_translation_amplitude_attr, "amplitudeT", "amplitudeT", kDouble, 1.0);
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
	// amplitude ramp
	g_in_translation_amplitude_ramp_attr = MCurveAttribute::createCurveAttr("amplitudeCurveT", "amplitudeCurveT", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// compound
	g_in_translation_compound_attr = l_compound_attr_fn.create("translatePart", "translatePart", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_iteration_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_stiffness_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_stiffness_ramp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_mass_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_mass_ramp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_damp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_damp_ramp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_amplitude_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_translation_amplitude_ramp_attr));
	
	//-----------------------------------------------------------------------
	// translation part
	// iteration count
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_iteration_attr, "itarationR", "itr", kInt, 10);
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(1));

	// inherit translation
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_inherit_attr, "inheritTranslate", "inheritTranslate", kDouble, 1.0);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_numeric_attr_fn.setDefault(1.);
	l_numeric_attr_fn.setMin(.0);
	// inherit translation ramp
	g_in_rotation_inherit_ramp_attr = MCurveAttribute::createCurveAttr("inheritCurveR", "inherittCurveR", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// stiffness
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_stiffness_attr, "stiffnessR", "stiffnessR", kInt, 1);
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

	// damp
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_damp_attr, "dumpR", "dumpR", kDouble, 1.0);
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
	// damp ramp
	g_in_rotation_damp_ramp_attr = MCurveAttribute::createCurveAttr("dampCurveR", "dampCurveR", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// amplitude
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_rotation_amplitude_attr, "amplituder", "amplitudeR", kDouble, 1.0);
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setMin(.0));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
	// amplitude ramp
	g_in_rotation_amplitude_ramp_attr = MCurveAttribute::createCurveAttr("amplitudeCurveR", "amplitudeCurveR", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// compound
	g_in_rotation_compound_attr = l_compound_attr_fn.create("rotatePart", "rotatePart", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_iteration_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_inherit_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_inherit_ramp_attr));	
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_stiffness_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_stiffness_ramp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_mass_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_mass_ramp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_damp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_damp_ramp_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_amplitude_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_in_rotation_amplitude_ramp_attr));
	
	//output and cache
	// Output Data
	g_out_data_attr = l_typed_attr_fn.create(g_out_mlist_long_s, g_out_mlist_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(false);
	l_typed_attr_fn.setWritable(false);

	// cache data
	g_data_cache_attr = l_typed_attr_fn.create("hiddenCache", "hiddenCache", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setConnectable(false);
	l_typed_attr_fn.setHidden(true);

	//--------------------------------------------------------------------
	// add attributes
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_enable_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_init_frame_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_time_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_cache_mode_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_translation_compound_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_in_rotation_compound_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_out_data_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_data_cache_attr));
	//--------------------------------------------------------------------
	// attributes effects
	//CHECK_MSTATUS_AND_RETURN_IT()


	return MS::kSuccess;
}
