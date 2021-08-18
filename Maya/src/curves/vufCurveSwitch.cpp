#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MGlobal.h>

#include <curves/vufCurveSwitch.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>
#include <dataCollectors/vufDataCollectorInclude.h>
#include <curves/vufMayaCurvesInclude.h>
//#include <curves/vufCurveContatiner.h>
using namespace vufRM;
using namespace vufMath;

MObject	vufCurveSwitch::g_choose_attr;
MObject	vufCurveSwitch::g_data_in_attr;
MObject	vufCurveSwitch::g_data_out_attr;

vufCurveSwitch::vufCurveSwitch() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveSwitch::creator()
{
	return new vufCurveSwitch();
}
MStatus	vufCurveSwitch::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;

	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_choose_attr, "index", "ndx", kInt, 0);
	l_numeric_attr_fn.setMin(0);
	l_numeric_attr_fn.setChannelBox(true);
	l_status = addAttribute(g_choose_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// in curve
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_typed_attr_fn.setArray(true);
	l_typed_attr_fn.setUsesArrayDataBuilder(true);
	l_status = addAttribute(g_data_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attributes affets
	l_status = attributeAffects(g_choose_attr, g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr, g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveSwitch::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		vuf::vufTimer l_timer("Curve Switch node compute");
		MStatus l_status;
		//------------------------------------------------------------------------------
		// switch index
		int l_index = p_data.inputValue(g_choose_attr).asInt();
		
		// handle out data		
		std::shared_ptr<vufCurveData>	l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		vufCurveContainer_4d& l_container = *(l_out_data->m_internal_data.get());
		
		// handle in data
		std::shared_ptr<vufCurveData>		l_in_data;
		MArrayDataHandle l_ah = p_data.inputValue(g_data_in_attr);
		l_status = l_ah.jumpToElement(l_index);
		if (l_status == MS::kSuccess)
		{
			mpxCurveWrapper* l_wrapp_ptr = (mpxCurveWrapper*)l_ah.inputValue().asPluginData();
			l_in_data = (l_wrapp_ptr == nullptr ? nullptr : l_wrapp_ptr->get_data());
		}
		if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)
		{
			MGlobal::displayError(name() + " inCurve is Null");
			l_container.set_curve_ptr(			nullptr);
			l_container.set_quaternion_fn_ptr(	nullptr);
			l_container.set_scale_fn_ptr(		nullptr);
			l_container.set_rebuild_fn_ptr(		nullptr);
			l_container.set_remap_fn_ptr(		nullptr);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		vufCurveContainer_4d& l_in_container = *(l_in_data->m_internal_data.get());

		l_container.set_curve_ptr(			l_in_container.get_curve_ptr());
		l_container.set_quaternion_fn_ptr(	l_in_container.get_quaternion_fn_ptr());
		l_container.set_scale_fn_ptr(		l_in_container.get_scale_fn_ptr());
		l_container.set_rebuild_fn_ptr(		l_in_container.get_rebuild_fn_ptr());
		l_container.set_remap_fn_ptr(		l_in_container.get_remap_fn_ptr());

		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
