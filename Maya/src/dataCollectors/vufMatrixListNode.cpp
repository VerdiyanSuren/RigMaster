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

MObject	vufMatrixListNode::g_transform_in_attr;
MObject	vufMatrixListNode::g_data_out_attr;

vufMatrixListNode::vufMatrixListNode() :MPxNode()
{}

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

	g_transform_in_attr = l_matrix_attr_fn.create("xForm", "xf", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_matrix_attr_fn.setWritable(true);
	l_matrix_attr_fn.setStorable(true);
	l_matrix_attr_fn.setHidden(false);
	l_matrix_attr_fn.setKeyable(true);
	l_matrix_attr_fn.setArray(true);
	l_matrix_attr_fn.setUsesArrayDataBuilder(true);
	l_matrix_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);

	g_data_out_attr = l_typed_attr_fn.create("outMatrixList", "oml", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	//g_data_out_attr = l_typed_attr_fn.create("TransformList","tl", mpxTransformListWrapper::g_id, MObject::kNullObj, &l_status);// , MFnPluginData().create(vufUniversalData::id, &status));
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	l_status = addAttribute(g_transform_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_transform_in_attr,	g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufMatrixListNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{		
		// Read Transforms and fill matricies
		MArrayDataHandle l_ah = p_data.inputValue(g_transform_in_attr);
		unsigned int l_matr_arr_length = l_ah.elementCount();
		if (m_matrix_array.length() != l_matr_arr_length)
		{
			m_matrix_array.setLength(l_matr_arr_length);
		}
		int l_matrix_index = 0;
		for (;;)
		{
			m_matrix_array[l_matrix_index] = l_ah.inputValue().asMatrix();
			++l_matrix_index;
			if (!l_ah.next()) break;
		}
		
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