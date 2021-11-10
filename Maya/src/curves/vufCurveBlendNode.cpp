#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <curves/vufCurveBlendNode.h>
#include <curves/vufCurveContatiner.h>
#include <curves/implicit/vufCurveBlend.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveBlendNode::g_curve_container_1_attr;
MObject	vufCurveBlendNode::g_curve_container_2_attr;
MObject	vufCurveBlendNode::g_use_fcurve_attr;
MObject	vufCurveBlendNode::g_fcurve_attr;
MObject	vufCurveBlendNode::g_weight_attr;
MObject	vufCurveBlendNode::g_data_out_attr;

vufCurveBlendNode::vufCurveBlendNode():MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveBlendNode::creator()
{
	return new vufCurveBlendNode();
}
MStatus	vufCurveBlendNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;

	g_curve_container_1_attr = l_typed_attr_fn.create("inCurveA", "ica", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));

	g_curve_container_2_attr = l_typed_attr_fn.create("inCurveB", "icb", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// fcurve
	g_fcurve_attr = l_typed_attr_fn.create("inFCurve", "ifc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// global weight
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_weight_attr, "weight", "w", kDouble, 0.5);
	CHECK_MSTATUS(l_numeric_attr_fn.setKeyable(true)); 
	CHECK_MSTATUS(l_numeric_attr_fn.setMin(0));
	CHECK_MSTATUS(l_numeric_attr_fn.setMax(1));
	// use function
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_use_fcurve_attr, "useFcurve", "ufc", kBoolean, true);
	CHECK_MSTATUS(l_numeric_attr_fn.setKeyable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(true));
	// Output Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "ocd", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	// Add Attributes
	l_status = addAttribute(g_use_fcurve_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_weight_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_fcurve_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_curve_container_1_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_curve_container_2_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// Attribute affects
	l_status = attributeAffects(g_curve_container_1_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_curve_container_2_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_fcurve_attr,				g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_use_fcurve_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_weight_attr,				g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;

}
MStatus	vufCurveBlendNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		//vuf::vufTimer l_timer("Blend curve node compute: ");
		MStatus l_status;
		double l_weight		= p_data.inputValue(g_weight_attr).asDouble();
		bool l_use_fcurve	= p_data.inputValue(g_use_fcurve_attr).asBool();
		std::shared_ptr<vufCurveData> l_in_container_1;
		std::shared_ptr<vufCurveData> l_in_container_2;
		std::shared_ptr<vufCurveData> l_in_fcurve_container;
		std::shared_ptr<vufCurveData> l_out_data;
		//std::shared_ptr<vufCurveContainerData_4d> l_internal_data;

		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data);

		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_curve_container_1_attr, l_in_container_1);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_curve_container_2_attr, l_in_container_2);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_fcurve_attr,			l_in_fcurve_container);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		vufCurveContainer_4d& l_container = *(l_out_data->m_internal_data.get());
		l_container.switch_curve(0, vufCurveType::k_blend_curve);
		l_container.switch_quaternion_fn(vufCurveQuatFnType::k_compose);
		l_container.switch_scale_fn(vufCurveScaleFnType::k_compose);

		
		auto l_crv_ptr = l_container.get_curve_ptr();
		std::shared_ptr<vufCurveBlend_4d> l_blend_ptr = std::static_pointer_cast<vufCurveBlend_4d>( l_crv_ptr );
		l_blend_ptr->set_container_ptr_1(l_in_container_1 == nullptr ? nullptr : l_in_container_1->m_internal_data);
		l_blend_ptr->set_container_ptr_2(l_in_container_2 == nullptr ? nullptr : l_in_container_2->m_internal_data);
		l_blend_ptr->set_container_ptr_blend_func(l_in_fcurve_container == nullptr? nullptr: l_in_fcurve_container->m_internal_data);
		l_blend_ptr->set_weight_i(l_weight);
		l_blend_ptr->set_use_fcurve_i(l_use_fcurve);
		
		auto l_a = (l_in_container_1 == nullptr ? nullptr : l_in_container_1->m_internal_data);
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
/*
MStatus vufCurveBlendNode::connectionMade(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	if (p_plug_1 != vufCurveBlendNode::g_curve_container_1_attr &&
		p_plug_1 != vufCurveBlendNode::g_curve_container_2_attr &&
		p_plug_1 != vufCurveBlendNode::g_fcurve_attr)
	{
		return MPxNode::connectionMade(p_plug_1, p_plug_2, p_as_src);
	}
	MStatus l_status;
	MObject				l_data_obj;
	MFnDependencyNode	l_node(p_plug_1.node());
	MPlug				l_plug	= l_node.findPlug(g_data_out_attr,true,&l_status);
	// When we call plug::getValue compute method will be implicity called
	// Is garantee that data hs been created by compute method
	// We can skip all checking but keep them just in case (who knows may be maya api will be changed)
	l_status	= l_plug.getValue(l_data_obj); 
	MFnPluginData l_pd_fn(l_data_obj);
	mpxCurveWrapper* l_mpx_data = (mpxCurveWrapper*)l_pd_fn.constData(&l_status);	
	if (l_mpx_data == nullptr)
	{
		VF_LOG_ERR("vufCurveBlend Connection Made unexeptable fail");
		return MPxNode::connectionMade(p_plug_1, p_plug_2, p_as_src);
	}
	std::shared_ptr<vufCurveData> l_out_data = l_mpx_data->get_data();
	if (l_out_data == nullptr || l_out_data->m_internal_data == nullptr)
	{
		VF_LOG_ERR("vufCurveBlend Connection Made unexeptable fail");
		return MPxNode::connectionMade(p_plug_1, p_plug_2, p_as_src);
	}
	vufCurveContainer_4d& l_container = *(l_out_data->m_internal_data.get());
	auto l_blend_curve = l_container.get_curve_ptr()->as_curve_blend();
	if (l_blend_curve == nullptr)
	{
		VF_LOG_ERR("vufCurveBlend Connection Made unexeptable fail: blendcurve is null");
		return MPxNode::connectionMade(p_plug_1, p_plug_2, p_as_src);
	}
	std::cout << " plug_1: " << p_plug_1.name() << " p_plug_2: " << p_plug_2.name() << " source: " << p_as_src << std::endl;
	std::cout << " curve_plug: " << l_plug.name() << std::endl;
	if (p_plug_1 == vufCurveBlendNode::g_curve_container_1_attr)
	{
		l_blend_curve->make_selializable_first(false);
	}
	if (p_plug_1 == vufCurveBlendNode::g_curve_container_2_attr)
	{
		l_blend_curve->make_selializable_second(false);
	}
	if (p_plug_1 == vufCurveBlendNode::g_fcurve_attr)
	{
		l_blend_curve->make_selializable_fcurve(false);
	}
	std::cout << "I: " << l_blend_curve->is_serializable_first() << " II: " << l_blend_curve->is_serializable_second() << " FC: " << l_blend_curve->is_serializable_fcurve() << std::endl;
	
	std::cout << std::endl;
	return MPxNode::connectionMade(p_plug_1, p_plug_2, p_as_src);
}
*/


