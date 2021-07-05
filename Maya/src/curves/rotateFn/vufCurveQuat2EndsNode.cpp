#include <curves/rotateFn/vufCurveQuat2EndsNode.h>
#include <curves/vufCurveContatiner.h>
#include <curves/quaternionFn/vufCurveQuaternion2EndsFn.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MMatrixArray.h>
#include <maya/MAngle.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveQuat2EndsNode::g_enable_attr;
//MObject	vufCurveQuat2EndsNode::g_quaternion_compound_attr;
MObject	vufCurveQuat2EndsNode::g_division_attr;
//MObject	vufCurveQuat2EndsNode::g_quaternion_mode_attr;

MObject	vufCurveQuat2EndsNode::g_fcurve_attr;
MObject	vufCurveQuat2EndsNode::g_blend_curve_attr;

MObject	vufCurveQuat2EndsNode::g_twist_start_attr;
MObject	vufCurveQuat2EndsNode::g_twist_end_attr;

MObject	vufCurveQuat2EndsNode::g_transfoms_start_attr;
MObject	vufCurveQuat2EndsNode::g_transfoms_end_attr;
MObject	vufCurveQuat2EndsNode::g_data_in_attr;
MObject	vufCurveQuat2EndsNode::g_data_out_attr;

vufCurveQuat2EndsNode::vufCurveQuat2EndsNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveQuat2EndsNode::creator()
{
	return new vufCurveQuat2EndsNode();
}
MStatus	vufCurveQuat2EndsNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnMatrixAttribute		l_matr_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	// enable/disable
	g_enable_attr = l_numeric_attr_fn.create("enable", "ar", MFnNumericData::kBoolean,false, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	l_status = addAttribute(g_enable_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	/* Division for quaternions sample*/
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_division_attr, "division", "div", kInt, 0);
	l_numeric_attr_fn.setMin(1);
	l_numeric_attr_fn.setDefault(10);
	l_status = addAttribute(g_division_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// start root matrix attribute
	g_transfoms_start_attr = l_matr_attr_fn.create("xformStart", "xfs", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	l_status = addAttribute(g_transfoms_start_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// end root matrix attribute
	g_transfoms_end_attr = l_matr_attr_fn.create("xformEnd", "xfe", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	l_status = addAttribute(g_transfoms_end_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// twist start
	g_twist_start_attr = l_unit_attr_fn.create("twistStart", "ts", MFnUnitAttribute::kAngle, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_unit_attr_fn.setChannelBox(true));
	l_status = addAttribute(g_twist_start_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// twist end
	g_twist_end_attr = l_unit_attr_fn.create("twistEnd", "te", MFnUnitAttribute::kAngle, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_unit_attr_fn.setChannelBox(true));
	l_status = addAttribute(g_twist_end_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// in data
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// in fcurve
	g_fcurve_attr = l_typed_attr_fn.create("fCurve", "fc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_fcurve_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// in blend curve
	g_blend_curve_attr = l_typed_attr_fn.create("bCurve", "bc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_blend_curve_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_enable_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_division_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transfoms_start_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transfoms_end_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_twist_start_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_twist_end_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_data_in_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_fcurve_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_blend_curve_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveQuat2EndsNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
#pragma region HANDLE_PARAMS
		bool	l_enable = p_data.inputValue(g_enable_attr).asBool();
		if (l_enable == false)
		{
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		int		l_division			= p_data.inputValue(g_division_attr).asInt();
		MMatrix l_matr_start		= p_data.inputValue(g_transfoms_start_attr).asMatrix();
		MMatrix l_matr_end			= p_data.inputValue(g_transfoms_end_attr).asMatrix();
		double	l_twist_start_value = p_data.inputValue(g_twist_start_attr).asAngle().asRadians();
		double	l_twist_end_value	= p_data.inputValue(g_twist_end_attr).asAngle().asRadians();
#pragma endregion


		std::shared_ptr<vufCurveData>		l_in_data;
		std::shared_ptr<vufCurveData>		l_out_data;
		std::shared_ptr<vufCurveData>		l_fcurve_data;
		std::shared_ptr<vufCurveData>		l_blend_data;

		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_data_in_attr,		l_in_data);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_fcurve_attr,		l_fcurve_data);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_blend_curve_attr,	l_blend_data);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
			auto l_quaterion_fn_ptr = l_out_data->m_internal_data->get_quaternion_fn_ptr();
			if (l_quaterion_fn_ptr == nullptr)
			{
				l_out_data->m_internal_data->set_quaternion_fn_ptr(vufCurveQuaternionFn_4d::create(vufCurveQuatFnType::k_2_ends));
			}
		}
		// get input curve
		if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)
		{
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		// get input fcurve
		std::shared_ptr<vufCurve< double, vufVector4> > l_fcurve_ptr = nullptr;
		if (l_fcurve_data != nullptr && l_fcurve_data->m_internal_data != nullptr)
		{
			l_fcurve_ptr = l_fcurve_data->m_internal_data->get_curve_ptr();
		}
		// get input blend curve
		std::shared_ptr<vufCurve< double, vufVector4> > l_blend_curve_ptr = nullptr;
		if (l_blend_data != nullptr && l_blend_data->m_internal_data != nullptr)
		{
			l_blend_curve_ptr = l_blend_data->m_internal_data->get_curve_ptr();
		}

		vufCurveContainer_4d& l_in_container = *(l_in_data->m_internal_data.get());
		vufCurveContainer_4d& l_out_container = *(l_out_data->m_internal_data.get());
		// copy pointers from in container to out except quat
		l_out_container.set_curve_ptr(		l_in_container.get_curve_ptr());
		l_out_container.set_rebuild_fn_ptr(	l_in_container.get_rebuild_fn_ptr());
		l_out_container.set_scale_fn_ptr(	l_in_container.get_scale_fn_ptr());
		//l_out_container.set_remap_fn_ptr(l_in_container.get_remap_fn_ptr());

		auto l_quat_ptr = l_out_data->m_internal_data->get_quaternion_fn_ptr()->as_2ends_fn();
		auto l_crv_ptr	= l_out_container.get_curve_ptr();

		l_quat_ptr->set_division_i(l_division, l_crv_ptr);
		l_quat_ptr->set_root_item_start_i(*((vufMatrix4<double>*)(&l_matr_start)));
		l_quat_ptr->set_root_item_end_i(*((vufMatrix4<double>*)(&l_matr_end)));
		l_quat_ptr->set_twist_start_mult_i(l_twist_start_value);
		l_quat_ptr->set_twist_end_mult_i(l_twist_end_value);
		l_quat_ptr->set_fcurve_i(l_fcurve_ptr);
		l_quat_ptr->set_blend_curve_i(l_blend_curve_ptr);

		l_quat_ptr->compute_bind_params_i(l_crv_ptr);
		l_quat_ptr->match_quaternions_i(l_out_container);

		p_data.setClean(g_data_out_attr);

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
