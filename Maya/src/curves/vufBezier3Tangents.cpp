#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MVector.h>
#include <curves/vufBezier3Tangents.h>

using namespace vufRM;

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
	MFnTypedAttribute		l_typed_attr_fn;
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
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("pass", 0));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("auto", 1));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("simple", 2));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.addField("arc", 4));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setDefault(0));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setChannelBox(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_enum_attr_fn.setStorable(true));
	//--------------------------------------------------------
	g_chord_part_attr = l_numeric_attr_fn.create("chordPart", "chp", MFnNumericData::kDouble);
	l_numeric_attr_fn.setStorable(true);
	l_numeric_attr_fn.setDefault(.333);	
	//--------------------------------------------------------
	g_cntrl_xform_attr = l_matrix_attr_fn.create("cntrlXForm", "cxf", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_matrix_attr_fn.setStorable(true));
	//--------------------------------------------------------
	g_inputs_group_attr = l_compound_attr_fn.create("inputsGroup", "ig", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_mode_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_chord_part_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.addChild(g_cntrl_xform_attr));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setArray(true));
	CHECK_MSTATUS_AND_RETURN_IT(l_compound_attr_fn.setUsesArrayDataBuilder(true));
	//--------------------------------------------------------
	//create attibutes
	//outPuts
	//--------------------------------------------------------
	g_out_tang_a_attr = l_matrix_attr_fn.create("outTangFormA", "oxA", MFnMatrixAttribute::kDouble, &l_status);
	l_matrix_attr_fn.setStorable(false);
	//--------------------------------------------------------
	g_out_knot_attr = l_matrix_attr_fn.create("outKnode", "okn", MFnMatrixAttribute::kDouble, &l_status);
	l_matrix_attr_fn.setStorable(false);
	//--------------------------------------------------------
	g_out_tang_b_attr = l_matrix_attr_fn.create("outTangFormB", "oxB", MFnMatrixAttribute::kDouble, &l_status);
	l_matrix_attr_fn.setStorable(false);
	//--------------------------------------------------------
	g_out_group_attr = l_compound_attr_fn.create("outputsGroup", "og", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_compound_attr_fn.addChild(g_out_tang_a_attr);
	l_compound_attr_fn.addChild(g_out_tang_b_attr);
	l_compound_attr_fn.setArray(true);
	l_compound_attr_fn.setUsesArrayDataBuilder(true);
	//--------------------------------------------------------	
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_closed_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_inputs_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_out_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(addAttribute(g_out_knot_attr));
	//--------------------------------------------------------
	//Attributes  Affects		
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr, g_out_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr, g_out_tang_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr, g_out_tang_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_closed_attr, g_out_knot_attr));

	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr, g_out_group_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr, g_out_tang_a_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr, g_out_tang_b_attr));
	CHECK_MSTATUS_AND_RETURN_IT(attributeAffects(g_inputs_group_attr, g_out_knot_attr));

	return MS::kSuccess;
}
MStatus	vufCurveBezier3Tangents::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus l_status;
	if (plug == g_out_tang_a_attr || plug == g_out_tang_b_attr || plug == g_out_group_attr)
	{
		bool l_closed = data.inputValue(g_closed_attr).asBool();

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
			m_knots_array[i].m_knot			= l_input_data.child(g_cntrl_xform_attr).asMatrix();
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
		}
	}
	return MS::kUnknownParameter;
}
void vufCurveBezier3Tangents::auto_tangent(const vufKnotInfo& p_prev, const vufKnotInfo& p_next, vufKnotInfo& p_curr)
{
	MVector l_pos_prev(p_next.m_knot[3][0], p_next.m_knot[3][1], p_next.m_knot[3][2]);
	MVector l_pos_next(p_next.m_knot[3][0], p_next.m_knot[3][1], p_next.m_knot[3][2]);
	MVector l_pos_curr(p_curr.m_knot[3][0], p_curr.m_knot[3][1], p_curr.m_knot[3][2]);
	auto l_dir = l_pos_prev + l_pos_next - 2.0 * l_pos_curr;
}


