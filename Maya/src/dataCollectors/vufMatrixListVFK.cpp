#include <dataCollectors/vufMatrixListVFK.h>

#include <vufMayaGlobalIncludes.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MAngle.h>

#include <data/vufMayaDataList.h>

#include <math/vufMatrix.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufMatrixListVFK::g_in_data_attr;
MObject	vufMatrixListVFK::g_out_data_attr;
MObject	vufMatrixListVFK::g_in_effectors_attr;
MObject	vufMatrixListVFK::g_t_attr;
MObject	vufMatrixListVFK::g_amount_attr;
MObject	vufMatrixListVFK::g_sigma_a_attr;
MObject	vufMatrixListVFK::g_sigma_b_attr;
MObject vufMatrixListVFK::g_translate_attr;
MObject vufMatrixListVFK::g_rot_compound_attr;
MObject vufMatrixListVFK::g_rx_attr;
MObject vufMatrixListVFK::g_ry_attr;
MObject vufMatrixListVFK::g_rz_attr;
MObject vufMatrixListVFK::g_scale_attr;

MObject vufMatrixListVFK::g_out_parent_attr;
MObject	vufMatrixListVFK::g_out_inverse_attr;
MObject	vufMatrixListVFK::g_outs_attr;


vufMatrixListVFK::vufMatrixListVFK() 
{
	m_gen_id = ++g_unique_id;
}
void*	vufMatrixListVFK::creator()
{
	return new vufMatrixListVFK();
}
void 	vufMatrixListVFK::postConstructor()
{
	setExistWithoutOutConnections(true);
	setExistWithoutInConnections(true);
}
MStatus	vufMatrixListVFK::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	MFnMatrixAttribute		l_matrix_attr_fn;
	MFnUnitAttribute		l_unit_attr_fn;
	// Input Data
	g_in_data_attr = l_typed_attr_fn.create(g_in_mlist_long_s, g_in_mlist_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// input effectors attributes
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_t_attr,		"param",	"prm",	kDouble, 0.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_amount_attr,	"amount",	"amt",	kDouble, 1.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_sigma_a_attr,	"falloffA", "fa",	kDouble, 1.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_sigma_b_attr,	"falloffB", "fb",	kDouble, 1.0);

	g_translate_attr	= l_numeric_attr_fn.create("translate", "tr",	MFnNumericData::k3Double,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_scale_attr		= l_numeric_attr_fn.create("scale", "scl",		MFnNumericData::k3Double,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_rx_attr			= l_unit_attr_fn.create("rotateX", "rx",		MFnUnitAttribute::kAngle,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_ry_attr			= l_unit_attr_fn.create("rotateY", "ry",		MFnUnitAttribute::kAngle,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_rz_attr			= l_unit_attr_fn.create("rotateZ", "rz",		MFnUnitAttribute::kAngle,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	g_rot_compound_attr = l_compound_attr_fn.create("rotation", "r", &l_status);									CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_rx_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_ry_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_rz_attr));
	// input effector compound attribute
	g_in_effectors_attr = l_compound_attr_fn.create("effector", "ef", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_t_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_amount_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_sigma_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_sigma_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_translate_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_rot_compound_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_scale_attr));
	
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setStorable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setWritable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setHidden(false));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setKeyable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setArray(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));
	// out matrix list
	g_out_data_attr = l_typed_attr_fn.create(g_out_mlist_long_s, g_out_mlist_s, mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_typed_attr_fn.setStorable(false));
	CHECK_MSTATUS_AND_RETURN_IT(l_typed_attr_fn.setWritable(false));
	
	// out parent and inverse
	g_out_parent_attr	= l_matrix_attr_fn.create("parent", "prnt", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_out_inverse_attr	= l_matrix_attr_fn.create("compensate", "cmp", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	g_outs_attr = l_compound_attr_fn.create("helpers", "hlp", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_out_parent_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_out_inverse_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setWritable(false));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setStorable(false));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setArray(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setUsesArrayDataBuilder(true));

	// Add Attributes
	l_status = addAttribute(g_in_data_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_in_effectors_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_out_data_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_outs_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// attributes affect
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_out_data_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_outs_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_out_parent_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_out_inverse_attr));

	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_effectors_attr, g_out_data_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_effectors_attr, g_outs_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_effectors_attr, g_out_parent_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_effectors_attr, g_out_inverse_attr));
	return MS::kSuccess;
}
MStatus	vufMatrixListVFK::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_out_data_attr || p_plug == g_outs_attr)
	{
		MStatus l_status;
		std::shared_ptr<vufMatrixListData>	l_out_data;
		std::shared_ptr<vufMatrixListData>	l_in_data;
		// this node change source data. so we make copy
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_out_data_attr, l_out_data, m_gen_id);
		VF_RM_GET_DATA_FROM_IN(mpxMatrixListWrapper, vufMatrixListData, p_data, g_in_data_attr, l_in_data);
		if (l_in_data == nullptr || l_in_data->m_internal_data == nullptr)
		{
			VF_MAYA_NODE_LOG_ERR(" in data is null");
			p_data.setClean(g_out_data_attr);
			p_data.setClean(g_outs_attr);
			return MS::kSuccess;
		}
		VF_CHECK_AND_CREATE_INTERNAL_DATA(l_out_data, vufObjectArray<vufMatrix_4d>);
		auto& l_in_array	= l_in_data->m_internal_data->m_array_v;
		auto& l_out_array	= l_out_data->m_internal_data->m_array_v;
		l_out_array = l_in_array;		
		uint l_xfrms_count	= (uint)l_in_array.size();
		if (l_xfrms_count < 2)
		{
			VF_MAYA_NODE_LOG_ERR(" list has to have items count more than 2");
			p_data.setClean(g_out_data_attr);
			p_data.setClean(g_outs_attr);
			return MS::kSuccess;
		}
		//--------------------------------------------------------------------------
		// read effectors
		MArrayDataHandle l_vh	= p_data.inputValue(g_in_effectors_attr);
		uint l_eff_count			= l_vh.elementCount();
		if (m_eff_arr.size() != l_eff_count) m_eff_arr.resize(l_eff_count);
		for (uint i = 0; i < l_eff_count; ++i)
		{
			MDataHandle l_input_data = l_vh.inputValue();
			m_eff_arr[i].m_t		= l_input_data.child(g_t_attr).asDouble();
			m_eff_arr[i].m_amount	= l_input_data.child(g_amount_attr).asDouble();
			m_eff_arr[i].m_sigma_a	= l_input_data.child(g_sigma_a_attr).asDouble();
			m_eff_arr[i].m_sigma_b	= l_input_data.child(g_sigma_b_attr).asDouble();
			m_eff_arr[i].m_sigma_a *= m_eff_arr[i].m_sigma_a;
			m_eff_arr[i].m_sigma_b *= m_eff_arr[i].m_sigma_b;
			auto l_translate	= l_input_data.child(g_translate_attr).asDouble3();
			m_eff_arr[i].m_tx	= l_translate[0];
			m_eff_arr[i].m_ty	= l_translate[1];
			m_eff_arr[i].m_tz	= l_translate[2];
			auto l_scale		= l_input_data.child(g_scale_attr).asDouble3();
			m_eff_arr[i].m_sx	= l_scale[0];
			m_eff_arr[i].m_sy	= l_scale[1];
			m_eff_arr[i].m_sz	= l_scale[2];
			
			m_eff_arr[i].m_rx = l_input_data.child(g_rx_attr).asAngle().value();
			m_eff_arr[i].m_ry = l_input_data.child(g_ry_attr).asAngle().value();
			m_eff_arr[i].m_rz = l_input_data.child(g_rz_attr).asAngle().value();
			if (!l_vh.next()) break;
		}
		//--------------------------------------------------------------------------
		// compute input matrix list params
		if (m_in_t_arr.size() != l_xfrms_count) m_in_t_arr.resize(l_xfrms_count);
		double l_length = compute_input_matrix_param(l_in_array,l_xfrms_count); // total length
		

		// do main job. modify output array
		vufMatrix_4d l_parent = vufMatrix_4d();
		for (uint i = 0; i < l_xfrms_count; ++i)
		{			
			vufVector_4d l_translate = vufVector_4d();
			vufVector_4d l_dir_x = l_in_array[i].get_axis_x_4().get_normalized();
			vufVector_4d l_dir_y = l_in_array[i].get_axis_y_4().get_normalized();
			vufVector_4d l_dir_z = l_in_array[i].get_axis_z_4().get_normalized();
			vufQuaternion_d l_quat = vufQuaternion_d();
			vufVector_4d l_scale = vufVector_4d();
			for (uint j = 0; j < l_eff_count; ++j)
			{
				double l_w = 0.0;
				double l_t = m_in_t_arr[i].m_t - m_eff_arr[j].m_t;
				if (l_t >= 0)
				{
					l_w = m_eff_arr[j].m_amount * exp(-l_t * l_t / m_eff_arr[j].m_sigma_a);
				}
				if (l_t < 0)
				{
					l_w = m_eff_arr[j].m_amount * exp(-l_t * l_t / m_eff_arr[j].m_sigma_b);
				}
				l_translate += l_w * m_eff_arr[j].m_translate;
				l_quat	+= l_w * m_eff_arr[j].m_quat;
				l_scale += l_w * (m_eff_arr[j].m_scale - vufVector_4d(1.0,1.0,1.0));
			}
			// translate
			l_translate =	l_dir_x * l_translate.x +
							l_dir_y * l_translate.y +
							l_dir_z * l_translate.z;
			l_out_array[i].set_translation(l_translate + l_in_array[i].get_translation_4());
			// scale
			l_out_array[i].set_scale_4(l_out_array[i].get_scale_4() + l_scale);
			//  rotate
			l_quat.normalize_in_place();
			l_out_array[i].set_quaternion(l_in_array[i].get_quaternion() * l_quat);
		}

		// set out for effector root
		if (m_outs_arr.size() != l_eff_count) m_outs_arr.resize(l_eff_count);
		vufVector_4d l_dir_start	= l_out_array[1].get_translation_4() - l_out_array[0].get_translation_4();
		vufVector_4d l_dir_end		= l_out_array.back().get_translation_4()	- l_out_array[l_xfrms_count -2].get_translation_4();
		l_dir_start.normalize_in_place();
		l_dir_end.normalize_in_place();		
		vufVector_4d l_x(1.0);
		for (uint i = 0; i < l_eff_count; ++i)
		{
			m_outs_arr[i].m_parent.set_to_identity_in_place();
			m_outs_arr[i].m_inverse.set_to_identity_in_place();
			if (m_eff_arr[i].m_t <= 0.0)
			{
				// translate
				vufVector_4d l_offset = l_dir_start * (m_eff_arr[i].m_t * l_length) + l_out_array.front().get_translation_4();
				m_outs_arr[i].m_parent.set_translation(l_offset);
				// rotate 
				vufQuaternion_d l_quat		= l_out_array.front().get_quaternion();
				vufVector_4d	l_x2		= l_quat.rotate_vector_by_quaternion(l_x);
				vufQuaternion_d l_quat_incr = vufQuaternion_d::rotate_arc(l_x, l_dir_start);
				m_outs_arr[i].m_parent.set_quaternion(l_quat * l_quat_incr);
				//m_outs_arr[i].m_inverse.set_quaternion(m_eff_arr[i].m_quat.get_conjugated());
				continue;
			}
			if (m_eff_arr[i].m_t >= 1.0)
			{
				//translate
				vufVector_4d l_offset = l_dir_end * ((m_eff_arr[i].m_t -1) * l_length) + l_out_array.back().get_translation_4();
				m_outs_arr[i].m_parent.set_translation(l_offset);
				// rotate 
				vufQuaternion_d l_quat = l_out_array.back().get_quaternion();
				vufVector_4d	l_x2 = l_quat.rotate_vector_by_quaternion(l_x);
				vufQuaternion_d l_quat_incr = vufQuaternion_d::rotate_arc(l_x, l_dir_end);
				m_outs_arr[i].m_parent.set_quaternion(l_quat * l_quat_incr);
				m_outs_arr[i].m_inverse.set_quaternion(m_eff_arr[i].m_quat.get_conjugated());
				continue;
			}
			auto l_indices = find_list_index(m_eff_arr[i].m_t, l_xfrms_count);
			double l_dt = m_in_t_arr[l_indices.m_next].m_t - m_in_t_arr[l_indices.m_prev].m_t; // t(knot next) - t(knot prev)
			double l_cl = m_eff_arr[i].m_t - m_in_t_arr[l_indices.m_prev].m_t; // t(eff) - t(knot prev)
			double l_w2 = l_cl/ l_dt;
			double l_w1 = 1.0 - l_w2;
			//parent translation
			vufVector_4d l_pos = l_out_array[l_indices.m_prev].get_translation_4() * l_w1 +
								 l_out_array[l_indices.m_next].get_translation_4() * l_w2;
			m_outs_arr[i].m_parent.set_translation(l_pos);
			// parent rotation
			vufVector_4d l_dir = l_out_array[l_indices.m_next].get_translation_4() - l_out_array[l_indices.m_prev].get_translation_4();
			l_dir.normalize_in_place();
			vufQuaternion_d l_quat = l_out_array[l_indices.m_prev].get_quaternion() * l_w1 +
									 l_out_array[l_indices.m_next].get_quaternion() * l_w2;
			l_quat.normalize_in_place();
			vufVector_4d l_x(1.0);
			vufVector_4d l_x2 = l_quat.rotate_vector_by_quaternion(l_x);
			vufQuaternion_d l_quat_incr = vufQuaternion_d::rotate_arc(l_x, l_dir);
			m_outs_arr[i].m_parent.set_quaternion(l_quat * l_quat_incr);
		}

		// set outs
		MArrayDataHandle	l_out_handle_array = p_data.outputValue(g_outs_attr);
		MArrayDataBuilder	l_out_array_bldr = l_out_handle_array.builder(&l_status);
		for (uint i = 0; i <l_eff_count; ++i)
		{
			MMatrix& l_parent_matr	= *((MMatrix*)(&m_outs_arr[i].m_parent));
			MMatrix& l_inverse_matr = *((MMatrix*)(&m_outs_arr[i].m_inverse));
			MDataHandle	l_out_hndl = l_out_array_bldr.addElement(i, &l_status);
			l_out_hndl.child(g_out_parent_attr).setMMatrix(l_parent_matr);
			l_out_hndl.child(g_out_inverse_attr).setMMatrix(l_inverse_matr);
		}
		l_out_handle_array.set(l_out_array_bldr);
		l_out_handle_array.setAllClean();
		p_data.setClean(g_out_data_attr);
		p_data.setClean(g_outs_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
MStatus	vufMatrixListVFK::connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	VF_RM_NODE_CONNECT_BROKEN_SIMPLE(mpxMatrixListWrapper, vufMatrixListData, g_in_data_attr);
}
double vufMatrixListVFK::compute_input_matrix_param(const std::vector<vufMath::vufMatrix_4d>& p_in_array, unsigned int p_xfrms_count)
{
	double l_length = 0;
	m_in_t_arr[0].m_l = 0.0;
	m_in_t_arr[0].m_t = 0.0;
	vufVector_4d l_prev = p_in_array[0].get_translation_4();
	for (uint i = 1; i < p_xfrms_count; ++i)
	{
		vufVector_4d l_next = p_in_array[i].get_translation_4();
		l_length += l_next.distance_to(l_prev);
		m_in_t_arr[i].m_l = l_length;
		l_prev = l_next;
	}
	for (uint i = 0; i < p_xfrms_count; ++i)
	{
		m_in_t_arr[i].m_t = m_in_t_arr[i].m_l / l_length;
		m_in_t_arr[i].m_dir_x = p_in_array[i].get_axis_x_4().get_normalized();
		m_in_t_arr[i].m_dir_y = p_in_array[i].get_axis_y_4().get_normalized();
		m_in_t_arr[i].m_dir_z = p_in_array[i].get_axis_z_4().get_normalized();
	}
	return l_length;
}
void	vufMatrixListVFK::compute_out_array(unsigned int p_xfrms_count, unsigned int p_eff_count)
{
	for (uint i = 0; i < p_xfrms_count; ++i)
	{
		vufVector_4d l_translate_l = vufVector_4d();
		vufVector_4d l_translate_w	= vufVector_4d();
		vufVector_4d l_scale_l		= vufVector_4d();
		vufVector_4d l_scale_w		= vufVector_4d();
		vufVector_4d l_unit			= vufVector_4d(1.0, 1.0, 1.0);
		double l_rx_l = 0;
		double l_ry_l = 0;
		double l_rz_l = 0;
		vufQuaternion_d l_quat = vufQuaternion_d();
		for (uint j = 0; j < p_eff_count; ++j)
		{
			double l_w = 0.0;
			double l_t = m_in_t_arr[i].m_t - m_eff_arr[j].m_t;
			if (l_t >= 0)
			{
				l_w = m_eff_arr[j].m_amount * exp(-l_t * l_t / m_eff_arr[j].m_sigma_a);
			}
			if (l_t < 0)
			{
				l_w = m_eff_arr[j].m_amount * exp(-l_t * l_t / m_eff_arr[j].m_sigma_b);
			}
			if (m_eff_arr[i].m_t_space == vufEffectors::k_local)	l_translate_l += l_w * m_eff_arr[j].m_translate_l;
			if (m_eff_arr[i].m_t_space == vufEffectors::k_world)	l_translate_w += l_w * m_eff_arr[j].m_translate_w;
			if (m_eff_arr[i].m_r_space == vufEffectors::k_local)
			{
				l_rx_l += l_w * m_eff_arr[j].m_rx;
				l_ry_l += l_w * m_eff_arr[j].m_ry;
				l_rz_l += l_w * m_eff_arr[j].m_rz;
			}
			if (m_eff_arr[i].m_r_space == vufEffectors::k_world)	l_quat		+= l_w * m_eff_arr[j].m_quat;
			if (m_eff_arr[i].m_s_space == vufEffectors::k_local)	l_scale_l	+= l_w * (m_eff_arr[j].m_scale - vufVector_4d(1.0, 1.0, 1.0));

			if (m_eff_arr[i].m_s_space == vufEffectors::k_world)
				//l_scale += l_w * (m_eff_arr[j].m_scale - l_unit);

			vufVector_4d l_pos =	m_in_t_arr[i].m_dir_x * l_translate_l.x +
									m_in_t_arr[i].m_dir_y * l_translate_l.y +
									m_in_t_arr[i].m_dir_z * l_translate_l.z; // local translate
			l_pos += l_translate_w; // world translate
		}

	}
}
vufMatrixListVFK::vufIndex vufMatrixListVFK::find_list_index(double p_t, unsigned int p_total_count) const
{
	if (p_t <= 0.0) return { 0,0 };
	if (p_t >= 1.0) return { p_total_count-1,p_total_count-1 };
	uint l_prev_index = 0;
	for (uint i = 0; i < m_in_t_arr.size(); ++i)
	{
		if (p_t > m_in_t_arr[i].m_t)
		{
			l_prev_index = i;
		}
	}
	return { l_prev_index , l_prev_index + 1 };
}
