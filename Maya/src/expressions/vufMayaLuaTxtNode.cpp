#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <data/vufMayaDataList.h>
#include <expressions/vufMayaLuaTxtNode.h>
#include <vufMayaGlobalIncludes.h>

using namespace vufRM;

MObject	vufMayaLuaTxtNode::g_expression_attr;
MObject	vufMayaLuaTxtNode::g_demand_attr;


vufMayaLuaTxtNode::vufMayaLuaTxtNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufMayaLuaTxtNode::creator()
{
	return new vufMayaLuaTxtNode();
}
MStatus	vufMayaLuaTxtNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;

	// Demand numeric attribute
	VF_TP_CREATE_STORABLE_NUMERIC_ATTR(g_demand_attr, "demand", "dmn", kDouble, 0.0);
	CHECK_MSTATUS(l_numeric_attr_fn.setHidden(true));

	// Expression attribute
	g_expression_attr = l_typed_attr_fn.create("script", "scr", mpxMayaLuaTxtWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	// Add Attributes
	l_status = addAttribute(g_demand_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_expression_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attribute affects
	l_status = attributeAffects(g_demand_attr, g_expression_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufMayaLuaTxtNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	return MS::kSuccess;
}

