#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <maya/MMatrix.h>
#include <data/vufMayaDataList.h>
#include <expressions/nodes/vufMayaLuaTxtNode.h>
#include <vufMayaGlobalIncludes.h>
#include <vufLuaWrapper.h>
#include <vufLog.h>

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
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_demand_attr, "demand", "dmn", kDouble, 0.0);
	CHECK_MSTATUS(l_numeric_attr_fn.setHidden(true));

	// Expression attribute
	g_expression_attr = l_typed_attr_fn.create("script", "scr", mpxMayaLuaTxtWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(true);

	// Add Attributes
	l_status = addAttribute(g_demand_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_expression_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attribute affects
	l_status = attributeAffects(g_demand_attr, g_expression_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufMayaLuaTxtNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_expression_attr)
	{
		MStatus l_status;
		std::shared_ptr<vufMayaLuaTxtData>	l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxMayaLuaTxtWrapper, vufMayaLuaTxtData, p_data, g_expression_attr, l_out_data);
		// reference check
		auto l_data_owner_id = l_out_data->get_owner_id();
		if (l_out_data->get_owner_id() != m_gen_id)
		{
			//To Do Copy data and make mine
		}
		l_out_data->set_owner_id(m_gen_id);
		if (l_out_data->m_internal_data == nullptr)
		{
			VF_LOG_INFO("CONSTRUCTOR");
			l_out_data->m_internal_data = std::shared_ptr<vuf::vufTxt>(new vuf::vufTxt());
		}
		VF_LOG_INFO(l_out_data->m_internal_data->get_script());


		p_data.setClean(g_expression_attr);

		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}

