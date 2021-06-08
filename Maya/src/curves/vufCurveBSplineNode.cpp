#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MGlobal.h>

#include <curves/vufCurveBSplineNode.h>
#include <curves/vufCurveOpenBSpline.h>
#include <math/vufMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MFnMatrixArrayData.h>

#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

using namespace vufRM;
using namespace vufMath;

// Curve
MObject	vufCurveBSplineNode::g_curve_compound_attr;
MObject	vufCurveBSplineNode::g_close_attr;
MObject	vufCurveBSplineNode::g_degree_attr;

VF_RM_CRV_NODE_DEFINE_QUATERNIONS_ATTR(vufCurveBSplineNode);
VF_RM_CRV_NODE_DEFINE_REBUILD_ATTR(vufCurveBSplineNode);
VF_RM_CRV_NODE_DEFINE_SCALE_ATTR(vufCurveBSplineNode);

MObject	vufCurveBSplineNode::g_transfoms_attr;
MObject	vufCurveBSplineNode::g_data_out_attr;

vufCurveBSplineNode::vufCurveBSplineNode():MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveBSplineNode::creator()
{
	return new vufCurveBSplineNode();
}
MStatus	vufCurveBSplineNode::initialize()
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
	CHECK_MSTATUS(l_enum_attr_fn.addField("1",	k_linear));
	CHECK_MSTATUS(l_enum_attr_fn.addField("2",	k_quadratic));
	CHECK_MSTATUS(l_enum_attr_fn.addField("3",  k_cubic));
	CHECK_MSTATUS(l_enum_attr_fn.addField("4",	k_tetra));
	CHECK_MSTATUS(l_enum_attr_fn.addField("5",	k_penta));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(3));	
	// Close
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_close_attr, "close", "cls", kBoolean, false);
	// Curve Compound
	g_curve_compound_attr = l_compound_attr_fn.create("curve", "crv", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_close_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_degree_attr));
