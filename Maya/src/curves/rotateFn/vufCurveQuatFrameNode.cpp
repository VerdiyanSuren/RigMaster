//#include <curves/vufCurve.h>
#include <curves/vufCurveContatiner.h>
#include <curves/rotateFn/vufCurveQuatFrameNode.h>
#include <curves/quaternionFn/vufCurveQuaternionTransportFn.h>
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

MObject	vufCurveQuatFrameNode::g_quaternion_compound_attr;
MObject	vufCurveQuatFrameNode::g_quaternion_division_attr;
MObject	vufCurveQuatFrameNode::g_quaternion_mode_attr;
MObject	vufCurveQuatFrameNode::g_quaternion_offset_attr;

MObject	vufCurveQuatFrameNode::g_quaternion_fcurve_attr;
MObject	vufCurveQuatFrameNode::g_quaternion_twist_attr;


MObject	vufCurveQuatFrameNode::g_transfoms_attr;
MObject	vufCurveQuatFrameNode::g_data_in_attr;
MObject	vufCurveQuatFrameNode::g_data_out_attr;
MObject	vufCurveQuatFrameNode::g_data_store_attr;

vufCurveQuatFrameNode::vufCurveQuatFrameNode():MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveQuatFrameNode::creator()
{
	return new vufCurveQuatFrameNode();
}
MStatus	vufCurveQuatFrameNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnMatrixAttribute		l_matr_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;

	g_quaternion_mode_attr = l_enum_attr_fn.create("applyRotation", "ar", 0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_enum_attr_fn.addField("Compute", 0));
	CHECK_MSTATUS(l_enum_attr_fn.addField("Keep", 1));
	CHECK_MSTATUS(l_enum_attr_fn.addField("Disable", 2));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(false));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	/* Division for quaternions sample*/
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_division_attr,	"division",			"div", kInt, 0);
	l_numeric_attr_fn.setMin(1);
	l_numeric_attr_fn.setDefault(10);
	/* Pin Start Quaternion */
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_offset_attr,	"rotationOffset",	"rof",	kDouble,	0.0);

	g_quaternion_compound_attr = l_compound_attr_fn.create("Rotation", "r", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_division_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_mode_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_offset_attr));
	l_status = addAttribute(g_quaternion_compound_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// transform matrix
	g_transfoms_attr = l_matr_attr_fn.create("xform", "xfr", MFnMatrixAttribute::kDouble,  &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	l_status = addAttribute(g_transfoms_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	g_quaternion_twist_attr = l_unit_attr_fn.create("twistMultiplier", "tm", MFnUnitAttribute::kAngle, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_unit_attr_fn.setChannelBox(true));
	l_status = addAttribute(g_quaternion_twist_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// in data
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// in fCurve
	g_quaternion_fcurve_attr = l_typed_attr_fn.create("fCurve", "fc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_quaternion_fcurve_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	g_data_store_attr = l_typed_attr_fn.create("hiddenQD", "hqd", mpxCurveQuatWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setConnectable(false);
	l_typed_attr_fn.setHidden(true);
	l_status = addAttribute(g_data_store_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_quaternion_compound_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_division_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_mode_attr,				g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_offset_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_data_in_attr,						g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transfoms_attr,					g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_twist_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_fcurve_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveQuatFrameNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		//------------------------------------------------------------------------------
		// handle out data
		std::shared_ptr<vufCurveData>		l_in_data;
		std::shared_ptr<vufCurveData>		l_out_data;
		std::shared_ptr<vufCurveData>		l_fcurve_data;
		std::shared_ptr<vufCurveQuatData>	l_store_data;

		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper,	vufCurveData,		p_data, g_data_out_attr,	l_out_data,		m_gen_id);
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveQuatWrapper,vufCurveQuatData,	p_data, g_data_store_attr,	l_store_data, m_gen_id);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_data_in_attr,			l_in_data);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_quaternion_fcurve_attr, l_fcurve_data);
		
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)
		{
			p_data.setClean(g_data_store_attr);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		if (l_store_data->m_internal_data == nullptr)
		{
			l_store_data->m_internal_data = vufCurveQuaternionFn_4d::create(vufCurveQuatFnType::k_transport);
		}
		std::shared_ptr<vufCurve< double, vufVector4> > l_fcurve_ptr = nullptr;
		if (l_fcurve_data != nullptr && l_fcurve_data->m_internal_data != nullptr)
		{
			l_fcurve_ptr = l_fcurve_data->m_internal_data->get_curve_ptr();
		}

		vufCurveContainer_4d& l_in_container = *(l_in_data->m_internal_data.get());
		vufCurveContainer_4d& l_out_container = *(l_out_data->m_internal_data.get());
		// copy pointers from in container to out except quat
		l_out_container.set_curve_ptr(l_in_container.get_curve_ptr());
		l_out_container.set_rebuild_fn_ptr(l_in_container.get_rebuild_fn_ptr());
		l_out_container.set_scale_fn_ptr(l_in_container.get_scale_fn_ptr());
		//l_out_container.set_remap_fn_ptr(l_in_container.get_remap_fn_ptr());

		auto l_quat_ptr = l_store_data->m_internal_data->as_transport_fn();
		auto l_crv_ptr = l_out_container.get_curve_ptr();
		l_out_container.set_quaternion_fn_ptr(l_store_data->m_internal_data);
#pragma region HANDLE_PARAMS
		MMatrix l_matr					= p_data.inputValue(g_transfoms_attr).asMatrix();
		int		l_division				= p_data.inputValue(g_quaternion_division_attr).asInt();
		short	l_quat_mode				= p_data.inputValue(g_quaternion_mode_attr).asShort();
		double	l_quat_offset_value		= p_data.inputValue(g_quaternion_offset_attr).asDouble();
		double	l_quat_twist_value		= p_data.inputValue(g_quaternion_twist_attr).asAngle().asRadians();
#pragma endregion
		if (l_quat_mode == 2 /*delete quaternion fn*/)
		{
			l_out_container.set_quaternion_fn_ptr(l_in_container.get_quaternion_fn_ptr());
			p_data.setClean(g_data_store_attr);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		//auto l_quat_ptr = l_store_data->m_internal_data->as_tranport_fn();
		l_quat_ptr->set_division_i(l_division, l_crv_ptr);
		l_quat_ptr->set_root_item_i(*( (vufMatrix4<double>*)(& l_matr) ));
		l_quat_ptr->set_fcurve(l_fcurve_ptr);
		l_quat_ptr->set_twist_multiplier(l_quat_twist_value);
		//l_quat_ptr->set_pin_start(l_quat_pin_start);
		//l_quat_ptr->set_pin_start_value(l_quat_pin_start_value);
		//l_quat_ptr->set_pin_end(l_quat_pin_end);
		//l_quat_ptr->set_pin_end_value(l_quat_pin_end_value);
		//l_quat_ptr->set_offset(l_quat_offset_value);

		//l_quat_ptr->set_item_count(l_division);
		//std::cout << l_matr << std::endl;
		//l_quat_ptr->set_item_at(0, *((vufMatrix4<double>*)(&l_matr)) );
		l_quat_ptr->compute_bind_params_i(l_crv_ptr);
		l_quat_ptr->match_quaternions_i(l_out_container);
		p_data.setClean(g_data_store_attr);
		p_data.setClean(g_data_out_attr);

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
