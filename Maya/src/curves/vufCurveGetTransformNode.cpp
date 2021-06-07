#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>

#include <curves/vufCurveGetTransformNode.h>
#include <math/vufMatrix.h>
#include <math/vufQuaternion.h>
#include <data/vufMayaDataList.h>


#include <maya/MMatrix.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveGetTransformNode::g_parent_xform_attr;
MObject	vufCurveGetTransformNode::g_axis_attr;
MObject	vufCurveGetTransformNode::g_in_curve_attr;
MObject	vufCurveGetTransformNode::g_param_attr;
MObject	vufCurveGetTransformNode::g_param_pos_attr;
MObject	vufCurveGetTransformNode::g_param_rot_attr;
MObject	vufCurveGetTransformNode::g_param_scl_attr;

MObject	vufCurveGetTransformNode::g_res_compound_attr;

MObject	vufCurveGetTransformNode::g_res_pos_attr;

MObject	vufCurveGetTransformNode::g_res_rot_attr;
MObject	vufCurveGetTransformNode::g_res_rot_x_attr;
MObject	vufCurveGetTransformNode::g_res_rot_y_attr;
MObject	vufCurveGetTransformNode::g_res_rot_z_attr;

MObject	vufCurveGetTransformNode::g_res_scl_attr;

MObject	vufCurveGetTransformNode::g_res_xform_attr;
MObject	vufCurveGetTransformNode::g_res_vufTransform;


