#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MGlobal.h>

#include <curves/vufCurveBSplineNode.h>
#include <curves/explicit/vufCurveOpenBSpline.h>
#include <math/vufMatrix.h>

#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>
#include <dataCollectors/vufDataCollectorInclude.h>

using namespace vufRM;
using namespace vufMath;

// Curve
MObject	vufCurveBSplineNode::g_curve_compound_attr;
MObject	vufCurveBSplineNode::g_close_attr;
MObject	vufCurveBSplineNode::g_degree_attr;
MObject	vufCurveBSplineNode::g_closest_division_attr;
MObject	vufCurveBSplineNode::g_rebuild_store_attr;
MObject	vufCurveBSplineNode::g_quaternion_store_attr;
MObject	vufCurveBSplineNode::g_scale_store_attr;
//MObject	vufCurveBSplineNode::g_remap_store_attr;
VF_RM_CRV_NODE_DEFINE_QUATERNIONS_ATTR(vufCurveBSplineNode);
VF_RM_CRV_NODE_DEFINE_REBUILD_ATTR(vufCurveBSplineNode);
VF_RM_CRV_NODE_DEFINE_SCALE_ATTR(vufCurveBSplineNode);
MObject	vufCurveBSplineNode::g_transfoms_attr;
MObject	vufCurveBSplineNode::g_data_out_attr;
MObject	vufCurveBSplineNode::g_params_out_attr;

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
	//CHECK_MSTATUS(l_enum_attr_fn.addField("4",	k_tetra));
	//CHECK_MSTATUS(l_enum_attr_fn.addField("5",	k_penta));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(2));

	// Close
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_close_attr, "close", "cls", kBoolean, false);

	// Curve Compound
	g_curve_compound_attr = l_compound_attr_fn.create("curve", "crv", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_close_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_degree_attr));
	l_status = addAttribute(g_curve_compound_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);

	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_closest_division_attr, "division", "div", kInt, 10);
	l_status = addAttribute(g_closest_division_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
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
	// Out double attr
	g_params_out_attr = l_typed_attr_fn.create("outDoubleList", "odl", MFnData::kDoubleArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_params_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	//------------------------------------------------------------------------------------------------
	// Add Attributes

	l_status = addAttribute(g_transfoms_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	//------------------------------------------------------------------------------------------------
	//  Attributes Affects
	l_status = attributeAffects(g_transfoms_attr, g_params_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	l_status = attributeAffects(g_transfoms_attr,			g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_curve_compound_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_degree_attr,				g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_close_attr,				g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	VF_RM_CRV_NODE_REBUILD_ATTR_AFFECT_TO(		g_data_out_attr);
	VF_RM_CRV_NODE_QUATERNIONS_ATTR_AFFECT_TO(	g_data_out_attr);
	VF_RM_CRV_NODE_SCALE_ATTR_AFFECT_TO(		g_data_out_attr);

	VF_RM_INIT_AND_ADD_HIDDEN_ATTR(g_rebuild_store_attr,	"hRebuild",		"hrb", mpxCurveRebuildWrapper);
	VF_RM_INIT_AND_ADD_HIDDEN_ATTR(g_quaternion_store_attr, "hRotation",	"hrt", mpxCurveQuatWrapper);
	VF_RM_INIT_AND_ADD_HIDDEN_ATTR(g_scale_store_attr,		"hScale",		"hsc", mpxCurveScaleWrapper);


	return MS::kSuccess;
}
MStatus	vufCurveBSplineNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr || p_plug == g_params_out_attr)
	{
		vuf::vufTimer l_timer("Bspline curve node compute");
		MStatus l_status;
		//------------------------------------------------------------------------------
		// handle out data
		std::shared_ptr<vufCurveData>	l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		//VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		vufCurveContainer_4d& l_container = *(l_out_data->m_internal_data.get());
		//------------------------------------------------------------------------------
		// Handle input transform list data
		MMatrixArray l_matrix_array;
		MDoubleArray l_double_array;
		VF_RM_GET_MATRIX_ARRAY_FROM_IN(g_transfoms_attr, l_matrix_array);
		uint32_t l_transforms_sz = (uint32_t)l_matrix_array.length();
		l_double_array.setLength(l_transforms_sz);
		//-----------------------------------------------------------------------------
		// Read attributes
		bool	l_close			= p_data.inputValue(g_close_attr).asBool();
		short	l_degree		= p_data.inputValue(g_degree_attr).asShort();
		//int		l_cls_div		= p_data.inputValue(g_closest_division_attr).asInt();
		short	l_quat_mode		= p_data.inputValue(g_quaternion_mode_attr).asShort();
		short	l_scale_mode	= p_data.inputValue(g_scale_mode_attr).asShort();
#pragma region HANDLE_CURVE
		//-------------------------------------------------------------------------------
		// Handle Curve
		// To Do OPTIMISE THIS LATER
		bool	l_is_crv_new	= (l_close ==true) ? 
			l_out_data->m_internal_data->switch_curve(l_degree, vufMath::vufCurveType::k_close_bspline):
			l_out_data->m_internal_data->switch_curve(l_degree, vufMath::vufCurveType::k_open_bspline);
		
		auto l_crv_ptr = l_out_data->m_internal_data->get_curve_ptr()->as_explicit_curve();
		if (l_crv_ptr == nullptr)
		{
			MString l_msg = name() + ": Can't  create bspline curve";
			VF_LOG_ERR(l_msg.asChar());
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		l_crv_ptr->set_nodes_count( l_transforms_sz );
		for (uint32_t i = 0; i < l_transforms_sz; ++i)
		{
			vufMatrix4<double>* l_matr = (vufMatrix4<double>*)& l_matrix_array[i];
			vufVector4<double> l_pos = l_matr->get_translation_4();
			l_crv_ptr->set_node_at(i, l_pos);
		}
		if (l_crv_ptr->is_valid() == false)
		{
			MString l_msg = name() + ": Curve is invalid";
			VF_LOG_ERR(l_msg.asChar());
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		if (l_quat_mode == 0 || l_scale_mode == 0)
		{
			//std::cout << "-------------------------------" << std::endl;
			for (uint32_t i = 0; i < l_transforms_sz; ++i)
			{
				//std::cout << i << ": " << std::endl;
				vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
				vufVector4<double> l_pos = l_matr->get_translation_4();
				l_double_array[i] = l_crv_ptr->get_closest_point_param(l_pos, 0.0, 1.0, 10);
				//std::cout << l_double_array[i] << " ";
			}
		}
#pragma endregion
#pragma region HANDLE_QUATERNION
		//------------------------------------------------------------------------------
		// Handle quaternion
		std::shared_ptr<vufCurveQuatData>	l_quaternion_store_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveQuatWrapper, vufCurveQuatData, p_data, g_quaternion_store_attr, l_quaternion_store_data, m_gen_id);
		//VF_RM_CRV_NODE_READ_QUATERNIONS_ATTR();
		if (l_quat_mode == 0 /* apply. always refresh*/)
		{
			l_out_data->m_internal_data->switch_quaternion_fn(vufCurveQuatFnType::k_param);
			auto l_qtr_ptr = l_out_data->m_internal_data->get_quaternion_fn_ptr()->as_params_fn();
			if (l_qtr_ptr == nullptr)
			{
				MString l_msg = name() + ": Failed to create rotation solver";
				VF_LOG_ERR(l_msg.asChar());
				p_data.setClean(g_data_out_attr);
				return MS::kSuccess;
			}
			l_qtr_ptr->set_item_count_i(l_transforms_sz);
			for (uint32_t i = 0; i < l_transforms_sz; ++i)
			{
				vufMatrix4<double>* l_matr = (vufMatrix4<double>*) &l_matrix_array[i];
				// set params and quaternions
				l_qtr_ptr->set_item_at_i(i,l_double_array[i], *l_matr, l_crv_ptr);
			}
			l_qtr_ptr->match_quaternions_i();

			l_quaternion_store_data->m_internal_data = l_qtr_ptr;
			//VF_LOG_INFO(l_qtr_ptr->to_string());
		}
		if (l_quat_mode == 1 /* just update quaternions values*/)
		{
			auto l_quat_store_ptr = l_quaternion_store_data->m_internal_data;
			if (l_quat_store_ptr != nullptr)
			{
				auto l_qtr_ptr = l_quat_store_ptr->as_params_fn();
				if (l_qtr_ptr != nullptr)
				{
					for (uint32_t i = 0; i < l_transforms_sz; ++i)
					{
						vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
						// update only quaterniions
						l_qtr_ptr->set_item_at_i(i, *l_matr, l_crv_ptr);
					}
					l_qtr_ptr->match_quaternions_i();
				}
			}
			l_out_data->m_internal_data->set_quaternion_fn_ptr(l_quat_store_ptr);
		}
		if (l_quat_mode == 2 /*delete quaternion fn*/)
		{
			l_out_data->m_internal_data->set_quaternion_fn_ptr(nullptr);
			//l_quaternion_store_data->m_internal_data = nullptr;
		}
#pragma endregion
#pragma region HANDLE_SCALE
		//------------------------------------------------------------------------------
		// Handle scale
		std::shared_ptr<vufCurveScaleData>	l_scale_store_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveScaleWrapper, vufCurveScaleData, p_data, g_scale_store_attr, l_scale_store_data, m_gen_id);
		//VF_RM_CRV_NODE_READ_SCALE_ATTR();

		if (l_scale_mode == 0 /* apply. always refresh*/)
		{
			l_out_data->m_internal_data->switch_scale_fn(vufCurveScaleFnType::k_params);
			auto l_scale_ptr = l_out_data->m_internal_data->get_scale_fn_ptr()->as_params_fn();
			if (l_scale_ptr == nullptr)
			{
				MString l_msg = name() + ": Failed to create scale solver";
				VF_LOG_ERR(l_msg.asChar());
				p_data.setClean(g_data_out_attr);
				return MS::kSuccess;
			}
			l_scale_ptr->set_item_count_i(l_transforms_sz);
			for (uint32_t i = 0; i < l_transforms_sz; ++i)
			{
				vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];	
				l_scale_ptr->set_item_at_i(i, l_double_array[i], *l_matr);
			}
			l_scale_ptr->match_scales_i();
			l_scale_store_data->m_internal_data = l_scale_ptr;
		}
		if (l_scale_mode == 1 /* just update scale fn */)
		{
			auto l_scale_store_ptr = l_scale_store_data->m_internal_data;
			if (l_scale_store_ptr != nullptr)
			{
				auto l_scl_ptr = l_scale_store_ptr->as_params_fn();
				if (l_scl_ptr != nullptr)
				{
					for (uint32_t i = 0; i < l_transforms_sz; ++i)
					{
						vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
						// update only quaterniions
						l_scl_ptr->set_item_at_i(i, *l_matr);
					}
					l_scl_ptr->match_scales_i();
				}
			}
			l_out_data->m_internal_data->set_scale_fn_ptr(l_scale_store_ptr);
		}
		if (l_scale_mode == 2 /* delete scale fn */)
		{
			l_out_data->m_internal_data->set_scale_fn_ptr(nullptr);
			//l_scale_store_data->m_internal_data = nullptr;
		}
#pragma endregion
#pragma region HANDLE_REBUILD
		//------------------------------------------------------------------------------
		// Handle rebuild fn 
		std::shared_ptr<vufCurveRebuildData>	l_rebuild_store_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveRebuildWrapper, vufCurveRebuildData, p_data, g_rebuild_store_attr, l_rebuild_store_data, m_gen_id);
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
				l_rebuild_store_data->m_internal_data = l_rbl_ptr;
			}
		}
		if (l_rebuild_mode == 1 /* keep rebuild fn*/)
		{
			std::shared_ptr<vufCurveRebuildFn_4d> l_rbl_ptr = l_rebuild_store_data->m_internal_data;
			if (l_rbl_ptr != nullptr)
			{
				l_rbl_ptr->set_clamp_start(l_rbld_pin_start);
				l_rbl_ptr->set_clamp_start_value(l_rbld_pin_start_value);
				l_rbl_ptr->set_clamp_end(l_rbld_pin_end);
				l_rbl_ptr->set_clamp_end_value(l_rbld_pin_end_value);
				l_rbl_ptr->set_offset(l_rbld_offset);
			}
			l_out_data->m_internal_data->set_rebuild_fn_ptr(l_rbl_ptr);
		}
		if (l_rebuild_mode == 2 /* delete rebuild fn*/)
		{
			l_out_data->m_internal_data->set_rebuild_fn_ptr(nullptr);
			//l_rebuild_store_data->m_internal_data = nullptr;
		}

#pragma endregion
		p_data.setClean(g_rebuild_store_attr);
		p_data.setClean(g_quaternion_store_attr);
		p_data.setClean(g_scale_store_attr);
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
