#include <dataCollectors/vufMatrixListNode.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MFnMatrixArrayData.h>


using namespace vufRM;

MObject	vufMatrixListNode::g_lock_attr;
MObject	vufMatrixListNode::g_transform_in_attr;
MObject	vufMatrixListNode::g_data_out_attr;
MObject	vufMatrixListNode::g_data_store_attr;


vufMatrixListNode::vufMatrixListNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}

void* vufMatrixListNode::creator()
{
	return new vufMatrixListNode();
}
MStatus	vufMatrixListNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute	l_typed_attr_fn;
	MFnMatrixAttribute	l_matrix_attr_fn;
	MFnNumericAttribute l_numeric_attr_fn;	
	//-------------------------------------------------------------------------------------------
	// Lock
	g_lock_attr = l_numeric_attr_fn.create("lock", "lc",MFnNumericData::kBoolean,true, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	//l_numeric_attr_fn.setWritable(true);
	l_numeric_attr_fn.setStorable(true);
	//l_numeric_attr_fn.setHidden(false);
	//l_numeric_attr_fn.setKeyable(true);
	//-------------------------------------------------------------------------------------------

	g_transform_in_attr = l_matrix_attr_fn.create("xForm", "xf", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_matrix_attr_fn.setWritable(true);
	l_matrix_attr_fn.setStorable(true);
	l_matrix_attr_fn.setHidden(false);
	l_matrix_attr_fn.setKeyable(true);
	l_matrix_attr_fn.setArray(true);
	l_matrix_attr_fn.setUsesArrayDataBuilder(true);
	l_matrix_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);

	g_data_store_attr = l_typed_attr_fn.create("storeMatrixList", "sml", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(true);
	l_typed_attr_fn.setConnectable(true);

	g_data_out_attr = l_typed_attr_fn.create("outMatrixList", "oml", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	//g_data_out_attr = l_typed_attr_fn.create("TransformList","tl", mpxTransformListWrapper::g_id, MObject::kNullObj, &l_status);// , MFnPluginData().create(vufUniversalData::id, &status));
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	l_status = addAttribute(g_lock_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_transform_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_store_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_lock_attr,			g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transform_in_attr,	g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufMatrixListNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		bool l_lock = p_data.inputValue(g_lock_attr).asBool();
		if (l_lock == true)
		{
			// locked. So let read from stored attribute
			MObject l_object = p_data.outputValue(g_data_store_attr).data();			
			MFnMatrixArrayData l_fn(l_object);
			m_array = l_fn.array();
			// set output
			MDataHandle l_out_handle = p_data.outputValue(g_data_out_attr);
			MFnMatrixArrayData l_data_fn;
			MObject l_obj_data = l_data_fn.create(m_array);
			l_out_handle.setMObject(l_obj_data);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		// unlocked
		// Read Transforms and fill matricies
		MArrayDataHandle l_ah = p_data.inputValue(g_transform_in_attr);
		unsigned int l_matr_arr_length = l_ah.elementCount();
		if (m_array.length() != l_matr_arr_length)
		{
			m_array.setLength(l_matr_arr_length);
		}
		int l_matrix_index = 0;
		for (;;)
		{
			m_array[l_matrix_index] = l_ah.inputValue().asMatrix();
			++l_matrix_index;
			if (!l_ah.next()) break;
		}
		// set to store
		MDataHandle l_store_handle = p_data.outputValue(g_data_store_attr);
		MFnMatrixArrayData l_data_store_fn;
		MObject l_obj_store_data = l_data_store_fn.create(m_array);
		l_store_handle.setMObject(l_obj_store_data);
		// set output handle
		MDataHandle l_out_handle = p_data.outputValue(g_data_out_attr);
		MFnMatrixArrayData l_data_fn;
		MObject l_obj_data = l_data_fn.create(m_array);
		l_out_handle.setMObject(l_obj_data);
		p_data.setClean(g_data_out_attr);
		p_data.setClean(l_obj_store_data);

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}