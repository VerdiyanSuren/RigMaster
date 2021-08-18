#include <data/vufMayaDataList.h>
#include <curves/get/vufCurveClosestParamNode.h>
#include <vufMayaGlobalIncludes.h>
#include <math/vufVector.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>

using namespace vufRM;
using namespace vufMath;
MObject	 vufCurveClosestParamNode::g_in_curve_attr;
MObject	 vufCurveClosestParamNode::g_in_xform_in_attr;
MObject  vufCurveClosestParamNode::g_in_start_param;
MObject  vufCurveClosestParamNode::g_in_end_param;
MObject	 vufCurveClosestParamNode::g_in_division_param;

MObject	vufCurveClosestParamNode::g_out_param_attr;


void* vufCurveClosestParamNode::creator()
{
	return  new vufCurveClosestParamNode();
}
MStatus	vufCurveClosestParamNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnMatrixAttribute		l_matrix_attr_fn;

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
	// input divisions
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_in_division_param, "division", "div", kInt, 10);
	l_numeric_attr_fn.setChannelBox(true);
	l_numeric_attr_fn.setDefault(10);
	l_numeric_attr_fn.setMin(1);
	l_status = addAttribute(g_in_division_param);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// Input Data
	g_in_curve_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_in_curve_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	g_in_xform_in_attr = l_matrix_attr_fn.create("inXform", "xf", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_matrix_attr_fn.setWritable(true);
	l_matrix_attr_fn.setStorable(true);
	l_matrix_attr_fn.setHidden(false);
	l_matrix_attr_fn.setKeyable(true);
	l_status = addAttribute(g_in_xform_in_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	g_out_param_attr = l_numeric_attr_fn.create("parameter", "prm", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));
	l_status = addAttribute(g_out_param_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//Attributes affects
	l_status = attributeAffects(g_in_division_param, g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_start_param,	g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_end_param,		g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_curve_attr,	g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_xform_in_attr, g_out_param_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveClosestParamNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_out_param_attr)
	{
		std::string l_node_name = name().asChar(); 
		l_node_name += " Curve cLosest param node compute";
		vuf::vufTimer l_timer(l_node_name);


		std::shared_ptr<vufCurveData> l_in_data;
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_in_curve_attr, l_in_data);
		if (l_in_data != nullptr && l_in_data->m_internal_data != nullptr)
		{	
			double l_start	= p_data.inputValue(g_in_start_param).asDouble();
			double l_end	= p_data.inputValue(g_in_end_param).asDouble();
			MMatrix l_matr	= p_data.inputValue(g_in_xform_in_attr).asMatrix();
			int		l_div   = p_data.inputValue(g_in_division_param).asInt();
			uint32_t l_dv = l_div > 0 ? l_div : 0;
			vufCurveContainer_4d&	l_in_container = *(l_in_data->m_internal_data.get());
			vufVector_4d			l_point(l_matr[3][0], l_matr[3][1], l_matr[3][2]);
			double l_res = l_in_container.get_closest_param(l_point, l_start, l_end, l_dv);
			
			p_data.outputValue(g_out_param_attr).setDouble(l_res);
			p_data.setClean(g_out_param_attr);
		}
		p_data.setClean(g_out_param_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}

