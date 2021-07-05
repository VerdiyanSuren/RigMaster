#include <curves/rotateFn/vufCurveQuatParamNode.h>
#include <curves/vufCurveContatiner.h>
#include <curves/quaternionFn/vufCurveQuaternionParamsFn.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MMatrixArray.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveQuatParamNode::g_enable_attr;
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
MStatus	vufCurveQuatParamNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	//MFnEnumAttribute		l_enum_attr_fn;
	MFnMatrixAttribute		l_matr_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;

	g_enable_attr = l_numeric_attr_fn.create("enable", "ar", MFnNumericData::kBoolean, false, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	l_status = addAttribute(g_enable_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);


	g_doubles_attr = l_typed_attr_fn.create("outDoubleList", "odl", MFnData::kDoubleArray, MObject::kNullObj, &l_status);
	l_numeric_attr_fn.setWritable(true);
	l_numeric_attr_fn.setStorable(true);
	l_numeric_attr_fn.setHidden(false);
	l_numeric_attr_fn.setKeyable(true);
	l_numeric_attr_fn.setArray(true);
	l_numeric_attr_fn.setUsesArrayDataBuilder(true);
	l_numeric_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);
	l_status = addAttribute(g_doubles_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	g_transfoms_attr = l_typed_attr_fn.create("xformList", "xfr", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	l_status = addAttribute(g_transfoms_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// in data
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_enable_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_doubles_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transfoms_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveQuatParamNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;

		bool	l_enable = p_data.inputValue(g_enable_attr).asBool();

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
