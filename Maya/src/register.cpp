#include <maya/MFnPlugin.h>
#include <iostream>

#include <vufLogMaya.h>
// include list of all our datas
#include <data/vufMayaDataList.h>

#include <expressions/vufMayaLuaTxtNode.h>
#include <expressions/vufMayaLuaExpressionNode.h>
#include <expressions/ui/vufExpressionWindowCmd.h>

#include <vufMayaGlobalIncludes.h>
#include <vufTxtSerializer.h>
using namespace vufRM;

const MTypeId	mpxMayaLuaTxtWrapper::		g_id(0x197130);	const MString	mpxMayaLuaTxtWrapper::g_type_name("LuaTxt");
const MTypeId	vufMayaLuaTxtNode::			g_id(0x197230);	const MString	vufMayaLuaTxtNode::g_type_name("vufLuaTxt");
const MTypeId	vufMayaLuaExpressionNode::	g_id(0x197330);	const MString	vufMayaLuaExpressionNode::g_type_name("vufLuaExpression");


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
	VF_REGISTER_DATA(mpxMayaLuaTxtWrapper);
	// register nodes
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

	VF_DEREGISTER_NODE(vufMayaLuaTxtNode);
	VF_DEREGISTER_NODE(vufMayaLuaExpressionNode);


	VF_DEREGISTER_DATA(mpxMayaLuaTxtWrapper);

	// deregister command
	vufExpressionWindowCmd::release_wnd();
	VF_DEREGISTER_COMMAND(vufExpressionWindowCmd);

	vuf::txtSerializer::release();

	return MS::kSuccess;
}
