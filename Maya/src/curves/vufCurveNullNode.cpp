#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <curves/vufCurveNullNode.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

using namespace vufRM;

MObject	vufCurveNullNode::g_lock_attr;
MObject	vufCurveNullNode::g_data_in_attr;
MObject	vufCurveNullNode::g_data_out_attr;
MObject	vufCurveNullNode::g_store_data_attr;

void* vufCurveNullNode::creator()
{
	return new vufCurveNullNode();
}
MStatus	vufCurveNullNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	/*
	g_lock_attr = l_numeric_attr_fn.create("lock", "lc", MFnNumericData::kBoolean, true, &l_status);
	
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_numeric_attr_fn.setWritable(true);
	l_numeric_attr_fn.setStorable(true);
	l_numeric_attr_fn.setHidden(false);
	l_numeric_attr_fn.setKeyable(true);
	*/
	VF_RM_CREATE_AND_ADD_LOCK_ATTR();
	// Input Data
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// Internal Data to store and lock
	g_store_data_attr = l_typed_attr_fn.create("hiddenCD", "hcd", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setConnectable(false);
	l_typed_attr_fn.setHidden(true);
	// Output Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "ocd", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	
	// Add Attributes
	l_status = addAttribute(g_data_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_store_data_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attribute affects
	l_status = attributeAffects(g_lock_attr, g_data_out_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr, g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveNullNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		bool	l_locked = p_data.inputValue(g_lock_attr).asBool();
		// if unlocked just pass
		if (l_locked == false)
		{
			//VF_LOG_INFO("UNLOCKED");
			m_locked = false;
			std::shared_ptr<vufCurveData> l_in_data;
			std::shared_ptr<vufCurveData> l_out_data;
			std::shared_ptr<vufCurveData> l_store_data;

			VF_RM_GET_DATA_FROM_OUT_AND_CREATE(	mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr,		l_out_data);
			VF_RM_GET_DATA_FROM_OUT_AND_CREATE(	mpxCurveWrapper, vufCurveData, p_data, g_store_data_attr,	l_store_data);
			VF_RM_GET_DATA_FROM_IN(				mpxCurveWrapper, vufCurveData, p_data, g_data_in_attr,		l_in_data);
			if (l_in_data != nullptr)
			{
				l_out_data->m_internal_data = l_in_data->m_internal_data;
			}
			l_store_data->m_internal_data = nullptr;
			p_data.setClean(g_store_data_attr);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		// if just locked
		if (m_locked == false)
		{
			//VF_LOG_INFO("JUST LOCKED");
			m_locked = true;
			std::shared_ptr<vufCurveData> l_in_data;
			std::shared_ptr<vufCurveData> l_out_data;
			std::shared_ptr<vufCurveData> l_store_data;
			VF_RM_GET_DATA_FROM_OUT_AND_CREATE(	mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr,	l_out_data);
			VF_RM_GET_DATA_FROM_OUT_AND_CREATE(	mpxCurveWrapper, vufCurveData, p_data, g_store_data_attr,l_store_data);
			VF_RM_GET_DATA_FROM_IN(				mpxCurveWrapper, vufCurveData, p_data, g_data_in_attr,	l_in_data);
			if (l_in_data != nullptr)
			{
				l_out_data->m_internal_data		= l_in_data->m_internal_data;
				// To Do 
				// copy enstead of assigning
				l_store_data->m_internal_data	= l_in_data->m_internal_data;
			}
			p_data.setClean(g_store_data_attr);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		//VF_LOG_INFO("LOCKED");
		// if locked
		std::shared_ptr<vufCurveData> l_out_data;
		std::shared_ptr<vufCurveData> l_store_data;
		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr,	l_out_data);
		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_store_data_attr, l_store_data);
		l_out_data->m_internal_data = l_store_data->m_internal_data;
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;		
	}
	return MS::kUnknownParameter;
}

/*
MStatus	vufQuatNullNode::connectionBroken(const MPlug& p_my_plug, const MPlug& p_other_plug, bool asSrc)
{
	if (p_my_plug == g_data_in_attr)
	{
		//To Do 
		// reset or make copy of data
	}
	return MS::kSuccess;
}
*/
