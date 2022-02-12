#include <dataCollectors/vufMatrixListVFK.h>

#include <vufMayaGlobalIncludes.h>

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>

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

MObject	vufMatrixListVFK::g_max_t_attr;
MObject	vufMatrixListVFK::g_in_effectors_attr;
MObject	vufMatrixListVFK::g_t_attr;
MObject	vufMatrixListVFK::g_amount_attr;
MObject	vufMatrixListVFK::g_sigma_a_attr;
MObject	vufMatrixListVFK::g_cnst_a_attr;
MObject	vufMatrixListVFK::g_sigma_b_attr;
MObject	vufMatrixListVFK::g_cnst_b_attr;
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
	// ramp attribute
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_max_t_attr,	"scaleParam", "sp",	kDouble, 1.0);
	l_numeric_attr_fn.setMin(1.0);
	// input effectors attributes
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_t_attr,		"param",	"prm",	kDouble, 0.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_amount_attr,	"amount",	"amt",	kDouble, 1.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_sigma_a_attr,	"falloffA", "fa",	kDouble, 1.0);
	l_numeric_attr_fn.setMin(0.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_cnst_a_attr,	"constA",	"cna",	kDouble, 0.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_sigma_b_attr,	"falloffB", "fb",	kDouble, 1.0);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_cnst_b_attr,	"constB",	"cnb",	kDouble, 0.0);
	l_numeric_attr_fn.setMin(0.0);


	g_scale_attr		= l_numeric_attr_fn.create(	"scale", "scl",		MFnNumericData::k3Double,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_rx_attr			= l_unit_attr_fn.create(	"rotateX", "rx",	MFnUnitAttribute::kAngle,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_ry_attr			= l_unit_attr_fn.create(	"rotateY", "ry",	MFnUnitAttribute::kAngle,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_rz_attr			= l_unit_attr_fn.create(	"rotateZ", "rz",	MFnUnitAttribute::kAngle,0.0, &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	g_rot_compound_attr = l_compound_attr_fn.create("rotate", "r", &l_status);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_rx_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_ry_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_rz_attr));
	// input effector compound attribute
	g_in_effectors_attr = l_compound_attr_fn.create("effector", "ef", &l_status);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_t_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_amount_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_sigma_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_cnst_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_sigma_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_cnst_b_attr));
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
	l_status = addAttribute(g_max_t_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_in_effectors_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_out_data_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_outs_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// attributes affect
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_out_data_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_outs_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_out_parent_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_in_data_attr, g_out_inverse_attr));

	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_max_t_attr, g_out_data_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_max_t_attr, g_outs_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_max_t_attr, g_out_parent_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_max_t_attr, g_out_inverse_attr));

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
		//vuf::vufTimer l_timer("Matrix VFK node compute: ");

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

		m_scale_t = p_data.inputValue(g_max_t_attr).asDouble();
		//--------------------------------------------------------------------------
		// read effectors
		MArrayDataHandle l_vh	= p_data.inputValue(g_in_effectors_attr);
		uint l_eff_count		= l_vh.elementCount();
		if (m_eff_arr.size() != l_eff_count) m_eff_arr.resize(l_eff_count);
		for (uint i = 0; i < l_eff_count; ++i)
		{
			MDataHandle l_input_data = l_vh.inputValue();
			m_eff_arr[i].m_t		= l_input_data.child(g_t_attr).asDouble();
			m_eff_arr[i].m_amount	= l_input_data.child(g_amount_attr).asDouble();
			m_eff_arr[i].m_sigma_a	= l_input_data.child(g_sigma_a_attr).asDouble();
			m_eff_arr[i].m_const_a	= l_input_data.child(g_cnst_a_attr).asDouble();
			m_eff_arr[i].m_sigma_b	= l_input_data.child(g_sigma_b_attr).asDouble();
			m_eff_arr[i].m_const_b	= l_input_data.child(g_cnst_b_attr).asDouble();
			m_eff_arr[i].m_sigma_a *= m_eff_arr[i].m_sigma_a;
			m_eff_arr[i].m_sigma_b *= m_eff_arr[i].m_sigma_b;
			auto l_scale		= l_input_data.child(g_scale_attr).asDouble3();
			m_eff_arr[i].m_scale.x = l_scale[0];
			m_eff_arr[i].m_scale.y = l_scale[1];
			m_eff_arr[i].m_scale.z = l_scale[2];
			m_eff_arr[i].m_rx = l_input_data.child(g_rot_compound_attr).child(g_rx_attr).asAngle().asRadians();
			m_eff_arr[i].m_ry = l_input_data.child(g_rot_compound_attr).child(g_ry_attr).asAngle().asRadians();
			m_eff_arr[i].m_rz = l_input_data.child(g_rot_compound_attr).child(g_rz_attr).asAngle().asRadians();
			if (!l_vh.next()) break;
		}
		//--------------------------------------------------------------------------
		// compute input matrix list params
		if (m_in_t_arr.size() != l_xfrms_count) m_in_t_arr.resize(l_xfrms_count);
		double l_length = compute_input_matrix_param(l_in_array); // total length
		//--------------------------------------------------------------------------
		// do main job. modify output array
		compute_out_array(l_xfrms_count, l_eff_count, l_in_array, l_out_array);

		// set out for effector root
		if (m_outs_arr.size() != l_eff_count) m_outs_arr.resize(l_eff_count);
		vufVector_4d l_dir_start	= l_out_array[1].get_translation_4()		- l_out_array[0].get_translation_4();
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
				vufVector_4d l_offset = l_dir_start * (m_eff_arr[i].m_t * l_length/m_scale_t) + l_out_array.front().get_translation_4();
				m_outs_arr[i].m_parent.set_translation(l_offset);
				// rotate 
				vufQuaternion_d l_quat		= m_in_t_arr.front().m_quat_w;
				vufVector_4d	l_x2		= l_quat.rotate_vector_by_quaternion(l_x);
				vufQuaternion_d l_quat_incr = vufQuaternion_d::rotate_arc(l_x2, l_dir_start);
				m_outs_arr[i].m_parent.set_quaternion(l_quat * l_quat_incr);
				vufQuaternion_d l_quat_x_l(m_eff_arr[i].m_rx, m_outs_arr.front().m_parent.get_axis_x_4());
				vufQuaternion_d l_quat_y_l(m_eff_arr[i].m_ry, m_outs_arr.front().m_parent.get_axis_y_4());
				vufQuaternion_d l_quat_z_l(m_eff_arr[i].m_rz, m_outs_arr.front().m_parent.get_axis_z_4());
				vufQuaternion_d l_quat_effectors = l_quat_z_l * l_quat_y_l * l_quat_x_l;
				m_outs_arr[i].m_inverse.set_quaternion(l_quat_effectors);
				continue;
			}
			if (m_eff_arr[i].m_t >= m_scale_t)
			{
				//translate
				vufVector_4d l_offset = l_dir_end * ((m_eff_arr[i].m_t - m_scale_t) * l_length/m_scale_t) + l_out_array.back().get_translation_4();
				m_outs_arr[i].m_parent.set_translation(l_offset);
				// rotate 
				vufQuaternion_d l_quat = m_in_t_arr.back().m_quat_w;
				vufVector_4d	l_x2 = l_quat.rotate_vector_by_quaternion(l_x);
				vufQuaternion_d l_quat_incr = vufQuaternion_d::rotate_arc(l_x2, l_dir_end);
				m_outs_arr[i].m_parent.set_quaternion(l_quat * l_quat_incr);
				vufQuaternion_d l_quat_x_l(m_eff_arr[i].m_rx, m_outs_arr.back().m_parent.get_axis_x_4());
				vufQuaternion_d l_quat_y_l(m_eff_arr[i].m_ry, m_outs_arr.back().m_parent.get_axis_y_4());
				vufQuaternion_d l_quat_z_l(m_eff_arr[i].m_rz, m_outs_arr.back().m_parent.get_axis_z_4());
				vufQuaternion_d l_quat_effectors = l_quat_z_l * l_quat_y_l * l_quat_x_l;
				m_outs_arr[i].m_inverse.set_quaternion(l_quat_effectors);
				continue;
			}
			unsigned int l_prev_ndx = m_eff_arr[i].m_prev;
			unsigned int l_next_ndx = m_eff_arr[i].m_next;
			//parent translation
			vufVector_4d l_pos_prev = l_out_array[l_prev_ndx].get_translation_4();
			vufVector_4d l_pos_next = l_out_array[l_next_ndx].get_translation_4();
			vufVector_4d l_offset = l_pos_prev * m_eff_arr[i].m_w_1 +
									l_pos_next * m_eff_arr[i].m_w_2;
			m_outs_arr[i].m_parent.set_translation(l_offset);
			// parent rotation
			vufQuaternion_d l_quat_1 = m_in_t_arr[l_prev_ndx].m_quat_w;
			vufQuaternion_d l_quat_2 = m_in_t_arr[l_next_ndx].m_quat_w;
			if (l_quat_1.dot(l_quat_2) < 0)
			{
				l_quat_2 = -l_quat_2;
			}
			vufQuaternion_d l_quat = l_quat_1 * m_eff_arr[i].m_w_1 +
									 l_quat_2 * m_eff_arr[i].m_w_2;
			l_quat.normalize_in_place();
			vufVector_4d	l_dir = l_pos_next - l_pos_prev;
			vufVector_4d	l_x2 = l_quat.rotate_vector_by_quaternion(l_x);
			vufQuaternion_d l_quat_incr = vufQuaternion_d::rotate_arc(l_x2, l_dir);
			m_outs_arr[i].m_parent.set_quaternion(l_quat * l_quat_incr);

			vufQuaternion_d l_quat_x_l(m_eff_arr[i].m_rx, m_outs_arr.front().m_parent.get_axis_x_4());
			vufQuaternion_d l_quat_y_l(m_eff_arr[i].m_ry, m_outs_arr.front().m_parent.get_axis_y_4());
			vufQuaternion_d l_quat_z_l(m_eff_arr[i].m_rz, m_outs_arr.front().m_parent.get_axis_z_4());
			vufQuaternion_d l_quat_effectors = l_quat_z_l * l_quat_y_l * l_quat_x_l;
			m_outs_arr[i].m_inverse.set_quaternion(l_quat_effectors);
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
double	vufMatrixListVFK::compute_input_matrix_param(const std::vector<vufMath::vufMatrix_4d>& p_in_array )
{
	double l_length = 0;
	unsigned int l_xforms_count = (unsigned int)p_in_array.size();
	unsigned int l_eff_count = (unsigned int)m_eff_arr.size();
	{
		m_in_t_arr[0].m_l = 0.0;
		m_in_t_arr[0].m_t = 0.0;
		m_in_t_arr[0].m_dir_x = p_in_array[0].get_axis_x_4();
		m_in_t_arr[0].m_dir_y = p_in_array[0].get_axis_y_4();
		m_in_t_arr[0].m_dir_z = p_in_array[0].get_axis_z_4();
		m_in_t_arr[0].m_scale_origin = vufVector_4d(m_in_t_arr[0].m_dir_x.length(),
													m_in_t_arr[0].m_dir_y.length(),
													m_in_t_arr[0].m_dir_z.length());
		m_in_t_arr[0].m_quat_w  = p_in_array[0].get_quaternion();
		m_in_t_arr[0].m_dir_x_n = m_in_t_arr[0].m_dir_x.get_normalized();
		m_in_t_arr[0].m_dir_y_n = m_in_t_arr[0].m_dir_y.get_normalized();
		m_in_t_arr[0].m_dir_z_n = m_in_t_arr[0].m_dir_z.get_normalized();
	}

	vufVector_4d l_prev = p_in_array[0].get_translation_4();
	for (uint i = 1; i < l_xforms_count; ++i)
	{
		// accumate lenght for each knot
		vufVector_4d l_next = p_in_array[i].get_translation_4();
		l_length += l_next.distance_to(l_prev);
		m_in_t_arr[i].m_l = l_length;
		{
			m_in_t_arr[i].m_dir_x = p_in_array[i].get_axis_x_4();
			m_in_t_arr[i].m_dir_y = p_in_array[i].get_axis_y_4();
			m_in_t_arr[i].m_dir_z = p_in_array[i].get_axis_z_4();
			m_in_t_arr[i].m_scale_origin = vufVector_4d(m_in_t_arr[i].m_dir_x.length(),
														m_in_t_arr[i].m_dir_y.length(),
														m_in_t_arr[i].m_dir_z.length());
			m_in_t_arr[i].m_dir_x_n = m_in_t_arr[i].m_dir_x.get_normalized();
			m_in_t_arr[i].m_dir_y_n = m_in_t_arr[i].m_dir_y.get_normalized();
			m_in_t_arr[i].m_dir_z_n = m_in_t_arr[i].m_dir_z.get_normalized();
			vufVector_4d l_rel = l_next - l_prev;
			//double l_dist = l_rel.length();
			m_in_t_arr[i].m_coord_p = vufVector_4d( l_rel.dot(m_in_t_arr[i - 1].m_dir_x_n),
													l_rel.dot(m_in_t_arr[i - 1].m_dir_y_n),
													l_rel.dot(m_in_t_arr[i - 1].m_dir_z_n));
			m_in_t_arr[i].m_quat_w = p_in_array[i].get_quaternion();
			m_in_t_arr[i].m_quat_r = m_in_t_arr[i].m_quat_w * m_in_t_arr[i - 1].m_quat_w.get_conjugated();
		}
		l_prev = l_next;
	}
	for (uint i = 1; i < l_xforms_count; ++i)
	{
		// compuite param for each knot
		m_in_t_arr[i].m_t = m_scale_t * (m_in_t_arr[i].m_l / l_length);
	}
	// for each effector compute prev next indices
	for (uint i = 0; i < l_eff_count; ++i)
	{
		double l_t = m_eff_arr[i].m_t;
		double l_prev_index;
		if (l_t <= 0.0) 
		{
			m_eff_arr[i].m_prev = 0;
			m_eff_arr[i].m_next = 0;
			m_eff_arr[i].m_w_1 = 1.0;
			m_eff_arr[i].m_w_2 = 0.0;
		};
		if (l_t >= m_scale_t)
		{
			m_eff_arr[i].m_prev = l_xforms_count - 1;
			m_eff_arr[i].m_next = l_xforms_count - 1;
			m_eff_arr[i].m_w_1 = 1.0;
			m_eff_arr[i].m_w_2 = 0.0;
		}
		for (uint j = 0; j < m_in_t_arr.size(); ++j)
		{
			if (l_t > m_in_t_arr[j].m_t)
			{
				l_prev_index = j;
			}
			double l_dt = m_in_t_arr[l_prev_index + 1].m_t - m_in_t_arr[l_prev_index].m_t;
			m_eff_arr[i].m_w_2 = (m_eff_arr[i].m_t - m_in_t_arr[l_prev_index].m_t) / l_dt;
			m_eff_arr[i].m_w_1 = 1.0 - m_eff_arr[i].m_w_2;
		}
		m_eff_arr[i].m_prev = l_prev_index;
		m_eff_arr[i].m_next = l_prev_index + 1;
	}
	return l_length;
}
void	vufMatrixListVFK::compute_out_array(unsigned int p_xfrms_count, unsigned int p_eff_count,
											const std::vector<vufMath::vufMatrix_4d>& p_in_array,
											std::vector<vufMath::vufMatrix_4d>& p_out_array)
{
	vufVector_4d l_unit = vufVector_4d(1.0, 1.0, 1.0);
	for (uint i = 0; i < p_xfrms_count; ++i)
	{
		vufVector_4d l_scale		= vufVector_4d();
		vufVector_4d l_unit			= vufVector_4d(1.0, 1.0, 1.0);
		double l_rx_l = 0;
		double l_ry_l = 0;
		double l_rz_l = 0;
		vufQuaternion_d l_quat_w = vufQuaternion_d();
		vufQuaternion_d l_quat_l = vufQuaternion_d();
		for (uint j = 0; j < p_eff_count; ++j)
		{
			double l_w = 0.0;
			double l_delta_t = m_in_t_arr[i].m_t - m_eff_arr[j].m_t;
			if (l_delta_t >= 0)
			{
				l_w = 1;
				if (l_delta_t > m_eff_arr[j].m_const_a)
				{
					double l_x = l_delta_t - m_eff_arr[j].m_const_a;
					l_w = m_eff_arr[j].m_amount * exp(-l_x * l_x / m_eff_arr[j].m_sigma_a);
				}
			}
			if (l_delta_t < 0)
			{
				l_w = 1;
				if (l_delta_t < -m_eff_arr[j].m_const_a)
				{
					double l_x = l_delta_t + m_eff_arr[j].m_const_a;
					l_w = m_eff_arr[j].m_amount * exp(-l_x * l_x / m_eff_arr[j].m_sigma_a);
				}
				l_w = m_eff_arr[j].m_amount * exp(-l_delta_t * l_delta_t / m_eff_arr[j].m_sigma_b);
			}
			
			l_rx_l += l_w * m_eff_arr[j].m_rx;
			l_ry_l += l_w * m_eff_arr[j].m_ry;
			l_rz_l += l_w * m_eff_arr[j].m_rz;
			l_scale += l_w * (m_eff_arr[j].m_scale - l_unit);
		}
		// translate
		if (i == 0)
		{
			vufQuaternion_d l_quat_x_l(l_rx_l, m_in_t_arr[i].m_dir_x);
			vufQuaternion_d l_quat_y_l(l_ry_l, m_in_t_arr[i].m_dir_y);
			vufQuaternion_d l_quat_z_l(l_rz_l, m_in_t_arr[i].m_dir_z);
			vufQuaternion_d l_quat_effectors	= l_quat_z_l * l_quat_y_l * l_quat_x_l;
			vufQuaternion_d l_quat_new			= m_in_t_arr[i].m_quat_w * l_quat_effectors;
			p_out_array[i].set_quaternion(l_quat_new);
			p_out_array[i].set_scale_4(p_in_array[i].get_scale_4() + l_scale);
			m_in_t_arr[i].m_quat_w = l_quat_new;
			continue;
	 	}
		//  rotate
		//l_quat_w.normalize_in_place();
		vufQuaternion_d l_quat_x_l(l_rx_l, m_in_t_arr[i].m_dir_x);
		vufQuaternion_d l_quat_y_l(l_ry_l, m_in_t_arr[i].m_dir_y);
		vufQuaternion_d l_quat_z_l(l_rz_l, m_in_t_arr[i].m_dir_z);
		vufQuaternion_d l_quat_effectors = l_quat_z_l* l_quat_y_l* l_quat_x_l;
		vufQuaternion_d l_quat_new = m_in_t_arr[i - 1].m_quat_w * m_in_t_arr[i].m_quat_r;
		l_quat_new *= l_quat_effectors;
		p_out_array[i].set_quaternion(l_quat_new);
		vufVector_4d l_pos =	m_in_t_arr[i].m_coord_p.x * p_out_array[i - 1].get_axis_x_4().get_normalized() +
								m_in_t_arr[i].m_coord_p.y * p_out_array[i - 1].get_axis_y_4().get_normalized() +
								m_in_t_arr[i].m_coord_p.z * p_out_array[i - 1].get_axis_z_4().get_normalized();
		m_in_t_arr[i].m_quat_w = l_quat_new;
		l_pos += p_out_array[i - 1].get_translation_4();
		p_out_array[i].set_translation( l_pos);
		//p_out_array[i].set_translation(vufVector_4d(0.0,1.0) + l_rel_pos);
		
		// scale
		p_out_array[i].set_scale_4(p_in_array[i].get_scale_4() + l_scale);
		
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
