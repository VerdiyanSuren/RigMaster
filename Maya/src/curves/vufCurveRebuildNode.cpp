#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnCompoundAttribute.h>

#include <curves/vufCurveRebuildNode.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>


using namespace vufRM;
using namespace vufMath;

MObject	vufCurveRebuildNode::g_lock_attr;
MObject	vufCurveRebuildNode::g_pass_attr;

MObject	vufCurveRebuildNode::g_rebuild_compound_attr;
MObject	vufCurveRebuildNode::g_rebuild_always_attr;
MObject	vufCurveRebuildNode::g_rebuild_type_attr;
MObject	vufCurveRebuildNode::g_rebuild_samples_attr;

MObject	vufCurveRebuildNode::g_curve_container_in_attr;
MObject	vufCurveRebuildNode::g_curve_container_out_attr;
MObject	vufCurveRebuildNode::g_store_data_attr;

vufCurveRebuildNode::vufCurveRebuildNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}
void* vufCurveRebuildNode::creator()
{
	return new vufCurveRebuildNode();
}


MStatus	vufCurveRebuildNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	MFnEnumAttribute		l_enum_attr_fn;
	MFnCompoundAttribute	l_compound_attr_fn;
	//------------------------------------------------------------------------------------------------
	VF_RM_CREATE_AND_ADD_LOCK_ATTR();
	VF_RM_CREATE_AND_ADD_PASS_ATTR();
	//------------------------------------------------------------------------------------------------
	// Rebuild Always
	g_rebuild_always_attr = l_numeric_attr_fn.create("rebuildAlways", "ral", MFnNumericData::kBoolean, false, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	// Rebuild Typo
	g_rebuild_type_attr = l_enum_attr_fn.create("type", "tp", 0, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_enum_attr_fn.addField("Constant", k_rebuild_constant));
	CHECK_MSTATUS(l_enum_attr_fn.setDefault(k_rebuild_constant));
	CHECK_MSTATUS(l_enum_attr_fn.setStorable(true));
	// Rebild Samples
	g_rebuild_samples_attr = l_numeric_attr_fn.create("samples", "smpl", MFnNumericData::kInt, 3, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_numeric_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_numeric_attr_fn.setMin(2));
	CHECK_MSTATUS(l_numeric_attr_fn.setDefault(5));
	// Rebuild Compound
	g_rebuild_compound_attr = l_compound_attr_fn.create("rebuild", "rbl", &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_rebuild_always_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_rebuild_type_attr));
	CHECK_MSTATUS(l_compound_attr_fn.addChild(g_rebuild_samples_attr));
	//----------------------------------------------------------------------------------------------------
	// Input Curve Container Attr
	g_curve_container_in_attr = l_typed_attr_fn.create("curveInData", "cid", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setWritable(true);
	l_typed_attr_fn.setReadable(false);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setHidden(false);
	l_typed_attr_fn.setKeyable(true);
	
	g_store_data_attr = l_typed_attr_fn.create("curveRebuildData", "crd", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setConnectable(false);
	l_typed_attr_fn.setHidden(true);

	// Out Curve Container Attr
	g_curve_container_out_attr = l_typed_attr_fn.create("curveOutData", "cod", mpxCurveWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	//----------------------------------------------------------------------------------------------------
	// Add Attribute
	l_status = addAttribute(g_rebuild_compound_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_curve_container_in_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_store_data_attr);				CHECK_MSTATUS_AND_RETURN_IT(l_status);
	//----------------------------------------------------------------------------------------------------
	// Attributes Affects
	l_status = attributeAffects(g_lock_attr,				g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_pass_attr,				g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_rebuild_compound_attr,	g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_rebuild_always_attr,		g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_rebuild_type_attr,		g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_rebuild_samples_attr,		g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	l_status = attributeAffects(g_curve_container_in_attr,	g_curve_container_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}

MStatus	vufCurveRebuildNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_curve_container_out_attr)
	{
		MStatus l_status;
		bool l_passed = p_data.inputValue(g_pass_attr).asBool();		
		// Pass attribute has got most important status
		if (l_passed == true)
		{
			// Work as non locked null node
			std::shared_ptr<vufCurveData> l_in_data;
			std::shared_ptr<vufCurveData> l_out_data;
			
			VF_RM_GET_DATA_FROM_OUT_AND_CREATE(mpxCurveWrapper, vufCurveData, p_data, g_curve_container_out_attr, l_out_data);
			VF_RM_GET_DATA_FROM_IN(mpxCurveWrapper, vufCurveData, p_data, g_curve_container_in_attr, l_in_data);
			if (l_in_data != nullptr)
			{
				l_out_data->m_internal_data = l_in_data->m_internal_data;
				p_data.setClean(g_curve_container_out_attr);
				return MS::kSuccess;
			}
			l_out_data->m_internal_data = nullptr;
			p_data.setClean(g_curve_container_out_attr);
			return MS::kSuccess;
		}
		// Lock attfribute has got next level importance status 
		bool l_locked = p_data.inputValue(g_lock_attr).asBool();
		if (l_locked == true)
		{
			std::shared_ptr<vufCurveData> l_in_data;
			std::shared_ptr<vufCurveData> l_out_data;

		}

		p_data.setClean(g_curve_container_out_attr);
	}
	return MS::kUnknownParameter;
}

