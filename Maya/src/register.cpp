#include <maya/MFnPlugin.h>
#include <iostream>

#include <vufLogMaya.h>
// include list of all our datas
#include <data/vufMayaDataList.h>

#include <expressions/nodes/vufMayaLuaPortNode.h>
#include <expressions/nodes/vufMayaLuaTxtNode.h>
#include <expressions/nodes/vufMayaLuaExpressionNode.h>
#include <expressions/ui/vufExpressionWindowCmd.h>

#include <vufMayaGlobalIncludes.h>
#include <vufTxtSerializer.h>
using namespace vufRM;
// Lua Data
const MTypeId	mpxMayaLuaPortWrapper::		g_id(0x197130);	const MString	mpxMayaLuaPortWrapper::g_type_name("LuaPortData");
const MTypeId	mpxMayaLuaTxtWrapper::		g_id(0x197230);	const MString	mpxMayaLuaTxtWrapper::g_type_name("LuaTxtData");
// Lua Nodes
const MTypeId	vufMayaLuaPortNode::		g_id(0x197330);	const MString	vufMayaLuaPortNode::g_type_name("vufLuaPort");
const MTypeId	vufMayaLuaTxtNode::			g_id(0x197430);	const MString	vufMayaLuaTxtNode::g_type_name("vufLuaTxt");
const MTypeId	vufMayaLuaExpressionNode::	g_id(0x197530);	const MString	vufMayaLuaExpressionNode::g_type_name("vufLuaExpression");


VF_TXT_WRITER_DEFINE_STATIC_VARS(); //Define txt serializer variables  
VF_DEFINE_CUSTOM_LOGGER(vufLogMaya);
//std::shared_ptr<vuf::vufLog> vuf::vufLog::g_log = nullptr;

MStatus initializePlugin(MObject obj)
{
	MStatus   l_status;
	std::cout << "---vufRigMaster  Suren Verdiyan 2021 vufer3d@gmail.com---" << std::endl;
	MFnPlugin l_plugin(obj, "Suren Verdiyan 2021 vufer3d@gmail.com", "0.1", "Any");

	// init libs
	vuf::txtSerializer::init();
	// register data
	VF_REGISTER_DATA(mpxMayaLuaPortWrapper);
	VF_REGISTER_DATA(mpxMayaLuaTxtWrapper);
	// register nodes
	VF_REGISTER_NODE(vufMayaLuaPortNode);
	VF_REGISTER_NODE(vufMayaLuaTxtNode);
	VF_REGISTER_NODE(vufMayaLuaExpressionNode);

	// register commands
	VF_REGISTER_COMMAND(vufExpressionWindowCmd);
	std::cout << "---------------------vufRigMaster  Loaded----------------" << std::endl;
	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	std::cout << "------------vufRigMaster unloaded------------" << std::endl;
	MStatus   l_status;
	MFnPlugin l_plugin(obj);

	VF_DEREGISTER_NODE(vufMayaLuaPortNode);
	VF_DEREGISTER_NODE(vufMayaLuaTxtNode);
	VF_DEREGISTER_NODE(vufMayaLuaExpressionNode);


	VF_DEREGISTER_DATA(mpxMayaLuaPortWrapper);
	VF_DEREGISTER_DATA(mpxMayaLuaTxtWrapper);

	// deregister command
	vufExpressionWindowCmd::release_wnd();
	VF_DEREGISTER_COMMAND(vufExpressionWindowCmd);

	vuf::txtSerializer::release();

	return MS::kSuccess;
}
