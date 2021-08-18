#include <data/vufMayaDataList.h>
#include <curves/get/vufCurveComponentParam.h>
#include <vufMayaGlobalIncludes.h>
#include <math/vufVector.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

using namespace vufRM;
using namespace vufMath;

MObject	 vufCurveComponentParamNode::g_in_curve_attr;
MObject	 vufCurveComponentParamNode::g_in_value_attr;
MObject	 vufCurveComponentParamNode::g_in_axis_attr;
MObject  vufCurveComponentParamNode::g_in_start_param;
MObject  vufCurveComponentParamNode::g_in_end_param;
MObject	 vufCurveComponentParamNode::g_in_division_param;

MObject	vufCurveComponentParamNode::g_out_param_attr;

void* vufCurveComponentParamNode::creator()
{
	return  new vufCurveComponentParamNode();
}
MStatus	vufCurveComponentParamNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;

	// In Axis
	g_in_axis_attr = l_enum_attr_fn.create("axis", "ax", 3, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_enum_attr_fn.addField("x", 0));
	CHECK_MSTATUS(l_enum_attr_fn.addField("y", 1));
	CHECK_MSTATUS(l_enum_attr_fn.addField("z", 2));
	//CHECK_MSTATUS(l_enum_attr_fn.addField("4",	k_tetra));
	//CHECK_MSTATUS(l_enum_attr_fn.addField("5",	k_penta));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(0));
	l_status = addAttribute(g_in_axis_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// In Value
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_value_attr, "value", "v", kDouble, 0.);
	l_numeric_attr_fn.setChannelBox(true);
	l_numeric_attr_fn.setKeyable(true);
	l_status = addAttribute(g_in_value_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// input start
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_start_param, "start", "str", kDouble, 0.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_numeric_attr_fn.setDefault(0.);
	l_status = addAttribute(g_in_start_param);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// input end
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_end_param, "end", "end", kDouble, 1.0);
	l_numeric_attr_fn.setChannelBox(true);
	l_numeric_attr_fn.setDefault(1.);
	l_status = addAttribute(g_in_end_param);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// In Divisions
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_division_param, "division", "div", kInt, 10);
	l_numeric_attr_fn.setChannelBox(true);
	l_numeric_attr_fn.setDefault(10);
	l_numeric_attr_fn.setMin(1);
	l_status = addAttribute(g_in_division_param);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// In Curve
	g_in_curve_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_in_curve_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// Out Param
	g_out_param_attr = l_numeric_attr_fn.create("parameter", "prm", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));
	l_status = addAttribute(g_out_param_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//Attributes affects
	l_status = attributeAffects(g_in_axis_attr,		g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_value_attr,	g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_start_param,	g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_end_param,		g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_division_param,g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_curve_attr,	g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveComponentParamNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_out_param_attr)
	{
		std::string l_node_name = name().asChar();
		l_node_name += " Curve component param node compute";
		vuf::vufTimer l_timer(l_node_name);


		std::shared_ptr<vufCurveData> l_in_data;
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_in_curve_attr, l_in_data);
		if (l_in_data != nullptr && l_in_data->m_internal_data != nullptr)
		{
			short  l_axis	= p_data.inputValue(g_in_axis_attr).asShort();
			double l_value	= p_data.inputValue(g_in_value_attr).asDouble();
			double l_start	= p_data.inputValue(g_in_start_param).asDouble();
			double l_end	= p_data.inputValue(g_in_end_param).asDouble();
			int	   l_div	= p_data.inputValue(g_in_division_param).asInt();
			uint32_t l_dv	= l_div > 0 ? l_div : 0;
			vufCurveContainer_4d& l_in_container = *(l_in_data->m_internal_data.get());
			double l_res = l_in_container.get_param_by_vector_component(l_value, l_axis, l_start, l_end, l_dv);

			p_data.outputValue(g_out_param_attr).setDouble(l_res);
			p_data.setClean(g_out_param_attr);
		}
		p_data.setClean(g_out_param_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}

using namespace vufRM;
using namespace vufMath;