void* vufCurveGetTransformNode::creator()
{
	return new vufCurveGetTransformNode();
}
MStatus	vufCurveGetTransformNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	MFnMatrixAttribute		l_matrix_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;

	// Parent xform
	g_parent_xform_attr = l_matrix_attr_fn.create("parentInverse", "pi", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_matrix_attr_fn.setWritable(true);
	l_matrix_attr_fn.setStorable(true);
	l_matrix_attr_fn.setHidden(false);
	l_matrix_attr_fn.setKeyable(true);

	// In curve data
	g_in_curve_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));

	// Axis attr 
	g_axis_attr = l_enum_attr_fn.create("axis", "ax", 3, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_enum_attr_fn.addField("x", k_x));
	CHECK_MSTATUS(l_enum_attr_fn.addField("y", k_y));
	CHECK_MSTATUS(l_enum_attr_fn.addField("z", k_z));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_enum_attr_fn.setChannelBox(true));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(k_x));

	// Position Param 
	g_param_attr = l_numeric_attr_fn.create("offset", "ofs", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(0.0));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setChannelBox(true));
	
	// Position Param 
	g_param_pos_attr = l_numeric_attr_fn.create("posParam", "pp", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(0.0));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setChannelBox(true));

	// Rotation Param
	g_param_rot_attr = l_numeric_attr_fn.create("rotParam", "rp", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(0.0));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setChannelBox(true));

	// Scale Param
	g_param_scl_attr = l_numeric_attr_fn.create("sclParam", "sp", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(0.0));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setChannelBox(true));

	// Result Positon
	g_res_pos_attr = l_numeric_attr_fn.create("translate", "t", MFnNumericData::k3Double,0.0,&l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));

	// Result Rotation X
	g_res_rot_x_attr = l_unit_attr_fn.create("rotateX", "rx", MFnUnitAttribute::kAngle,0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setWritable(false));
	
	// Result Rotation Y
	g_res_rot_y_attr = l_unit_attr_fn.create("rotateY", "ry", MFnUnitAttribute::kAngle,0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setWritable(false));

	// Result Rotation Z
	g_res_rot_z_attr = l_unit_attr_fn.create("rotateZ", "rz", MFnUnitAttribute::kAngle, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setWritable(false));

	// Rotation compound
	g_res_rot_attr = l_compound_attr_fn.create("rotation", "r", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_res_rot_x_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_res_rot_y_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_res_rot_z_attr));
	CHECK_MSTATUS(l_compound_attr_fn.setWritable(false));

	// result scale
	g_res_scl_attr = l_numeric_attr_fn.create("scale", "s", MFnNumericData::k3Double, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(false));

	g_res_xform_attr = l_matrix_attr_fn.create("xForm", "xfr", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_matrix_attr_fn.setWritable(false));

	g_res_compound_attr = l_compound_attr_fn.create("result", "rs", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_res_pos_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_res_rot_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_res_scl_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_res_xform_attr));
	CHECK_MSTATUS(l_compound_attr_fn.setWritable(false));

	//------------------------------------------------------------------
	// Add Attributes
	l_status = addAttribute(g_parent_xform_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_in_curve_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_axis_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_param_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_param_pos_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_param_rot_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_param_scl_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//------------------------------------------------------------------
	// Attribute Affect
	l_status = attributeAffects(g_parent_xform_attr,	g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_curve_attr,		g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_axis_attr,			g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_attr,			g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_pos_attr,		g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_rot_attr,		g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_scl_attr,		g_res_compound_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	/*
	l_status = attributeAffects(g_parent_xform_attr,	g_res_pos_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_in_curve_attr,		g_res_pos_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_axis_attr,			g_res_pos_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_attr,			g_res_pos_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_pos_attr,		g_res_pos_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_rot_attr,		g_res_pos_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_param_scl_attr,		g_res_pos_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	*/

	return MS::kSuccess;
}
MStatus	vufCurveGetTransformNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (	p_plug == g_res_compound_attr	||
			p_plug == g_res_pos_attr		|| 
			p_plug == g_res_rot_attr		||
			p_plug == g_res_rot_x_attr		|| 
			p_plug == g_res_rot_y_attr		|| 
			p_plug == g_res_rot_z_attr		|| 
			p_plug == g_res_scl_attr		||
			p_plug == g_res_xform_attr)
	{
		//VF_LOG_INFO("COMPUTE TRANSFORM");
		MMatrix l_parent_matr= p_data.inputValue(g_parent_xform_attr).asMatrix();
		double l_offset		= p_data.inputValue(g_param_attr).asDouble();
		double l_pos_param	= p_data.inputValue(g_param_pos_attr).asDouble();
		double l_rot_param	= p_data.inputValue(g_param_rot_attr).asDouble();
		double l_scl_param	= p_data.inputValue(g_param_scl_attr).asDouble();

		std::shared_ptr<vufCurveData> l_in_data;
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_in_curve_attr, l_in_data);
		if (l_in_data != nullptr && l_in_data->m_internal_data != nullptr)
		{
			//std::cout << "Compute: " << l_offset + l_pos_param <<std::endl;
			auto l_pos = l_in_data->m_internal_data->get_pos_at(l_offset + l_pos_param);
			auto l_qtr = l_in_data->m_internal_data->get_quaternion_at(l_offset + l_rot_param);
			auto l_scl = l_in_data->m_internal_data->get_scale_at(l_offset + l_scl_param);
			//std::cout << "q: " << l_qtr << std::endl;
			vufMatrix_4d l_matr;
			l_matr.set_quaternion(l_qtr);
			l_matr.set_translation(l_pos);
			MMatrix* l_maya_matr = (MMatrix*)&l_matr;
			//auto l_euler = l_matr.get_euler_xyz();
			l_qtr.conjugate_in_place();
			MQuaternion l_mq(l_qtr.x, l_qtr.y, l_qtr.z, l_qtr.w);
			MEulerRotation l_euler = l_mq.asEulerRotation();


			p_data.outputValue(g_res_pos_attr).set3Double(l_pos.x, l_pos.y, l_pos.z);
			p_data.outputValue(g_res_rot_x_attr).setDouble(l_euler.x);
			p_data.outputValue(g_res_rot_y_attr).setDouble(l_euler.y);
			p_data.outputValue(g_res_rot_z_attr).setDouble(l_euler.z);
			p_data.outputValue(g_res_scl_attr).set3Double(l_scl.x, l_scl.y, l_scl.z);
			p_data.outputValue(g_res_xform_attr).setMMatrix(*l_maya_matr);

			
			p_data.setClean(g_res_pos_attr);
			p_data.setClean(g_res_rot_x_attr);
			p_data.setClean(g_res_rot_y_attr);
			p_data.setClean(g_res_rot_z_attr);
			p_data.setClean(g_res_xform_attr);
			p_data.setClean(g_res_compound_attr);
		}
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}