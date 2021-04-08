#include <vufLog.h>
#include <expressions/nodes/vufMayaLuaPortNode.h>
#include <expressions/data/vufMayaLuaPortInternalData.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>
#include <vufLuaWrapper.h>


#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>


using namespace vufRM;

MObject	vufMayaLuaPortNode::g_port_attr;
MObject	vufMayaLuaPortNode::g_demand_attr;


vufMayaLuaPortNode::vufMayaLuaPortNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufMayaLuaPortNode::creator()
{
	return new vufMayaLuaPortNode();
}
MStatus	vufMayaLuaPortNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;

	// Demand numeric attribute
	VF_TP_CREATE_STORABLE_NUMERIC_ATTR(g_demand_attr, "demand", "dmn", kDouble, 0.0);
	CHECK_MSTATUS(l_numeric_attr_fn.setHidden(true));

	// Expression attribute
	g_port_attr = l_typed_attr_fn.create("port", "prt", mpxMayaLuaPortWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	// Add Attributes
	l_status = addAttribute(g_demand_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_port_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attribute affects
	l_status = attributeAffects(g_demand_attr, g_port_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufMayaLuaPortNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_port_attr)
	{
		MStatus l_status;
		std::shared_ptr<vufMayaLuaPortData>	l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxMayaLuaPortWrapper, vufMayaLuaPortData, p_data, g_port_attr, l_out_data);
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
			l_out_data->m_internal_data = std::shared_ptr<vufMayaLuaPortInternalData>(new vufMayaLuaPortInternalData());
		}
		// Do Main Job Here
		
		/*
		VF_LOG_INFO(l_out_data->m_internal_data->m_lua_txt->get_script());
		*/

		p_data.setClean(g_port_attr);

		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}

