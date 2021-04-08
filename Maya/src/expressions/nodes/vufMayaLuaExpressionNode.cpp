#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>

#include <expressions/nodes/vufMayaLuaExpressionNode.h>
#include <data/vufMayaDataList.h>
#include <vufLuaWrapper.h>

using namespace vufRM;


MObject	vufMayaLuaExpressionNode::g_script_attr;
MObject	vufMayaLuaExpressionNode::g_ports_script_attr;

MObject	vufMayaLuaExpressionNode::g_input_ports_attr;
MObject	vufMayaLuaExpressionNode::g_input_time_attr;
MObject	vufMayaLuaExpressionNode::g_input_number_attr;
MObject	vufMayaLuaExpressionNode::g_input_angle_attr;
MObject	vufMayaLuaExpressionNode::g_input_matrix_attr;
MObject	vufMayaLuaExpressionNode::g_input_mesh_attr;
MObject	vufMayaLuaExpressionNode::g_input_curve_attr;
MObject	vufMayaLuaExpressionNode::g_input_surface_attr;

MObject	vufMayaLuaExpressionNode::g_output_ports_attr;
MObject	vufMayaLuaExpressionNode::g_output_number_attr;
MObject	vufMayaLuaExpressionNode::g_output_angle_attr;
MObject	vufMayaLuaExpressionNode::g_output_matrix_attr;

vufMayaLuaExpressionNode::vufMayaLuaExpressionNode()
{
	//m_lua_machine.open_machine();
}
vufMayaLuaExpressionNode::~vufMayaLuaExpressionNode()
{
	//m_lua_machine.close_machine();
}
void* vufMayaLuaExpressionNode::creator()
{
	return new vufMayaLuaExpressionNode();
}
MStatus	vufMayaLuaExpressionNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;

	// Ports definition expression
	g_ports_script_attr = l_typed_attr_fn.create("inPortScript", "ips", mpxMayaLuaPortWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));

	//Script 
	g_script_attr = l_typed_attr_fn.create("script","scr", mpxMayaLuaTxtWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
#pragma region INPUT_PORTS
	//Time
	g_input_time_attr = l_unit_attr_fn.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setWritable(true));
	// Numbers
	g_input_number_attr = l_numeric_attr_fn.create("inNumber", "inm", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setArray(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Angles
	g_input_angle_attr = l_unit_attr_fn.create("angle", "ngl", MFnUnitAttribute::kAngle, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_unit_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_unit_attr_fn.setArray(true));
	CHECK_MSTATUS(l_unit_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS(l_unit_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// Matrix

	// Mesh

	// Curve

	// Surface

	// Compound input
	g_input_ports_attr = l_compound_attr_fn.create("inputPorts", "ip", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_compound_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_time_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_number_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_input_angle_attr));
	CHECK_MSTATUS(l_compound_attr_fn.setKeyable(true));

#pragma endregion
#pragma region OUTPUT_PORTS
	g_output_number_attr = l_numeric_attr_fn.create("outNumber", "onm", MFnNumericData::kDouble, 0.0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setArray(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setUsesArrayDataBuilder(true));

	g_output_ports_attr = l_compound_attr_fn.create("outputPorts", "op", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.setWritable(false));
	CHECK_MSTATUS(l_compound_attr_fn.setReadable(true));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_output_number_attr));


#pragma endregion
	// Add attributes
	l_status = addAttribute(g_ports_script_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_script_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_input_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attributes affect
	l_status = attributeAffects(g_ports_script_attr,	g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_script_attr,			g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_input_ports_attr,		g_output_ports_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);


	return MS::kSuccess;
}
MStatus	vufMayaLuaExpressionNode::compute(const MPlug& plug, MDataBlock& data)
{
	return MS::kSuccess;
}
