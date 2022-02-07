#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MVector.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MArrayDataHandle.h>

#include <curves/vufCurveBezier3Tangents.h>
#include <vufMathInclude.h>
#include <vufLogMaya.h>


using namespace vufRM;
using namespace vufMath;

MObject vufCurveBezier3Tangents::g_closed_attr;
MObject vufCurveBezier3Tangents::g_inputs_group_attr;
MObject vufCurveBezier3Tangents::g_cntrl_xform_attr;

MObject vufCurveBezier3Tangents::g_mode_attr;
MObject vufCurveBezier3Tangents::g_chord_part_attr;

MObject vufCurveBezier3Tangents::g_out_group_attr;
MObject vufCurveBezier3Tangents::g_out_knot_attr;
MObject vufCurveBezier3Tangents::g_out_tang_a_attr;
MObject vufCurveBezier3Tangents::g_out_tang_b_attr;

void* vufCurveBezier3Tangents::creator()
{
	return new vufCurveBezier3Tangents();
}
MStatus vufCurveBezier3Tangents::initialize()
{
	MStatus l_status;
	//MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	MFnMatrixAttribute		l_matrix_attr_fn;
	//--------------------------------------------------------
	//create attibutes
	//I N T P U T S
	//--------------------------------------------------------
	g_closed_attr = l_numeric_attr_fn.create("closed", "cls", MFnNumericData::kBoolean,false,&l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setStorable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setChannelBox(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setDefault(false));
	//--------------------------------------------------------
	// mode enum
	//--------------------------------------------------------	
	g_mode_attr = l_enum_attr_fn.create("mode", "md", 0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("auto",		0));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("simple",	1));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("arc",		2));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("pass",		3));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setDefault(0));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setChannelBox(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setStorable(true));
	//--------------------------------------------------------
	g_chord_part_attr = l_numeric_attr_fn.create("chord", "chr", MFnNumericData::kDouble);
	l_numeric_attr_fn.setStorable(true);
	l_numeric_attr_fn.setDefault(.333);	
	//--------------------------------------------------------
	g_cntrl_xform_attr = l_matrix_attr_fn.create("xForm", "xfr", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_matrix_attr_fn.setStorable(true));
	//--------------------------------------------------------
	g_inputs_group_attr = l_compound_attr_fn.create("inputGroup", "ig", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_mode_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_chord_part_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_cntrl_xform_attr));
	
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setStorable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setWritable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setHidden(false));
	CHECK_MSTATUS_AND_RETURN_IT(l_numeric_attr_fn.setKeyable(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setArray(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setUsesArrayDataBuilder(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_matrix_attr_fn.setDisconnectBehavior(MFnAttribute::kDelete));

	//--------------------------------------------------------
	//create attibutes
	//outPuts
	//--------------------------------------------------------
	g_out_tang_a_attr = l_matrix_attr_fn.create("tangA", "txA", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_matrix_attr_fn.setStorable(false));
	//--------------------------------------------------------
	g_out_knot_attr = l_matrix_attr_fn.create("knot", "kn", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_matrix_attr_fn.setStorable(false));
	//--------------------------------------------------------
	g_out_tang_b_attr = l_matrix_attr_fn.create("tangB", "txB", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_matrix_attr_fn.setStorable(false));
	//--------------------------------------------------------
	g_out_group_attr = l_compound_attr_fn.create("outputGroup", "og", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_out_tang_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_out_tang_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_out_knot_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setStorable(false));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setWritable(false));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setArray(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setUsesArrayDataBuilder(true));
	//l_compound_attr_fn.setCached(false);
	//--------------------------------------------------------	
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_closed_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_inputs_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_out_group_attr));
	//--------------------------------------------------------
	//Attributes  Affects		
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr,			g_out_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr,	g_out_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_mode_attr,			g_out_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_chord_part_attr,		g_out_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_cntrl_xform_attr,	g_out_group_attr));

	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr,			g_out_knot_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr,	g_out_knot_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_mode_attr,			g_out_knot_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_chord_part_attr,		g_out_knot_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_cntrl_xform_attr,	g_out_knot_attr));

	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr,			g_out_tang_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr,	g_out_tang_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_mode_attr,			g_out_tang_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_chord_part_attr,		g_out_tang_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_cntrl_xform_attr,	g_out_tang_a_attr));
	
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr,			g_out_tang_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr,	g_out_tang_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_mode_attr,			g_out_tang_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_chord_part_attr,		g_out_tang_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_cntrl_xform_attr,	g_out_tang_b_attr));

	return MS::kSuccess;
}
MStatus	vufCurveBezier3Tangents::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus l_status;
	if (plug == g_out_tang_a_attr ||
		plug == g_out_tang_b_attr ||
		plug == g_out_group_attr ||
		plug == g_out_knot_attr )
	{
		m_closed = data.inputValue(g_closed_attr).asBool();

		MArrayDataHandle l_vh = data.inputValue(g_inputs_group_attr);
		int l_knots_count	= l_vh.elementCount();
		if (m_knots_array.size() != l_knots_count)
		{
			m_knots_array.resize(l_knots_count);
		}
		for (int i = 0; i < l_knots_count; ++i)//read nodes
		{
			MDataHandle l_input_data		= l_vh.inputValue().child(g_inputs_group_attr);
			m_knots_array[i].m_mode			= l_input_data.child(g_mode_attr).asShort();
			m_knots_array[i].m_chord_length = l_input_data.child(g_chord_part_attr).asDouble();
			m_knots_array[i].m_knot			= vufMatrix_4d::cast_from<MMatrix>(l_input_data.child(g_cntrl_xform_attr).asMatrix());
			if (!l_vh.next()) break;	
		}
		
		for (int i = 1; i < (l_knots_count - 1); ++i)
		{
			if (m_knots_array[i].m_mode == k_pass)
			{
				m_knots_array[i].m_tangent_a = m_knots_array[i].m_knot;
				m_knots_array[i].m_tangent_b = m_knots_array[i].m_knot;
				continue;
			}
			if (m_knots_array[i].m_mode == k_auto)
			{
				auto_tangent(m_knots_array[i - 1], m_knots_array[i + 1], m_knots_array[i]);
			}
			if (m_knots_array[i].m_mode == k_simple)
			{
				simple_tangent(m_knots_array[i - 1], m_knots_array[i + 1], m_knots_array[i]);
			}
			if (m_knots_array[i].m_mode == k_arc)
			{
				arc_tangent(m_knots_array[i - 1], m_knots_array[i + 1], m_knots_array[i]);
			}
		}
		if (m_closed == true)
		{
			if (m_knots_array.front().m_mode == k_pass)
			{
				m_knots_array.front().m_tangent_a = m_knots_array.front().m_knot;
				m_knots_array.front().m_tangent_b = m_knots_array.front().m_knot;
			}
			if (m_knots_array.back().m_mode == k_pass)
			{
				m_knots_array.back().m_tangent_a = m_knots_array.back().m_knot;
				m_knots_array.back().m_tangent_b = m_knots_array.back().m_knot;
			}

			if (m_knots_array.front().m_mode == k_auto)
			{
				auto_tangent(m_knots_array.back(), m_knots_array[1], m_knots_array.front());
			}
			if (m_knots_array.back().m_mode == k_auto)
			{
				auto_tangent(m_knots_array[l_knots_count-2], m_knots_array[0], m_knots_array.back());
			}

			if (m_knots_array.front().m_mode == k_simple)
			{
				simple_tangent(m_knots_array.back(), m_knots_array[1], m_knots_array.front());
			}
			if (m_knots_array.back().m_mode == k_simple)
			{
				simple_tangent(m_knots_array[l_knots_count - 2], m_knots_array[0], m_knots_array.back());
			}

		}
		else
		{
			if (m_knots_array.front().m_mode	== k_pass)
			{
				m_knots_array.front().m_tangent_a = m_knots_array.front().m_knot;
				m_knots_array.front().m_tangent_b = m_knots_array.front().m_knot;
			}
			if (m_knots_array.back().m_mode		== k_pass)
			{
				m_knots_array.back().m_tangent_a = m_knots_array.back().m_knot;
				m_knots_array.back().m_tangent_b = m_knots_array.back().m_knot;
			}
			
			if (m_knots_array.front().m_mode	== k_auto)
			{
				auto_tangent_first_knot(m_knots_array[1], m_knots_array.front());
			}
			if (m_knots_array.back().m_mode		== k_auto)
			{
				auto_tangent_last_knot(m_knots_array[l_knots_count -2], m_knots_array.back());
			}
			
			if (m_knots_array.front().m_mode	== k_simple)
			{
				simple_tangent_first_knot(m_knots_array[1], m_knots_array.front());
			}
			if (m_knots_array.back().m_mode		== k_simple)
			{
				simple_tangent_last_knot(m_knots_array[l_knots_count - 2], m_knots_array.back());
			}
		}
		
		//setOutputs
		MArrayDataHandle l_out_array = data.outputValue(g_out_group_attr);
		MArrayDataBuilder l_blder = l_out_array.builder(&l_status);
		for (int i = 0; i < l_knots_count; ++i)
		{
			MDataHandle l_out_handle = l_blder.addElement(i, &l_status);
			if (l_status == MS::kSuccess)
			{

				MMatrix l_m = vufMatrix_4d::cast_to<MMatrix>(m_knots_array[i].m_knot);
				MMatrix l_a = vufMatrix_4d::cast_to<MMatrix>(m_knots_array[i].m_tangent_a);
				MMatrix l_b = vufMatrix_4d::cast_to<MMatrix>(m_knots_array[i].m_tangent_b);
				auto l_mi = l_m.inverse();
				l_a *= l_mi;// * aPtr[i].tm1;
				l_b *= l_mi;// * aPtr[i].tm2;			

				l_out_handle.child(g_out_knot_attr).setMMatrix(l_m);
				l_out_handle.child(g_out_tang_a_attr).setMMatrix(l_a);
				l_out_handle.child(g_out_tang_b_attr).setMMatrix(l_b);
			}
		}
		l_out_array.set(l_blder);
		l_out_array.setAllClean();
		data.setClean(g_out_group_attr);
		data.setClean(plug);
		return MS::kSuccess;

	}
	return MS::kUnknownParameter;
}
void	vufCurveBezier3Tangents::auto_tangent(const vufKnotInfo& p_prev, const vufKnotInfo& p_next, vufKnotInfo& p_curr)
{
	p_curr.m_tangent_a = p_curr.m_knot;
	p_curr.m_tangent_b = p_curr.m_knot;
	vufVector_4d l_pos_prev = p_prev.m_knot.get_translation_4();
	vufVector_4d l_pos_next = p_next.m_knot.get_translation_4();
	vufVector_4d l_pos_curr = p_curr.m_knot.get_translation_4();

	auto l_dir_next = l_pos_next - l_pos_curr;
	auto l_dir_prev = l_pos_prev - l_pos_curr;
	auto l_dir_next_norm = l_dir_next.get_normalized();
	auto l_dir_prev_norm = l_dir_prev.get_normalized();
	if ( l_dir_next_norm.get_cross(l_dir_prev_norm).length2() < VF_MATH_EPSILON )
	{		
		p_curr.m_tangent_a.set_translation( l_pos_curr	+ l_dir_prev * p_curr.m_chord_length);
		p_curr.m_tangent_b.set_translation( l_pos_curr	+ l_dir_next * p_curr.m_chord_length);
		return;
	}
	auto l_dir = l_dir_next_norm - l_dir_prev_norm;
	l_dir.normalize_in_place();
	double l_lngth_next = l_dir.dot(l_dir_next) * p_curr.m_chord_length;
	double l_lngth_prev = l_dir.dot(l_dir_prev) * p_curr.m_chord_length;

	p_curr.m_tangent_a.set_translation(l_pos_curr + l_dir * l_lngth_next);
	p_curr.m_tangent_b.set_translation(l_pos_curr + l_dir * l_lngth_prev);
}
void	vufCurveBezier3Tangents::auto_tangent_first_knot(const vufKnotInfo& p_next, vufKnotInfo& p_current)
{
	p_current.m_tangent_a		= p_current.m_knot;
	p_current.m_tangent_b		= p_current.m_knot;
	vufVector_4d l_next_tngnt	= p_next.m_tangent_b.get_translation_4();
	vufVector_4d l_pos_cur		= p_current.m_knot.get_translation_4();
	vufVector_4d l_pos_tang		= l_pos_cur + (l_next_tngnt - l_pos_cur)* p_current.m_chord_length;
	p_current.m_tangent_a.set_translation(l_pos_tang);
	p_current.m_tangent_b.set_translation(2.0 * l_pos_cur - l_pos_tang);
}
void	vufCurveBezier3Tangents::auto_tangent_last_knot(const vufKnotInfo& p_prev, vufKnotInfo& p_current)
{
	p_current.m_tangent_a		= p_current.m_knot;
	p_current.m_tangent_b		= p_current.m_knot;
	vufVector_4d l_prev_tngnt	= p_prev.m_tangent_a.get_translation_4();
	vufVector_4d l_pos_cur		= p_current.m_knot.get_translation_4();
	vufVector_4d l_pos_tang		= l_pos_cur + (l_prev_tngnt - l_pos_cur) *p_current.m_chord_length;
	p_current.m_tangent_b.set_translation(l_pos_tang);
	p_current.m_tangent_a.set_translation(2.0 * l_pos_cur - l_pos_tang);
}

