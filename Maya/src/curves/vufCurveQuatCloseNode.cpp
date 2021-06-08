#include <curves/vufCurveQuatCloseNode.h>
#include <curves/vufCurveContatiner.h>
#include <curves/quaternionFn/vufCurveQuaternionCloseFn.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MMatrixArray.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveQuatCloseNode::g_quaternion_compound_attr;
MObject	vufCurveQuatCloseNode::g_quaternion_division_attr;
MObject	vufCurveQuatCloseNode::g_quaternion_mode_attr;
MObject	vufCurveQuatCloseNode::g_quaternion_pin_start_attr;
MObject	vufCurveQuatCloseNode::g_quaternion_pin_start_value_attr;
MObject	vufCurveQuatCloseNode::g_quaternion_pin_end_attr;
MObject	vufCurveQuatCloseNode::g_quaternion_pin_end_value_attr;
MObject	vufCurveQuatCloseNode::g_quaternion_offset_attr;

MObject	vufCurveQuatCloseNode::g_transfoms_attr;
MObject	vufCurveQuatCloseNode::g_data_in_attr;
MObject	vufCurveQuatCloseNode::g_data_out_attr;
MObject	vufCurveQuatCloseNode::g_data_store_attr;

vufCurveQuatCloseNode::vufCurveQuatCloseNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveQuatCloseNode::creator()
{
	return new vufCurveQuatCloseNode();
}
MStatus	vufCurveQuatCloseNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;

	g_quaternion_mode_attr = l_enum_attr_fn.create("applyRotation", "ar", 0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_enum_attr_fn.addField("Compute", 0));
	CHECK_MSTATUS(l_enum_attr_fn.addField("Keep", 1));
	CHECK_MSTATUS(l_enum_attr_fn.addField("Disable", 2));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(false));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	/* Division for closest point*/
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_division_attr, "division", "div", kInt, 0.0);
	/* Pin Start Quaternion */
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_pin_start_attr,			"rotationPinStart",		"rsp", kBoolean, false);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_pin_start_value_attr,	"rotaionPinStartValue", "rsv", kDouble, 0.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_pin_end_attr,			"rotationPinEnd",		"rnp", kBoolean, false);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_pin_end_value_attr,		"rotationPinEndValue",	"rnv", kDouble, 1.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_quaternion_offset_attr,			"rotationOffset",		"rof", kDouble, 0.0);

	g_quaternion_compound_attr = l_compound_attr_fn.create("Rotation", "r", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_division_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_mode_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_pin_start_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_pin_start_value_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_pin_end_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_pin_end_value_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_quaternion_offset_attr));
	l_status = addAttribute(g_quaternion_compound_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);


	g_transfoms_attr = l_typed_attr_fn.create("xformList", "xfr", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	l_status = addAttribute(g_transfoms_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// in data
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	g_data_store_attr = l_typed_attr_fn.create("hiddenCD", "hcd", mpxCurveQuatWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setConnectable(false);
	l_typed_attr_fn.setHidden(true);
	l_status = addAttribute(g_data_store_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_quaternion_compound_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_division_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_mode_attr,				g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_pin_start_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_pin_start_value_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_pin_end_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_pin_end_value_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_quaternion_offset_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_data_in_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transfoms_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveQuatCloseNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		//------------------------------------------------------------------------------
		// handle out data
		std::shared_ptr<vufCurveData>		l_in_data;
		std::shared_ptr<vufCurveData>		l_out_data;
		std::shared_ptr<vufCurveQuatData>	l_store_data;

		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper,		vufCurveData,		p_data, g_data_out_attr, l_out_data);
		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveQuatWrapper, vufCurveQuatData,	p_data, g_data_store_attr, l_store_data);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_data_in_attr, l_in_data);
		//------------------------------------------------------------------------------
		// reference check
		auto l_data_owner_id = l_out_data->get_owner_id();
		if (l_out_data->get_owner_id() != m_gen_id)
		{
			//To Do Copy data and make mine
		}
		l_out_data->set_owner_id(m_gen_id);
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
			l_store_data->m_internal_data = vufCurveQuaternionFn_4d::create(vufCurveQuatFnType::k_closest);
		}
		vufCurveContainer_4d& l_in_container = *(l_in_data->m_internal_data.get());
		vufCurveContainer_4d& l_out_container = *(l_out_data->m_internal_data.get());
		// copy pointers from in container to out except quat
		l_out_container.set_curve_ptr(l_in_container.get_curve_ptr());
		l_out_container.set_rebuild_fn_ptr(l_in_container.get_rebuild_fn_ptr());
		l_out_container.set_scale_fn_ptr(l_in_container.get_scale_fn_ptr());
		//l_out_container.set_remap_fn_ptr(l_in_container.get_remap_fn_ptr());

		l_out_container.set_quaternion_fn_ptr(l_store_data->m_internal_data);
#pragma region HANDLE_INPUT_TRANSFORM_LIST
		//------------------------------------------------------------------------------
		// Handle input transform list data
		MDataHandle l_input_data = p_data.inputValue(g_transfoms_attr);
		MObject		l_in_obj = l_input_data.data();
		MFnMatrixArrayData l_in_data_fn(l_in_obj, &l_status);
		MMatrixArray l_matrix_array = l_in_data_fn.array(&l_status);

		uint32_t l_transforms_sz = (uint32_t)l_matrix_array.length();
#pragma endregion
#pragma region HANDLE_PARAMS
		int		l_division				= p_data.inputValue(g_quaternion_division_attr).asInt();
		short	l_quat_mode				= p_data.inputValue(g_quaternion_mode_attr).asShort();
		bool	l_quat_pin_start		= p_data.inputValue(g_quaternion_pin_start_attr).asBool();
		double	l_quat_pin_start_value	= p_data.inputValue(g_quaternion_pin_start_value_attr).asDouble();
		bool	l_quat_pin_end			= p_data.inputValue(g_quaternion_pin_end_attr).asBool();
		double	l_quat_pin_end_value	= p_data.inputValue(g_quaternion_pin_end_value_attr).asBool();
		double	l_quat_offset_value		= p_data.inputValue(g_quaternion_offset_attr).asBool();
#pragma endregion
		if (l_quat_mode == 2 /*delete quaternion fn*/)
		{
			l_out_container.set_quaternion_fn_ptr(l_in_container.get_quaternion_fn_ptr());
			p_data.setClean(g_data_store_attr);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		auto l_quat_ptr = l_store_data->m_internal_data->as_closest_fn();
		
		l_quat_ptr->set_pin_start(l_quat_pin_start);
		l_quat_ptr->set_pin_start_value(l_quat_pin_start_value);
		l_quat_ptr->set_pin_end(l_quat_pin_end);
		l_quat_ptr->set_pin_end_value(l_quat_pin_end_value);
		l_quat_ptr->set_offset(l_quat_offset_value);

		l_quat_ptr->set_item_count(l_transforms_sz);
		for (uint32_t i = 0; i < l_transforms_sz; ++i)
		{
			vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
			l_quat_ptr->set_item_at(i, *l_matr);
		}
		if (l_quat_mode == 0 /* apply. always refresh*/)
		{
			l_quat_ptr->compute_bind_params(l_out_container, l_division);
		}
		l_quat_ptr->match_quaternions(l_out_container);

		p_data.setClean(g_data_store_attr);
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;	
	}
	return MS::kUnknownParameter;
}
