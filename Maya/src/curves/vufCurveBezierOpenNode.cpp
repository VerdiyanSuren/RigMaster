#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MGlobal.h>

#include <curves/vufCurveBezierOpenNode.h>
#include <curves/explicit/vufCurveBezierOpen.h>
#include <math/vufMatrix.h>

#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveBezierOpenNode::g_curve_compound_attr;
MObject	vufCurveBezierOpenNode::g_degree_attr;
MObject	vufCurveBezierOpenNode::g_rebuild_store_attr;
MObject	vufCurveBezierOpenNode::g_quaternion_store_attr;
MObject	vufCurveBezierOpenNode::g_scale_store_attr;
//MObject	vufCurveBSplineNode::g_remap_store_attr;

VF_RM_CRV_NODE_DEFINE_QUATERNIONS_ATTR(vufCurveBezierOpenNode);
VF_RM_CRV_NODE_DEFINE_REBUILD_ATTR(vufCurveBezierOpenNode);
VF_RM_CRV_NODE_DEFINE_SCALE_ATTR(vufCurveBezierOpenNode);

MObject	vufCurveBezierOpenNode::g_transfoms_attr;
MObject	vufCurveBezierOpenNode::g_data_out_attr;

vufCurveBezierOpenNode::vufCurveBezierOpenNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveBezierOpenNode::creator()
{
	return new vufCurveBezierOpenNode();
}
MStatus	vufCurveBezierOpenNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;

#pragma region CURVE_ATTR
	//------------------------------------------------------------------------------------------------
	// Curve 
	// Degree
	g_degree_attr = l_enum_attr_fn.create("degree", "dgr", 3, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_enum_attr_fn.addField("1", 0));
	CHECK_MSTATUS(l_enum_attr_fn.addField("2", 1));
	CHECK_MSTATUS(l_enum_attr_fn.addField("3", 2));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(2));
	g_curve_compound_attr = l_compound_attr_fn.create("curve", "crv", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_degree_attr));
	l_status = addAttribute(g_curve_compound_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

#pragma endregion
	//------------------------------------------------------------------------------------------------
// Transform list
	g_transfoms_attr = l_typed_attr_fn.create("xformList", "xfr", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	l_status = addAttribute(g_transfoms_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	//------------------------------------------------------------------------------------------------
	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_curve_compound_attr, g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_degree_attr, g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transfoms_attr, g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveBezierOpenNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		std::shared_ptr<vufCurveData>	l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		//------------------------------------------------------------------------------
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		vufCurveContainer_4d& l_container = *(l_out_data->m_internal_data.get());
		//------------------------------------------------------------------------------
#pragma region HANDLE_INPUT_TRANSFORM_LIST
	// Handle input transform list data
		MDataHandle l_input_data = p_data.inputValue(g_transfoms_attr);
		MObject		l_in_obj = l_input_data.data();
		MFnMatrixArrayData l_in_data_fn(l_in_obj, &l_status);
		MMatrixArray l_matrix_array = l_in_data_fn.array(&l_status);

		uint32_t l_transforms_sz = (uint32_t)l_matrix_array.length();
#pragma endregion
		short	l_degree = p_data.inputValue(g_degree_attr).asShort();
		l_out_data->m_internal_data->switch_curve(l_degree + 1, vufMath::vufCurveType::k_open_bezier_piecewise);
		auto l_crv_ptr = l_out_data->m_internal_data->get_curve_ptr()->as_explicit_curve();
		if (l_crv_ptr == nullptr)
		{
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		l_crv_ptr->set_nodes_count(l_transforms_sz);
		for (uint32_t i = 0; i < l_transforms_sz; ++i)
		{
			vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
			l_crv_ptr->set_node_at(i, l_matr->get_translation_4());
		}
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
