#include <maya/MFnPlugin.h>
#include <iostream>

#include <vufLuaStatic.h>
#include <vufLogMaya.h>
// include list of all our datas
#include <data/vufMayaDataList.h>

#include <expressions/nodes/vufMayaLuaPortNode.h>
#include <expressions/nodes/vufMayaLuaTxtNode.h>
#include <expressions/nodes/vufMayaLuaExpressionNode.h>
#include <expressions/ui/vufExpressionWindowCmd.h>

#include <dataCollectors/vufMatrixListNode.h>
#include <dataCollectors/vufMatrixListNullNode.h>
#include <dataCollectors/vufDoubleListNode.h>
#include <dataCollectors/vufDoubleListNullNode.h>

#include <curves/locator/vufCurveLocator.h>
#include <curves/vufCurveFromMayaNode.h>
#include <curves/vufCurveFromMayaNode.h>
#include <curves/vufCurveToMayaNode.h>
#include <curves/vufCurveBSplineNode.h>
#include <curves/vufCurveBlendNode.h>
#include <curves/vufCurveQuatCloseNode.h>
#include <curves/vufCurveQuatFrameNode.h>
#include <curves/vufCurveNullNode.h>
#include <curves/vufCurveGetTransformNode.h>
#include <curves/vufCurveClosestPointNode.h>

#include <vufMayaGlobalIncludes.h>
#include <serializer/vufTxtSerializer.h>

#include <expressions/luaWrappers/luaUnitTest/vufLuaMayaMathUnitTest.h>

using namespace vufRM;

const MString g_draw_registrant_id("vufRM");
// Lua Data
const MTypeId	mpxMayaLuaPortWrapper::		g_id(0x297130);	const MString	mpxMayaLuaPortWrapper::		g_type_name("LuaPortData");
const MTypeId	mpxMayaLuaTxtWrapper::		g_id(0x297230);	const MString	mpxMayaLuaTxtWrapper::		g_type_name("LuaTxtData");
// Lua Nodes
const MTypeId	vufMayaLuaPortNode::		g_id(0x297330);	const MString	vufMayaLuaPortNode::		g_type_name("vufLuaPort");
const MTypeId	vufMayaLuaTxtNode::			g_id(0x197430);	const MString	vufMayaLuaTxtNode::			g_type_name("vufLuaTxt");
const MTypeId	vufMayaLuaExpressionNode::	g_id(0x197530);	const MString	vufMayaLuaExpressionNode::	g_type_name("vufLuaExpression");
//  Transform List
//const MTypeId	mpxTransformListWrapper::	g_id(0x297630);	const MString	mpxTransformListWrapper::	g_type_name("TransformListData");
const MTypeId	vufMatrixListNode::			g_id(0x297630);	const MString	vufMatrixListNode::			g_type_name("vufMatrixList");
const MTypeId	vufMatrixListNullNode::		g_id(0x297730);	const MString	vufMatrixListNullNode::		g_type_name("vufMatrixListNull");
const MTypeId	vufDoubleListNode::			g_id(0x297830);	const MString	vufDoubleListNode::			g_type_name("vufDoubleList");
const MTypeId	vufDoubleListNullNode::		g_id(0x297930);	const MString	vufDoubleListNullNode::		g_type_name("vufDoubleListNull");
// Curve Data
const MTypeId	mpxCurveWrapper::			g_id(0x298030);	const MString	mpxCurveWrapper::			g_type_name("CurveData");
const MTypeId	mpxCurveQuatWrapper::		g_id(0x298130);	const MString	mpxCurveQuatWrapper::		g_type_name("CurveRotData");
const MTypeId	mpxCurveScaleWrapper::		g_id(0x298230);	const MString	mpxCurveScaleWrapper::		g_type_name("CurveScaleData");
const MTypeId	mpxCurveRebuildWrapper::	g_id(0x298330);	const MString	mpxCurveRebuildWrapper::	g_type_name("CurveRebuildData");

