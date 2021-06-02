#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDoubleArray.h>
#include <maya/MPointArray.h>

#include <curves/vufCurveFromMayaNode.h>
#include <data/vufMayaDataList.h>
#include <curves/vufOpenBSpline.h>
#include <vufMayaGlobalIncludes.h>


using namespace vufRM;
using namespace vufMath;

MObject	vufFromMayaCurveNode::g_lock_attr;
MObject	vufFromMayaCurveNode::g_maya_curve_attr;
MObject	vufFromMayaCurveNode::g_data_out_attr;

vufFromMayaCurveNode::vufFromMayaCurveNode():MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufFromMayaCurveNode::creator()
{
	return new vufFromMayaCurveNode();
}
MStatus	vufFromMayaCurveNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;

	//------------------------------------------------------------------------------------------------
	// Lock
	VF_RM_CREATE_AND_ADD_LOCK_ATTR();
	//------------------------------------------------------------------------------------------------
	// Maya Curve Attribute
	g_maya_curve_attr = l_typed_attr_fn.create("mayaCurve","mc",MFnData::kNurbsCurve, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);

	//------------------------------------------------------------------------------------------------
	// Out Curve Data
	g_data_out_attr = l_typed_attr_fn.create("curveData", "cd", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	//------------------------------------------------------------------------------------------------
	// Add Attributes
	l_status = addAttribute(g_maya_curve_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	//------------------------------------------------------------------------------------------------
	// Attribute Affects
	l_status = attributeAffects(g_lock_attr, g_data_out_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_maya_curve_attr, g_data_out_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);


	return MS::kSuccess;
}
MStatus	vufFromMayaCurveNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		bool	l_locked			= p_data.inputValue(g_lock_attr).asBool();
		//-----------------------------------------------------------------------
		// Read our container data
		std::shared_ptr<vufCurveData> l_out_data;
		VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_data_out_attr, l_out_data);
		auto l_data_owner_id = l_out_data->get_owner_id();
		if (l_out_data->get_owner_id() != m_gen_id)
		{
			// To Do 
			// Copy data and make mine
		}
		l_out_data->set_owner_id(m_gen_id);
		if (l_out_data->m_internal_data == nullptr)
		{
			l_out_data->m_internal_data = vufCurveContainer_4d::create();
		}
		if (l_locked == true)
		{
			// Node is locked. Let node  stay unchanged.
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		//-----------------------------------------------------------------------
		// Read maya curve
		MObject l_maya_curve_object	= p_data.inputValue(g_maya_curve_attr).asNurbsCurve();
		if (l_maya_curve_object.isNull() == false &&
			l_maya_curve_object.hasFn(MFn::kNurbsCurve) == true)
		{
			// Get Info from maya curve
			MFnNurbsCurve	l_maya_crv_fn(l_maya_curve_object);
			MDoubleArray	l_knots_array;
			MPointArray		l_cv_array;
			auto l_form	=  l_maya_crv_fn.form();
			l_maya_crv_fn.getKnots(l_knots_array);
			l_maya_crv_fn.getCVs(l_cv_array, MSpace::kWorld);
			int l_degree = l_maya_crv_fn.degree();
			//---------------------------------------------------------------------
			bool l_is_crv_new	= l_out_data->m_internal_data->switch_curve( l_degree, vufMath::vufCurveType::k_open_bspline);
			auto l_crv_ptr		= l_out_data->m_internal_data->get_curve_ptr()->as_explicit_curve();
			
			if (l_crv_ptr != nullptr )
			{				
				if (l_is_crv_new == true)
				{
					// make knot vector initialization
				}
				// Fill constrol points 
				l_crv_ptr->set_nodes_count(l_cv_array.length());
				for (unsigned int i = 0; i < l_cv_array.length(); ++i)
				{
					l_crv_ptr->set_node_at(i, vufVector_4d(l_cv_array[i].x, l_cv_array[i].y, l_cv_array[i].z));
				}
			}
			// Set our data
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		l_out_data->m_internal_data->set_curve_ptr(nullptr);
		p_data.setClean(g_data_out_attr);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}