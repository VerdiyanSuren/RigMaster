#include <data/vufMayaDataList.h>
#include <curves/vufCurveClosestPointNode.h>
#include <vufMayaGlobalIncludes.h>
#include <math/vufVector.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>

using namespace vufRM;
using namespace vufMath;
MObject	vufCurveClosestPointNode::g_in_curve_attr;
MObject	vufCurveClosestPointNode::g_xform_in_attr;
MObject	vufCurveClosestPointNode::g_translate_attr;
MObject	vufCurveClosestPointNode::g_tangent_attr;
MObject	vufCurveClosestPointNode::g_param_attr;
MObject	vufCurveClosestPointNode::g_dist_attr;


void* vufCurveClosestPointNode::creator()
{
	return  new vufCurveClosestPointNode();
}
MStatus	vufCurveClosestPointNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnMatrixAttribute		l_matrix_attr_fn;

	// Input Data
	g_in_curve_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));


	g_xform_in_attr = l_matrix_attr_fn.create("inXform", "xf", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_matrix_attr_fn.setWritable(true);
	l_matrix_attr_fn.setStorable(true);
	l_matrix_attr_fn.setHidden(false);
	l_matrix_attr_fn.setKeyable(true);

	g_translate_attr = l_numeric_attr_fn.create("translate", "t", MFnNumericData::k3Double, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));

	g_tangent_attr = l_numeric_attr_fn.create("tangent", "tn", MFnNumericData::k3Double, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));
	
	g_param_attr = l_numeric_attr_fn.create("parameter", "prm", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));

	g_dist_attr = l_numeric_attr_fn.create("distance", "dst", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));


	l_status = addAttribute(g_in_curve_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_xform_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_translate_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_tangent_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_param_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_dist_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_in_curve_attr, g_translate_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_curve_attr, g_tangent_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_curve_attr, g_param_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_curve_attr, g_dist_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_xform_in_attr, g_translate_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_xform_in_attr, g_tangent_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_xform_in_attr, g_param_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_xform_in_attr, g_dist_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	

	return MS::kSuccess;
}
MStatus	vufCurveClosestPointNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_translate_attr	||
		p_plug == g_tangent_attr	||
		p_plug == g_param_attr		||
		p_plug == g_dist_attr)
	{
		MMatrix l_matr = p_data.inputValue(g_xform_in_attr).asMatrix();
		std::shared_ptr<vufCurveData> l_in_data;
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_in_curve_attr, l_in_data);
		if (l_in_data != nullptr && l_in_data->m_internal_data != nullptr)
		{		
			auto l_crv_ptr = l_in_data->m_internal_data->get_curve_ptr();
			vufVector_4d l_point(l_matr[3][0], l_matr[3][1], l_matr[3][2]);
			//l_crv_ptr->log_me();
			auto l_param	= l_crv_ptr->get_closest_point_param(l_point);
			auto l_pos		= l_crv_ptr->get_pos_at(l_param);
			auto l_tng		= l_crv_ptr->get_tangent_at(l_param);
			auto l_dist		= l_point.distance_to(l_pos);
			
			p_data.outputValue(g_translate_attr).set3Double(l_pos.x, l_pos.y, l_pos.z);
			p_data.outputValue(g_tangent_attr).set3Double(l_tng.x, l_tng.y, l_tng.z);
			p_data.outputValue(g_param_attr).setDouble(l_param);
			p_data.outputValue(g_tangent_attr).setDouble(l_dist);

			p_data.setClean(g_translate_attr);
			p_data.setClean(g_translate_attr);
			p_data.setClean(g_param_attr);
			p_data.setClean(g_tangent_attr);
		}
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}