#pragma endregion
	//------------------------------------------------------------------------------------------------
	// QuaternionsFn
	// apply quaternions
	VF_RM_CRV_NODE_INIT_REBUILD_ATTR();
	VF_RM_CRV_NODE_INIT_QUATERNIONS_ATTR();
	VF_RM_CRV_NODE_INIT_SCALE_ATTR();
	//------------------------------------------------------------------------------------------------
	// Transform list
	g_transfoms_attr = l_typed_attr_fn.create("xformList", "xfr", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	//------------------------------------------------------------------------------------------------
	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id,MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	//------------------------------------------------------------------------------------------------
	// Add Attributes
	l_status = addAttribute(g_curve_compound_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_transfoms_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//------------------------------------------------------------------------------------------------
	//  Attributes Affects
	l_status = attributeAffects(g_curve_compound_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_degree_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_close_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	VF_RM_CRV_NODE_REBUILD_ATTR_AFFECT_TO(		g_data_out_attr);
	VF_RM_CRV_NODE_QUATERNIONS_ATTR_AFFECT_TO(	g_data_out_attr);
	VF_RM_CRV_NODE_SCALE_ATTR_AFFECT_TO(		g_data_out_attr);

	l_status = attributeAffects(g_transfoms_attr, g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveBSplineNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr )
	{
		MStatus l_status;
		//------------------------------------------------------------------------------
		// handle out data
		std::shared_ptr<vufCurveData>	l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data);
		//------------------------------------------------------------------------------
		// reference check
		auto l_data_owner_id = l_out_data->get_owner_id();
		if (l_out_data->get_owner_id() != m_gen_id)
		{
			//To Do Copy data and make mine
		}
		l_out_data->set_owner_id(m_gen_id);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		vufCurveContainer_4d& l_container = *(l_out_data->m_internal_data.get());
#pragma region HANDLE_INPUT_TRANSFORM_LIST
		//------------------------------------------------------------------------------
		// Handle input transform list data
		MDataHandle l_input_data = p_data.inputValue(g_transfoms_attr);
		MObject		l_in_obj = l_input_data.data();
		MFnMatrixArrayData l_in_data_fn(l_in_obj,&l_status);
		MMatrixArray l_matrix_array = l_in_data_fn.array(&l_status);

		uint32_t l_transforms_sz = (uint32_t)l_matrix_array.length();		
#pragma endregion		
#pragma region HANDLE_CURVE		
		//-------------------------------------------------------------------------------
		// Handle Curve
		bool	l_close			= p_data.inputValue(g_close_attr).asBool();
		short	l_degree		= p_data.inputValue(g_degree_attr).asShort();
		bool	l_is_crv_new	= (l_close ==true) ? 
			l_out_data->m_internal_data->switch_curve(l_degree, vufMath::vufCurveType::k_close_bspline):
			l_out_data->m_internal_data->switch_curve(l_degree, vufMath::vufCurveType::k_open_bspline);
		
		auto l_crv_ptr = l_out_data->m_internal_data->get_curve_ptr()->as_explicit_curve();
		if (l_crv_ptr == nullptr)
		{
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		l_crv_ptr->set_nodes_count( l_transforms_sz );
		for (uint32_t i = 0; i < l_transforms_sz; ++i)
		{
			vufMatrix4<double>* l_matr = (vufMatrix4<double>*)& l_matrix_array[i];
			l_crv_ptr->set_node_at(i, l_matr->get_translation_4());
		}
		// TO DO
		// Check if curve is valid
		if (l_crv_ptr->is_valid() == false)
		{
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
#pragma endregion		
#pragma region HANDLE_REBUILD
		//------------------------------------------------------------------------------
		// Handle rebuild fn 
		VF_RM_CRV_NODE_READ_REBUILD_ATTR();
		if (l_rebuild_mode == 0 /*apply. always refresh*/)
		{
			l_out_data->m_internal_data->switch_rebuild_fn(vufCurveRebuildFnType::k_constant_step);
			std::shared_ptr<vufCurveRebuildFn_4d> l_rbl_ptr =  l_out_data->m_internal_data->get_rebuild_fn_ptr();
			if (l_rbl_ptr != nullptr)
			{
				auto l_r_ptr = l_rbl_ptr->as_uniform_rebuild_fn();
				l_r_ptr->set_clamp_start(l_rbld_pin_start);
				l_r_ptr->set_clamp_start_value(l_rbld_pin_start_value);
				l_r_ptr->set_clamp_end(l_rbld_pin_end);
				l_r_ptr->set_clamp_end_value(l_rbld_pin_end_value);
				l_r_ptr->set_offset(l_rbld_offset);
				l_r_ptr->m_div_per_segment = l_rbld_samples;
				l_r_ptr->rebuild(*(l_container.get_curve_ptr()));
			}
		}
		if (l_rebuild_mode == 1 /* keep rebuild fn*/)
		{
			std::shared_ptr<vufCurveRebuildFn_4d> l_rbl_ptr = l_out_data->m_internal_data->get_rebuild_fn_ptr();
			l_rbl_ptr->set_clamp_start(l_rbld_pin_start);
			l_rbl_ptr->set_clamp_start_value(l_rbld_pin_start_value);
			l_rbl_ptr->set_clamp_end(l_rbld_pin_end);
			l_rbl_ptr->set_clamp_end_value(l_rbld_pin_end_value);
			l_rbl_ptr->set_offset(l_rbld_offset);

		}
		if (l_rebuild_mode == 2 /* delete rebuild fn*/)
		{
			l_out_data->m_internal_data->set_rebuild_fn_ptr(nullptr);
		}		

#pragma endregion
#pragma region HANDLE_QUATERNION		
		//------------------------------------------------------------------------------
		// Handle quaternion		
		VF_RM_CRV_NODE_READ_QUATERNIONS_ATTR();

		if (l_quat_mode == 0 /* apply. always refresh*/)
		{
			l_out_data->m_internal_data->switch_quaternion_fn(vufCurveQuatFnType::k_closest);
			std::shared_ptr<vufCurveQuaternionFn_4d> l_quaternion_ptr = l_out_data->m_internal_data->get_quaternion_fn_ptr();
			if (l_quaternion_ptr != nullptr)
			{
				auto l_qtr_ptr = l_quaternion_ptr->as_closest_fn();
				l_qtr_ptr->set_pin_start(l_quat_pin_start);
				l_qtr_ptr->set_pin_start_value(l_quat_pin_start_value);
				l_qtr_ptr->set_pin_end(l_quat_pin_end);
				l_qtr_ptr->set_pin_end_value(l_quat_pin_end_value);
				l_qtr_ptr->set_offset(l_quat_offset_value);
				l_qtr_ptr->set_item_count(l_transforms_sz);
				for (uint32_t i = 0; i < l_transforms_sz; ++i)
				{
					vufMatrix4<double>* l_matr = (vufMatrix4<double>*) &l_matrix_array[i];
					l_qtr_ptr->set_item_at(i, *l_matr);
				}
				l_qtr_ptr->compute_bind_params(l_container,10);
				l_qtr_ptr->match_quaternions(l_container);
				//VF_LOG_INFO(l_qtr_ptr->to_string());
			}
		}
		if (l_quat_mode == 1 /* just update quaternions values*/)
		{
			std::shared_ptr<vufCurveQuaternionFn_4d> l_qtr_ptr = l_out_data->m_internal_data->get_quaternion_fn_ptr();
			if (l_qtr_ptr != nullptr)
			{

				l_qtr_ptr->set_pin_start(l_quat_pin_start);
				l_qtr_ptr->set_pin_start_value(l_quat_pin_start_value);
				l_qtr_ptr->set_pin_end(l_quat_pin_end);
				l_qtr_ptr->set_pin_end_value(l_quat_pin_end_value);
				l_qtr_ptr->set_offset(l_quat_offset_value);
				for (uint32_t i = 0; i < l_transforms_sz; ++i)
				{
					vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
					l_qtr_ptr->set_item_at(i, *l_matr);
				}
				l_qtr_ptr->match_quaternions(l_container);
			}
		}
		if (l_quat_mode == 2 /*delete quaternion fn*/)
		{
			l_out_data->m_internal_data->set_quaternion_fn_ptr(nullptr);
		}
#pragma endregion
#pragma region HANDLE_SCALE
		//------------------------------------------------------------------------------
		// Handle scale
		VF_RM_CRV_NODE_READ_SCALE_ATTR();
		if (l_scale_mode == 0 /* apply. always refresh*/)
		{
			l_out_data->m_internal_data->switch_scale_fn(vufCurveScaleFnType::k_closest_params);
			
			std::shared_ptr<vufCurveScaleFn_4d> l_scale_ptr = l_out_data->m_internal_data->get_scale_fn_ptr();
			if (l_scale_ptr != nullptr)
			{
				l_scale_ptr->set_pin_start(l_scale_pin_start);
				l_scale_ptr->set_pin_start_value(l_scale_pin_start_value);
				l_scale_ptr->set_pin_end(l_scale_pin_end);
				l_scale_ptr->set_pin_end_value(l_scale_pin_end_value);
				l_scale_ptr->set_offset(l_scale_offset_value);
				
				l_scale_ptr->set_item_count(l_transforms_sz);
				for (uint32_t i = 0; i < l_transforms_sz; ++i)
				{
					vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
					vufVector_4d l_scl_vec(	l_matr->get_scale_x(),
											l_matr->get_scale_y(),
											l_matr->get_scale_z());
					l_scale_ptr->set_item_at(i, l_matr->get_translation_4(), l_scl_vec);
				}
				l_scale_ptr->compute_bind_param(l_container, 10);
				l_scale_ptr->match_scales(l_container);
			}
		}
		if (l_scale_mode == 1 /* just update scale fn */)
		{
			l_out_data->m_internal_data->switch_scale_fn(vufCurveScaleFnType::k_closest_params);
			std::shared_ptr<vufCurveScaleFn_4d> l_scale_ptr = l_out_data->m_internal_data->get_scale_fn_ptr();
			if (l_scale_ptr != nullptr)
			{
				l_scale_ptr->set_pin_start(l_scale_pin_start);
				l_scale_ptr->set_pin_start_value(l_scale_pin_start_value);
				l_scale_ptr->set_pin_end(l_scale_pin_end);
				l_scale_ptr->set_pin_end_value(l_scale_pin_end_value);
				l_scale_ptr->set_offset(l_scale_offset_value);

				l_scale_ptr->set_item_count(l_transforms_sz);
				for (uint32_t i = 0; i < l_transforms_sz; ++i)
				{
					vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
					vufVector_4d l_scl_vec(	l_matr->get_scale_x(),
											l_matr->get_scale_y(),
											l_matr->get_scale_z());
					l_scale_ptr->set_item_at(i, l_matr->get_translation_4(), l_scl_vec);
				}
			}
		}
		if (l_scale_mode == 2 /* delete scale fn */)
		{
			l_out_data->m_internal_data->set_scale_fn_ptr(nullptr);
		}
#pragma endregion
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}