// Curve locator
const MTypeId	vufCurveLocator::			g_id(0x298430);	const MString	vufCurveLocator::			g_type_name("vufCurveLocator"); const MString	vufCurveLocator::g_drawDb_classification("drawdb/geometry/vufRMCurveLocator");
// Curve Nodes
const MTypeId	vufFromMayaCurveNode::		g_id(0x298530);	const MString	vufFromMayaCurveNode::		g_type_name("vufCurveFromMaya");
const MTypeId	vufCurveToMayaNode::		g_id(0x298630);	const MString	vufCurveToMayaNode::		g_type_name("vufCurveToMaya");
const MTypeId	vufCurveBSplineNode::		g_id(0x298730);	const MString	vufCurveBSplineNode::		g_type_name("vufCurveBSpline");
const MTypeId	vufCurveBlendNode::			g_id(0x298830);	const MString	vufCurveBlendNode::			g_type_name("vufCurveBlend");
const MTypeId	vufCurveNullNode::			g_id(0x298930);	const MString	vufCurveNullNode::			g_type_name("vufCurveNull");
const MTypeId	vufCurveGetTransformNode::	g_id(0x299030);	const MString	vufCurveGetTransformNode::	g_type_name("vufCurveGetXForm");
const MTypeId	vufCurveClosestPointNode::	g_id(0x299130);	const MString	vufCurveClosestPointNode::	g_type_name("vufCurveClosestPoint");
const MTypeId	vufCurveQuatCloseNode::		g_id(0x299230);	const MString	vufCurveQuatCloseNode::		g_type_name("vufCurveRotationClosest");
const MTypeId	vufCurveQuatFrameNode::		g_id(0x299330);	const MString	vufCurveQuatFrameNode::		g_type_name("vufCurveRotationFrame");


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
	VF_REGISTER_DATA(mpxCurveWrapper);
	VF_REGISTER_DATA(mpxCurveQuatWrapper); 
	VF_REGISTER_DATA(mpxCurveScaleWrapper);
	VF_REGISTER_DATA(mpxCurveRebuildWrapper);
	// register nodes
	VF_REGISTER_NODE(vufMayaLuaPortNode);
	VF_REGISTER_NODE(vufMayaLuaTxtNode);
	VF_REGISTER_NODE(vufMayaLuaExpressionNode);

	VF_REGISTER_NODE(vufMatrixListNode);
	VF_REGISTER_NODE(vufMatrixListNullNode);
	VF_REGISTER_NODE(vufDoubleListNode);
	VF_REGISTER_NODE(vufDoubleListNullNode);
	

	VF_REGISTER_NODE(vufFromMayaCurveNode);
	VF_REGISTER_NODE(vufCurveToMayaNode);
	VF_REGISTER_NODE(vufCurveBSplineNode);
	VF_REGISTER_NODE(vufCurveBlendNode);
	VF_REGISTER_NODE(vufCurveNullNode);
	VF_REGISTER_NODE(vufCurveGetTransformNode);
	VF_REGISTER_NODE(vufCurveClosestPointNode);
	VF_REGISTER_NODE(vufCurveQuatCloseNode);
	VF_REGISTER_NODE(vufCurveQuatFrameNode);

	VF_RM_REGISTER_LOCATOR(vufCurveLocator, l_plugin, l_status);
	// register commands
	VF_REGISTER_COMMAND(vufExpressionWindowCmd);
	std::cout << "---------------------vufRigMaster  Loaded----------------" << std::endl;

	//vufLuaMayaMathUT l_ut;
	//l_ut.run();

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	std::cout << "------------vufRigMaster unloaded------------" << std::endl;
	MStatus   l_status;
	MFnPlugin l_plugin(obj);
	VF_RM_DEREGISTER_LOCATOR(vufCurveLocator, l_plugin, l_status);

	VF_DEREGISTER_NODE(vufMayaLuaPortNode);
	VF_DEREGISTER_NODE(vufMayaLuaTxtNode);
	VF_DEREGISTER_NODE(vufMayaLuaExpressionNode);

	VF_DEREGISTER_NODE(vufMatrixListNode);
	VF_DEREGISTER_NODE(vufMatrixListNullNode);
	VF_DEREGISTER_NODE(vufDoubleListNode);
	VF_DEREGISTER_NODE(vufDoubleListNullNode);

	VF_DEREGISTER_NODE(vufFromMayaCurveNode);
	VF_DEREGISTER_NODE(vufCurveToMayaNode);
	VF_DEREGISTER_NODE(vufCurveBSplineNode);
	VF_DEREGISTER_NODE(vufCurveBlendNode);
	VF_DEREGISTER_NODE(vufCurveNullNode);
	VF_DEREGISTER_NODE(vufCurveGetTransformNode);
	VF_DEREGISTER_NODE(vufCurveClosestPointNode);
	VF_DEREGISTER_NODE(vufCurveQuatCloseNode);
	VF_DEREGISTER_NODE(vufCurveQuatFrameNode);

	VF_DEREGISTER_DATA(mpxMayaLuaPortWrapper);
	VF_DEREGISTER_DATA(mpxMayaLuaTxtWrapper);
	VF_DEREGISTER_DATA(mpxCurveWrapper);
	VF_DEREGISTER_DATA(mpxCurveQuatWrapper);
	VF_DEREGISTER_DATA(mpxCurveScaleWrapper);
	VF_DEREGISTER_DATA(mpxCurveRebuildWrapper);

	// deregister command
	vufExpressionWindowCmd::release_wnd();
	VF_DEREGISTER_COMMAND(vufExpressionWindowCmd);

	vuf::txtSerializer::release();

	return MS::kSuccess;
}
