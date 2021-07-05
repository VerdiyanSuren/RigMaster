#include <dataCollectors/vufDoubleListNode.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
//#include <maya/MFnDoubleAttribute.h>
#include <maya/MArrayDataHandle.h>
//#include <maya/MMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnDoubleArrayData.h>


using namespace vufRM;

MObject	vufDoubleListNode::g_double_in_attr;
MObject	vufDoubleListNode::g_data_out_attr;

vufDoubleListNode::vufDoubleListNode() :MPxNode()
{}

void* vufDoubleListNode::creator()
{
	return new vufDoubleListNode();
}
MStatus	vufDoubleListNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute	l_typed_attr_fn;
	//MFnMatrixAttribute	l_matrix_attr_fn;
	MFnNumericAttribute l_numeric_attr_fn;
	//-------------------------------------------------------------------------------------------

	g_double_in_attr = l_numeric_attr_fn.create("value", "val", MFnNumericData::kDouble, 0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_numeric_attr_fn.setWritable(true);
	l_numeric_attr_fn.setStorable(true);
	l_numeric_attr_fn.setHidden(false);
	l_numeric_attr_fn.setKeyable(true);
	l_numeric_attr_fn.setArray(true);
	l_numeric_attr_fn.setUsesArrayDataBuilder(true);
	l_numeric_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);

	g_data_out_attr = l_typed_attr_fn.create("outDoubleList", "odl", MFnData::kDoubleArray, MObject::kNullObj, &l_status);
	//g_data_out_attr = l_typed_attr_fn.create("TransformList","tl", mpxTransformListWrapper::g_id, MObject::kNullObj, &l_status);// , MFnPluginData().create(vufUniversalData::id, &status));
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	l_status = addAttribute(g_double_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_double_in_attr, g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufDoubleListNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		// Read Transforms and fill matricies
		MDoubleArray l_double_array;
		MArrayDataHandle l_ah = p_data.inputValue(g_double_in_attr);
		unsigned int l_matr_arr_length = l_ah.elementCount();
		l_double_array.setLength(l_matr_arr_length);
		int l_double_index = 0;
		for (;;)
		{
			l_double_array[l_double_index] = l_ah.inputValue().asDouble();
			++l_double_index;
			if (!l_ah.next()) break;
		}
		// set output handle
		MDataHandle l_out_handle = p_data.outputValue(g_data_out_attr);
		MFnDoubleArrayData l_data_fn;
		MObject l_obj_data = l_data_fn.create(l_double_array);
		l_out_handle.setMObject(l_obj_data);
		p_data.setClean(g_data_out_attr);

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}