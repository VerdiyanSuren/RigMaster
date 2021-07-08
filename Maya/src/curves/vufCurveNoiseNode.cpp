#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MTime.h>
#include <maya/MGlobal.h>

#include <curves/vufCurveNoiseNode.h>
#include <noise/vufPelinNoise.h>
#include <vufMayaGlobalIncludes.h>
#include <data/vufMayaDataList.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveNoiseNode::g_enable_attr;
MObject	vufCurveNoiseNode::g_use_quat_attr;
MObject	vufCurveNoiseNode::g_use_scale_attr;
MObject	vufCurveNoiseNode::g_time_attr;
MObject	vufCurveNoiseNode::g_speed_attr;
MObject	vufCurveNoiseNode::g_amount_x_attr;
MObject	vufCurveNoiseNode::g_amount_y_attr;
MObject	vufCurveNoiseNode::g_amount_z_attr;
MObject	vufCurveNoiseNode::g_scale_x_attr;
MObject	vufCurveNoiseNode::g_scale_y_attr;
MObject	vufCurveNoiseNode::g_scale_z_attr;
MObject	vufCurveNoiseNode::g_offset_x_attr;
MObject	vufCurveNoiseNode::g_offset_y_attr;
MObject	vufCurveNoiseNode::g_offset_z_attr;

MObject	vufCurveNoiseNode::g_data_in_attr;
MObject	vufCurveNoiseNode::g_data_out_attr;

vufCurveNoiseNode::vufCurveNoiseNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveNoiseNode::creator()
{
	return new vufCurveNoiseNode();
}
MStatus	vufCurveNoiseNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;

	// 	enable
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_enable_attr, "enable", "en", kBoolean, true);
	l_status = addAttribute(g_enable_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// use frame rotation	
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_use_quat_attr, "useRotation", "ur", kBoolean, false);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_use_quat_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// use frame rotation	
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_use_scale_attr, "useScale", "us", kBoolean, false);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_use_scale_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// time
	g_time_attr = l_unit_attr_fn.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setStorable(true));
	l_status = addAttribute(g_time_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// speed
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_speed_attr, "speed", "spd", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_speed_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// amount x
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_amount_x_attr, "amountX", "ax", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_amount_x_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// amount y
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_amount_y_attr, "amountY", "ay", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_amount_y_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// amount x
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_amount_z_attr, "amountZ", "az", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_amount_z_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// scale x
	 VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_x_attr, "scaleX", "sx", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_scale_x_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// scale y
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_y_attr, "scaleY", "sy", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_scale_y_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// scale x
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_scale_z_attr, "scaleZ", "sz", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_scale_z_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// offset x
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_offset_x_attr, "offsetX", "ox", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_offset_x_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// offset y
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_offset_y_attr, "offsetY", "oy", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_offset_y_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// offset z
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_offset_z_attr, "offsetZ", "oz", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_offset_z_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

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
	l_status = attributeAffects(g_use_quat_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_use_scale_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_time_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_speed_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_amount_x_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_amount_y_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_amount_z_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_scale_x_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_scale_y_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_scale_z_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_offset_x_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_offset_y_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_offset_z_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}

MStatus	vufCurveNoiseNode::compute(const MPlug& p_plug, MDataBlock& p_data)
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

		vufCurveContainer_4d& l_in_container = *(l_in_data->m_internal_data.get());
		vufCurveContainer_4d& l_out_container = *(l_out_data->m_internal_data.get());
		//l_out_container.set_curve_ptr(l_in_container.get_curve_ptr());
		l_out_container.set_rebuild_fn_ptr(l_in_container.get_rebuild_fn_ptr());
		l_out_container.set_scale_fn_ptr(l_in_container.get_scale_fn_ptr());
		l_out_container.set_quaternion_fn_ptr(l_in_container.get_quaternion_fn_ptr());

		if (l_enable == false)
		{
			l_out_container.set_curve_ptr(l_in_container.get_curve_ptr());
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		l_out_container.switch_curve(0, vufMath::vufCurveType::k_noise_curve);
		auto l_crv		= l_out_container.get_curve_ptr()->as_curve_noise();
		//VF_LOG_INFO(l_crv->to_string());
		//MGlobal::displayWarning("Noise compute");
		//auto l_crv_in	= l_in_container.get_curve_ptr();
		l_crv->set_use_quat_i(p_data.inputValue(g_use_quat_attr).asBool());
		l_crv->set_use_scale_i(p_data.inputValue(g_use_scale_attr).asBool());
		l_crv->set_time_i(p_data.inputValue(g_time_attr).asTime().value());
		l_crv->set_speed_i(p_data.inputValue(g_speed_attr).asDouble());
		l_crv->set_amount_x_i(p_data.inputValue(g_amount_x_attr).asDouble());
		l_crv->set_amount_y_i(p_data.inputValue(g_amount_y_attr).asDouble());
		l_crv->set_amount_z_i(p_data.inputValue(g_amount_z_attr).asDouble());
		l_crv->set_scale_x_i(p_data.inputValue(g_scale_x_attr).asDouble());
		l_crv->set_scale_y_i(p_data.inputValue(g_scale_y_attr).asDouble());
		l_crv->set_scale_z_i(p_data.inputValue(g_scale_z_attr).asDouble());
		l_crv->set_offset_x_i(p_data.inputValue(g_offset_x_attr).asDouble());
		l_crv->set_offset_y_i(p_data.inputValue(g_offset_y_attr).asDouble());
		l_crv->set_offset_z_i(p_data.inputValue(g_offset_z_attr).asDouble());
		l_crv->set_container_i(l_in_data->m_internal_data);

		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}

