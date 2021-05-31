#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>

#include "quatCurves/vufToMayaCurveNode.h"
#include "quatCurves/vufCurveData.h"
#include "vufGlobalIncludes.h"


using namespace vufTP;

//attributes
MObject	vufToMayaCurveNode::g_lock_attr;
MObject	vufToMayaCurveNode::g_data_in_attr;
MObject	vufToMayaCurveNode::g_maya_curve_attr;

void* vufToMayaCurveNode::creator()
{
	return new vufToMayaCurveNode();
}
MStatus	vufToMayaCurveNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;

	VF_TP_CREATE_AND_ADD_LOCK_ATTR();

	//in quat curve data
	g_data_in_attr = l_typed_attr_fn.create("inCurveData", "icd", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);


	// Maya Curve Attribute
	g_maya_curve_attr = l_typed_attr_fn.create("mayaCurve", "mc", MFnData::kNurbsCurve, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	l_status = addAttribute(g_data_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_maya_curve_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_lock_attr,	g_maya_curve_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr, g_maya_curve_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);


	return MS::kSuccess;
}
MStatus vufToMayaCurveNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_maya_curve_attr)
	{
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}

