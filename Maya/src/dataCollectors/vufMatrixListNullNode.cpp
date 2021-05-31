#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <dataCollectors/vufMatrixListNullNode.h>
#include <data/vufData.h>
#include <vufMayaGlobalIncludes.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MFnMatrixArrayData.h>

using namespace vufRM;

MObject	vufMatrixListNullNode::g_lock_attr;
MObject	vufMatrixListNullNode::g_data_in_attr;
MObject	vufMatrixListNullNode::g_data_out_attr;
MObject	vufMatrixListNullNode::g_data_store_attr;


vufMatrixListNullNode::vufMatrixListNullNode() :MPxNode()
{}

void* vufMatrixListNullNode::creator()
{
	return new vufMatrixListNullNode();
}
MStatus	vufMatrixListNullNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	
	VF_RM_CREATE_AND_ADD_LOCK_ATTR();
	// Input Data
	g_data_in_attr	= l_typed_attr_fn.create("inMatrixList", "iml",	 MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// Internal Data to store and lock
	g_data_store_attr = l_typed_attr_fn.create("hiddenML", "hml", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setConnectable(false);
	l_typed_attr_fn.setHidden(true);
	// Output Data
	g_data_out_attr = l_typed_attr_fn.create("outMatrixList", "oml", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	//g_data_out_attr = l_typed_attr_fn.create("TransformList","tl", mpxTransformListWrapper::g_id, MObject::kNullObj, &l_status);// , MFnPluginData().create(vufUniversalData::id, &status));
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	// Add Attributes
	l_status = addAttribute(g_data_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_store_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attribute affects
	l_status = attributeAffects(g_lock_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr, g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufMatrixListNullNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		bool	l_locked = p_data.inputValue(g_lock_attr).asBool();
		// if unlocked just pass
		if (l_locked == false) 
		{
			m_locked = false;

			MDataHandle l_input_data_map	= p_data.inputValue(g_data_in_attr);
			MObject		l_in_obj			= l_input_data_map.data();
			MFnMatrixArrayData l_in_data_fn(l_in_obj);
			m_matrix_array = l_in_data_fn.array();
			// set output handle
			MDataHandle l_out_handle = p_data.outputValue(g_data_out_attr);
			MFnMatrixArrayData l_out_data_fn;
			MObject l_out_obj_data = l_out_data_fn.create(m_matrix_array);
			l_out_handle.setMObject(l_out_obj_data);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		// if just locked
		if (m_locked == false)
		{
			m_locked = true;

			MDataHandle l_input_data_map = p_data.inputValue(g_data_in_attr);
			MObject		l_in_obj = l_input_data_map.data();
			MFnMatrixArrayData l_in_data_fn(l_in_obj);
			m_matrix_array = l_in_data_fn.array();
			
			// set hidden 
			MDataHandle l_store_handle = p_data.outputValue(g_data_store_attr);
			MFnMatrixArrayData l_store_data_fn;
			MObject l_store_obj_data = l_store_data_fn.create(m_matrix_array);
			l_store_handle.setMObject(l_store_obj_data);
			
			// set output handle
			MDataHandle l_out_handle = p_data.outputValue(g_data_out_attr);
			MFnMatrixArrayData l_out_data_fn;
			MObject l_out_obj_data = l_out_data_fn.create(m_matrix_array);
			l_out_handle.setMObject(l_out_obj_data);
			p_data.setClean(g_data_out_attr);
			p_data.setClean(g_data_store_attr);

			return MS::kSuccess;
		}
		// if locked
		MDataHandle l_store_data_map = p_data.outputValue(g_data_store_attr);
		MObject		l_store_obj = l_store_data_map.data();
		MFnMatrixArrayData l_store_data_fn(l_store_obj);
		m_matrix_array = l_store_data_fn.array();
		 
		// set output handle
		MDataHandle l_out_handle = p_data.outputValue(g_data_out_attr);
		MFnMatrixArrayData l_data_fn;
		MObject l_obj_data = l_data_fn.create(m_matrix_array);
		l_out_handle.setMObject(l_obj_data);
		p_data.setClean(g_data_out_attr);

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}