MStatus vufCurveBlendNode::connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	if (p_plug_1 != vufCurveBlendNode::g_curve_container_1_attr &&
		p_plug_1 != vufCurveBlendNode::g_curve_container_2_attr &&
		p_plug_1 != vufCurveBlendNode::g_fcurve_attr)
	{
		return MPxNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);
	}
	MStatus l_status;
	MObject				l_data_obj;
	MFnDependencyNode	l_node(p_plug_1.node());
	// When we call plug::getValue compute method will be implicity called
	// Is garantee that data hs been created by compute method
	// We can skip all checking but keep them just in case (who knows may be maya api will be changed)
	std::shared_ptr<vufCurveContainer_4d> l_crv_out_cntnr;
	if (p_plug_1 == vufCurveBlendNode::g_curve_container_1_attr)
	{
		MPlug	l_plug_in_a = l_node.findPlug(g_curve_container_1_attr, true, &l_status);
		std::shared_ptr<vufCurveContainer_4d> l_crv_input_a_cntnr;
		VF_RM_GET_INTERNAL_DATA_FROM_PLUG(mpxCurveWrapper, l_plug_in_a, l_crv_input_a_cntnr);
		std::shared_ptr<vufCurveContainer_4d> l_crv_input_a_cntnr_copy = (l_crv_input_a_cntnr == nullptr ? nullptr: l_crv_input_a_cntnr->get_copy());
		VF_RM_SET_INTERNAL_DATA_TO_PLUG(mpxCurveWrapper, vufCurveData, l_plug_in_a, l_crv_input_a_cntnr_copy);
	}
	if (p_plug_1 == vufCurveBlendNode::g_curve_container_2_attr)
	{
		MPlug	l_plug_in_b = l_node.findPlug(g_curve_container_2_attr, true, &l_status);
		std::shared_ptr<vufCurveContainer_4d> l_crv_input_b_cntnr;
		VF_RM_GET_INTERNAL_DATA_FROM_PLUG(mpxCurveWrapper, l_plug_in_b, l_crv_input_b_cntnr);
		std::shared_ptr<vufCurveContainer_4d> l_crv_input_b_cntnr_copy = (l_crv_input_b_cntnr == nullptr ? nullptr : l_crv_input_b_cntnr->get_copy());
		VF_RM_SET_INTERNAL_DATA_TO_PLUG(mpxCurveWrapper, vufCurveData, l_plug_in_b, l_crv_input_b_cntnr_copy);

	}
	if (p_plug_1 == vufCurveBlendNode::g_fcurve_attr)
	{
		MPlug	l_plug_in_f = l_node.findPlug(g_fcurve_attr, true, &l_status);
		std::shared_ptr<vufCurveContainer_4d> l_crv_input_f_cntnr;
		VF_RM_GET_INTERNAL_DATA_FROM_PLUG(mpxCurveWrapper, l_plug_in_f, l_crv_input_f_cntnr);
		std::shared_ptr<vufCurveContainer_4d> l_crv_input_f_cntnr_copy = (l_crv_input_f_cntnr == nullptr ? nullptr : l_crv_input_f_cntnr->get_copy());
		VF_RM_SET_INTERNAL_DATA_TO_PLUG(mpxCurveWrapper, vufCurveData, l_plug_in_f, l_crv_input_f_cntnr_copy);
	}
	return MPxNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);;
}