void	vufCurveBezier3Tangents::simple_tangent(const vufKnotInfo& p_prev, const vufKnotInfo& p_next, vufKnotInfo& p_curr)
{
	p_curr.m_tangent_a = p_curr.m_knot;
	p_curr.m_tangent_b = p_curr.m_knot;
	vufVector_4d l_pos_prev = p_prev.m_knot.get_translation_4();
	vufVector_4d l_pos_next = p_next.m_knot.get_translation_4();
	vufVector_4d l_pos_curr = p_curr.m_knot.get_translation_4();

	auto l_dir_next = l_pos_next - l_pos_curr;
	auto l_dir_prev = l_pos_prev - l_pos_curr;
	double l_lngth_next = l_dir_next.length() * p_curr.m_chord_length;
	double l_lngth_prev = l_dir_prev.length() * p_curr.m_chord_length;

	auto l_dir_x = p_curr.m_knot.get_axis_x_4();
	l_dir_x.normalize_in_place();
	p_curr.m_tangent_a.set_translation(l_pos_curr + l_dir_x * l_lngth_next);
	p_curr.m_tangent_b.set_translation(l_pos_curr - l_dir_x * l_lngth_prev);
}
void vufCurveBezier3Tangents::simple_tangent_first_knot(const vufKnotInfo& p_next, vufKnotInfo& p_curr)
{
	p_curr.m_tangent_a = p_curr.m_knot;
	p_curr.m_tangent_b = p_curr.m_knot;
	vufVector_4d l_pos_next = p_next.m_knot.get_translation_4();
	vufVector_4d l_pos_curr = p_curr.m_knot.get_translation_4();
	
	auto l_dir_next = l_pos_next - l_pos_curr;
	double l_lngth	= l_dir_next.length() * p_curr.m_chord_length;
	auto l_dir_x = p_curr.m_knot.get_axis_x_4();
	l_dir_x.normalize_in_place();
	p_curr.m_tangent_a.set_translation(l_pos_curr + l_dir_x * l_lngth);
	p_curr.m_tangent_b.set_translation(l_pos_curr - l_dir_x * l_lngth);
}
void vufCurveBezier3Tangents::simple_tangent_last_knot(const vufKnotInfo& p_prev, vufKnotInfo& p_curr)
{
	p_curr.m_tangent_a = p_curr.m_knot;
	p_curr.m_tangent_b = p_curr.m_knot;
	vufVector_4d l_pos_next = p_prev.m_knot.get_translation_4();
	vufVector_4d l_pos_curr = p_curr.m_knot.get_translation_4();

	auto l_dir_next = l_pos_next - l_pos_curr;
	double l_lngth = l_dir_next.length() * p_curr.m_chord_length;
	auto l_dir_x = p_curr.m_knot.get_axis_x_4();
	l_dir_x.normalize_in_place();
	p_curr.m_tangent_b.set_translation(l_pos_curr - l_dir_x * l_lngth);
	p_curr.m_tangent_a.set_translation(l_pos_curr + l_dir_x * l_lngth);
}


void	vufCurveBezier3Tangents::arc_tangent(const vufKnotInfo& p_prev, const vufKnotInfo& p_next, vufKnotInfo& p_current)
{

}
