#include <curves/rotateFn/vufCurveQuatParamNode.h>
#include <curves/vufCurveContatiner.h>
#include <curves/quaternionFn/vufCurveQuaternionParamsFn.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MMatrixArray.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveQuatParamNode::g_doubles_attr;
MObject	vufCurveQuatParamNode::g_transfoms_attr;
MObject	vufCurveQuatParamNode::g_data_in_attr;
MObject	vufCurveQuatParamNode::g_data_out_attr;

vufCurveQuatParamNode::vufCurveQuatParamNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveQuatParamNode::creator()
{
	return new vufCurveQuatParamNode();
}
