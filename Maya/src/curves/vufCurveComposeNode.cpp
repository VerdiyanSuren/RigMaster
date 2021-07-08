#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>

#include <curves/vufCurveComposeNode.h>
#include <vufMayaGlobalIncludes.h>
#include <data/vufMayaDataList.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveComposeNode::g_enable_attr;
MObject	vufCurveComposeNode::g_operator_attr;
MObject	vufCurveComposeNode::g_weight_a_attr;
MObject	vufCurveComposeNode::g_weight_b_attr;
MObject	vufCurveComposeNode::g_data_a_in_attr;
MObject	vufCurveComposeNode::g_data_b_in_attr;
MObject	vufCurveComposeNode::g_data_out_attr;

vufCurveComposeNode::vufCurveComposeNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveComposeNode::creator()
{
	return new vufCurveComposeNode();
}
MStatus	vufCurveComposeNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	// 	enable
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_enable_attr, "enable", "en", kBoolean, true);
	l_status = addAttribute(g_enable_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// operator
	g_operator_attr = l_enum_attr_fn.create("operator", "op", 3, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_enum_attr_fn.addField("+", 0));
	CHECK_MSTATUS(l_enum_attr_fn.addField("*", 1));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(1));
	l_status = addAttribute(g_operator_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// weight a
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_weight_a_attr, "weightA", "wa", k3Double, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_weight_a_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// weight b
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_weight_b_attr, "weightB", "wb", k3Double, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_weight_b_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// in curve a
	// in curve
	g_data_a_in_attr = l_typed_attr_fn.create("inCurveA", "ica", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_a_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// in curve b
	g_data_b_in_attr = l_typed_attr_fn.create("inCurveB", "icb", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_b_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// out curve
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// Attributes affets
	l_status = attributeAffects(g_enable_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_operator_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_weight_a_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_weight_b_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_a_in_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_b_in_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveComposeNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		bool	l_enable = p_data.inputValue(g_enable_attr).asBool();
		std::shared_ptr<vufCurveData>		l_in_a_data;
		std::shared_ptr<vufCurveData>		l_in_b_data;
		std::shared_ptr<vufCurveData>		l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_data_a_in_attr, l_in_a_data);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_data_b_in_attr, l_in_b_data);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		if (l_in_a_data == nullptr || l_in_a_data->m_internal_data == nullptr)
		{
			MGlobal::displayError(name() + " inCurve is Null");
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}


		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}