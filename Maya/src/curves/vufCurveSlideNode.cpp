#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MGlobal.h>

#include <curves/vufCurveSlideNode.h>
#include <curves/vufCurveContatiner.h>
#include <curves/implicit/vufCurveSlide.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveSlideNode::g_enable_attr;
MObject	vufCurveSlideNode::g_offset_attr;
MObject	vufCurveSlideNode::g_use_length_attr;
MObject	vufCurveSlideNode::g_length_attr;
MObject	vufCurveSlideNode::g_hardness_attr;
MObject	vufCurveSlideNode::g_start_attr;
MObject	vufCurveSlideNode::g_start_weight_attr;
MObject	vufCurveSlideNode::g_end_attr;
MObject	vufCurveSlideNode::g_end_weight_attr;
MObject	vufCurveSlideNode::g_data_in_attr;
MObject	vufCurveSlideNode::g_data_out_attr;

vufCurveSlideNode::vufCurveSlideNode():MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveSlideNode::creator()
{
	return new vufCurveSlideNode();
}
MStatus	vufCurveSlideNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;

	// 	enable
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_enable_attr, "enable", "en", kBoolean, true);
	l_status = addAttribute(g_enable_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// offset
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_offset_attr, "offset", "ofs", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_offset_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// use length
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_use_length_attr, "useLength", "ul", kBoolean, true);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_use_length_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// length
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_length_attr, "length", "lng", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_length_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// hardness
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_hardness_attr, "hardness", "hnd", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_hardness_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//start param
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_start_attr, "startParam", "stp", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_start_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//start param
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_start_weight_attr, "startPin", "sp", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_numeric_attr_fn.setMin(0.0);
	l_numeric_attr_fn.setMax(1.0);
	l_status = addAttribute(g_start_weight_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//end param
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_end_attr, "endParam", "etp", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_end_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//start param
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_end_weight_attr, "endPin", "ep", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_numeric_attr_fn.setMin(0.0);
	l_numeric_attr_fn.setMax(1.0);
	l_status = addAttribute(g_end_weight_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// in curve
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// out data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attributes affets
	l_status = attributeAffects(g_enable_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_offset_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_use_length_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_length_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_hardness_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_start_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_start_weight_attr,g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_end_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_end_weight_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);


	return MS::kSuccess;
}
MStatus	vufCurveSlideNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		bool	l_enable = p_data.inputValue(g_enable_attr).asBool();
		std::shared_ptr<vufCurveData>		l_in_data;
		std::shared_ptr<vufCurveData>		l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_data_in_attr, l_in_data);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)
		{
			MGlobal::displayError(name() + " inCurve is Null");
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		vufCurveContainer_4d& l_in_container	= *(l_in_data->m_internal_data.get());
		vufCurveContainer_4d& l_out_container	= *(l_out_data->m_internal_data.get());
		if (l_enable == false)
		{
			l_out_container.set_rebuild_fn_ptr(l_in_container.get_rebuild_fn_ptr());
			l_out_container.set_scale_fn_ptr(l_in_container.get_scale_fn_ptr());
			l_out_container.set_quaternion_fn_ptr(l_in_container.get_quaternion_fn_ptr());
			l_out_container.set_curve_ptr(l_in_container.get_curve_ptr());
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		l_out_container.switch_curve(0, vufMath::vufCurveType::k_slide_curve);
		auto l_crv = l_out_container.get_curve_ptr()->as_curve_slide();

		l_crv->set_hardness(	p_data.inputValue(g_hardness_attr).asDouble());
		l_crv->set_offset(		p_data.inputValue(g_offset_attr).asDouble());
		l_crv->set_start_param(	p_data.inputValue(g_start_attr).asDouble());
		l_crv->set_start_weight(p_data.inputValue(g_start_weight_attr).asDouble());
		l_crv->set_end_param(	p_data.inputValue(g_end_attr).asDouble());
		l_crv->set_end_weight(	p_data.inputValue(g_end_weight_attr).asDouble());
		l_crv->set_length(		p_data.inputValue(g_length_attr).asDouble());
		l_crv->set_use_lenght(	p_data.inputValue(g_use_length_attr).asBool());
		l_crv->set_container_ptr(l_in_data->m_internal_data);
		l_crv->update();

		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}
