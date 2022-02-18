#include <dataCollectors/vufMatrixListNode.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MMatrix.h>
//#include <maya/MMatrixArray.h>
//#include <maya/MFnMatrixArrayData.h>
#include <vufObjectArrayFn.h>


using namespace vufRM;
using namespace vufMath;

MObject	vufMatrixListNode::g_transform_in_attr;
MObject	vufMatrixListNode::g_data_out_attr;

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

	g_transform_in_attr = l_matrix_attr_fn.create(g_in_mbld_long_s, g_in_mbld_s, MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_matrix_attr_fn.setWritable(true);
	l_matrix_attr_fn.setStorable(true);
	l_matrix_attr_fn.setHidden(false);
	l_matrix_attr_fn.setKeyable(true);
	l_matrix_attr_fn.setArray(true);
	l_matrix_attr_fn.setIndexMatters(false);
	//l_matrix_attr_fn.setUsesArrayDataBuilder(true);
	l_matrix_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);

	g_data_out_attr = l_typed_attr_fn.create(g_out_mlist_long_s, g_out_mlist_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	//g_data_out_attr = l_typed_attr_fn.create("TransformList","tl", mpxTransformListWrapper::g_id, MObject::kNullObj, &l_status);// , MFnPluginData().create(vufUniversalData::id, &status));
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(false);
	l_typed_attr_fn.setWritable(false);

	l_status = addAttribute(g_transform_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_transform_in_attr,	g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	return MS::kSuccess;
}
void 	vufMatrixListNode::postConstructor()
{
	setExistWithoutOutConnections(true);
	setExistWithoutInConnections(true);
}
MStatus	vufMatrixListNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{	
		MStatus l_status;
vuf::vufTimer l_timer("Matrix collector node compute: ");
		//------------------------------------------------------------------------------
		// handle out data
		std::shared_ptr<vufMatrixListData>	l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		//VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data);
		VF_CHECK_AND_CREATE_INTERNAL_DATA(l_out_data, vufObjectArray<vufMatrix_4d>);
		auto& l_matrix_array = (l_out_data->m_internal_data.get())->m_array_v;
		//------------------------------------------------------------------------------
		// Read Transforms and fill matricies
		//MMatrixArray l_matrix_array;
		MArrayDataHandle l_ah = p_data.inputArrayValue(g_transform_in_attr);
		//MArrayDataHandle l_ah = p_data.outputArrayValue(g_transform_in_attr);
		unsigned int l_matr_arr_length = l_ah.elementCount();
		if (l_matr_arr_length != l_matrix_array.size())
		{
			l_matrix_array.resize(l_matr_arr_length);
		}
		int l_matrix_index = 0;
		for (;;)
		{
			l_matrix_array[l_matrix_index] = *((vufMath::vufMatrix_4d*)(&l_ah.inputValue().asMatrix()));
			++l_matrix_index;
			if (!l_ah.next()) break;
		}		
		// set output handle
		p_data.setClean(g_data_out_attr);

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}