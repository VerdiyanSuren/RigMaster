#include <curves/rotateFn/vufCurveQuatParamNode.h>
#include <curves/vufCurveContatiner.h>
#include <curves/quaternionFn/vufCurveQuaternionParamsFn.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MMatrixArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MGlobal.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufCurveQuatParamNode::g_enable_attr;
MObject	vufCurveQuatParamNode::g_doubles_attr;
MObject	vufCurveQuatParamNode::g_transfoms_attr;
MObject	vufCurveQuatParamNode::g_data_in_attr;
MObject	vufCurveQuatParamNode::g_data_out_attr;

vufCurveQuatParamNode::vufCurveQuatParamNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveQuatParamNode::creator()
{
	return new vufCurveQuatParamNode();
}
MStatus	vufCurveQuatParamNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	//MFnEnumAttribute		l_enum_attr_fn;
	MFnMatrixAttribute		l_matr_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;

	g_enable_attr = l_numeric_attr_fn.create("enable", "ar", MFnNumericData::kBoolean, false, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	l_status = addAttribute(g_enable_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);


	g_doubles_attr = l_typed_attr_fn.create("doubleList", "dl", MFnData::kDoubleArray, MObject::kNullObj, &l_status);
	l_numeric_attr_fn.setWritable(true);
	l_numeric_attr_fn.setStorable(true);
	l_numeric_attr_fn.setHidden(false);
	l_numeric_attr_fn.setKeyable(true);
	//l_numeric_attr_fn.setArray(true);
	l_numeric_attr_fn.setUsesArrayDataBuilder(true);
	//l_numeric_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete);
	l_status = addAttribute(g_doubles_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	g_transfoms_attr = l_typed_attr_fn.create("xformList", "xfr", MFnData::kMatrixArray, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	l_status = addAttribute(g_transfoms_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// in data
	g_data_in_attr = l_typed_attr_fn.create("inCurve", "ic", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	l_status = addAttribute(g_data_in_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);
	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("outCurve", "oc", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);
	l_status = addAttribute(g_data_out_attr); CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_enable_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_doubles_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_transfoms_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr,		g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
MStatus	vufCurveQuatParamNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;

		bool	l_enable = p_data.inputValue(g_enable_attr).asBool();
		std::shared_ptr<vufCurveData>		l_in_data;
		std::shared_ptr<vufCurveData>		l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_data_in_attr, l_in_data);
		// create out data if doesnt exist
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		// if input data is absent then abort evaluation
		if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)
		{
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		vufCurveContainer_4d& l_in_container = *(l_in_data->m_internal_data.get());
		vufCurveContainer_4d& l_out_container = *(l_out_data->m_internal_data.get());

		l_out_container.set_curve_ptr(l_in_container.get_curve_ptr());
		l_out_container.set_rebuild_fn_ptr(l_in_container.get_rebuild_fn_ptr());
		l_out_container.set_scale_fn_ptr(l_in_container.get_scale_fn_ptr());
		l_out_container.set_quaternion_fn_ptr(l_in_container.get_quaternion_fn_ptr());
		//l_out_container.set_remap_fn_ptr(l_in_container.get_remap_fn_ptr());

		if (l_enable == false)
		{
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		//l_out_container.set_remap_fn_ptr(l_in_container.get_remap_fn_ptr());
		MMatrixArray	l_matrix_array;
		MDoubleArray	l_double_array;
		uint32_t		l_transforms_sz;
#pragma region HANDLE_INPUT_TRANSFORM_LIST
//------------------------------------------------------------------------------
// Handle input transform list data
		{
			MDataHandle l_input_data	= p_data.inputValue(g_transfoms_attr);
			MObject		l_in_obj		= l_input_data.data();
			MFnMatrixArrayData l_in_data_fn(l_in_obj, &l_status);
			l_matrix_array				= l_in_data_fn.array(&l_status);

			l_transforms_sz = (uint32_t)l_matrix_array.length();
			if (l_transforms_sz < 2)
			{
				MString l_str = name() + " Wrong Number of transforms. Expects at least two.";
				MGlobal::displayError(l_str);
				p_data.setClean(g_data_out_attr);
				return MS::kSuccess;
			}
		}
#pragma endregion
#pragma region HANDLE_INPUT_DOUBLE_LIST
		{
			MDataHandle l_input_data	= p_data.inputValue(g_doubles_attr);
			MObject		l_in_obj		= l_input_data.data();
			MFnDoubleArrayData l_in_data_fn(l_in_obj, &l_status);
			l_double_array				= l_in_data_fn.array(&l_status);

			uint32_t l_double_sz = (uint32_t)l_double_array.length();
			if (l_double_sz != l_transforms_sz)
			{
				MString l_str = name() + " Wrong doubles array and matrix array has to be the same.";
				MGlobal::displayError(l_str);
				p_data.setClean(g_data_out_attr);
				return MS::kSuccess;
			}
		}
#pragma endregion
		std::shared_ptr < vufCurve<double, vufMath::vufVector4> > l_crv_ptr = l_in_container.get_curve_ptr();
		if (l_crv_ptr == nullptr || l_crv_ptr->is_valid() == false)
		{
			MString l_str = name() + " Invalid Curve.";
			MGlobal::displayError(l_str);
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		l_out_container.switch_quaternion_fn(vufCurveQuatFnType::k_param);
		auto l_quat_ptr = l_out_container.get_quaternion_fn_ptr()->as_params_fn();
		
		l_quat_ptr->set_item_count_i(l_transforms_sz);
		for (uint32_t i = 0; i < l_transforms_sz; ++i)
		{
			double l_val = l_out_container.get_curve_val_by_remaped(l_double_array[i]);
			//std::cout << l_val << std::endl;
			vufMatrix4<double>* l_matr = (vufMatrix4<double>*) & l_matrix_array[i];
			l_quat_ptr->set_item_at_i(i, l_val, *l_matr, l_crv_ptr );
		}
		//VF_LOG_INFO( l_quat_ptr->to_string());
		l_quat_ptr->sort_params_i();
		l_quat_ptr->match_quaternions_i();

		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
