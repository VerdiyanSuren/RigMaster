#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <curves/vufCurveBlendNode.h>
#include <curves/implicit/vufCurveBlend.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveBlendNode::g_curve_container_1_attr;
MObject	vufCurveBlendNode::g_curve_container_2_attr;
MObject	vufCurveBlendNode::g_weight_attr;
MObject	vufCurveBlendNode::g_data_out_attr;
vufCurveBlendNode::vufCurveBlendNode()
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

	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_weight_attr, "weight", "w", kDouble, 0.5);
	CHECK_MSTATUS(l_numeric_attr_fn.setKeyable(true)); 
	CHECK_MSTATUS(l_numeric_attr_fn.setMin(0));
	CHECK_MSTATUS(l_numeric_attr_fn.setMax(1));
	// Output Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "ocd", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	// Add Attributes
	l_status = addAttribute(g_curve_container_1_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_curve_container_2_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_weight_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// Attribute affects
	l_status = attributeAffects(g_curve_container_1_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_curve_container_2_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_weight_attr,				g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;

}
MStatus	vufCurveBlendNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		double l_weight = p_data.inputValue(g_weight_attr).asDouble();

		std::shared_ptr<vufCurveData> l_in_container_1;
		std::shared_ptr<vufCurveData> l_in_container_2;
		std::shared_ptr<vufCurveData> l_out_data;
		//std::shared_ptr<vufCurveContainerData_4d> l_internal_data;

		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data);

		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_curve_container_1_attr, l_in_container_1);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_curve_container_2_attr, l_in_container_2);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		vufCurveContainer_4d& l_container = *(l_out_data->m_internal_data.get());
		l_container.switch_curve(0, vufCurveType::k_blend_curve);
		l_container.switch_quaternion_fn(vufCurveQuatFnType::k_blend);
		
		auto l_crv_ptr = l_container.get_curve_ptr();
		std::shared_ptr<vufCurveBlend_4d> l_blend_ptr = std::static_pointer_cast<vufCurveBlend_4d>( l_crv_ptr );
		l_blend_ptr->set_container_ptr_1(l_in_container_1->m_internal_data);
		l_blend_ptr->set_container_ptr_2(l_in_container_2->m_internal_data);
		l_blend_ptr->set_weight(l_weight);
		
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
