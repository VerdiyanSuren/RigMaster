#include <dataCollectors/vufMatrixListLookAtNode.h>
#include <vufMayaGlobalIncludes.h>
#include <maya/MFnTypedAttribute.h>
#include <data/vufMayaDataList.h>
#include <math/vufMatrix.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufMatrixListLookAtNode::g_data_in_attr;
MObject	vufMatrixListLookAtNode::g_data_out_attr;

vufMatrixListLookAtNode::vufMatrixListLookAtNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}

void*	vufMatrixListLookAtNode::creator()
{
	return new vufMatrixListLookAtNode();
}
MStatus	vufMatrixListLookAtNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;

	// Input Data
	g_data_in_attr = l_typed_attr_fn.create("inMatrixList", "iml", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// Output Data
	g_data_out_attr = l_typed_attr_fn.create("outMatrixList", "oml", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	//g_data_out_attr = l_typed_attr_fn.create("TransformList","tl", mpxTransformListWrapper::g_id, MObject::kNullObj, &l_status);// , MFnPluginData().create(vufUniversalData::id, &status));
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(false);
	l_typed_attr_fn.setWritable(false);

	// Add Attributes
	l_status = addAttribute(g_data_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attribute affects
	l_status = attributeAffects(g_data_in_attr, g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
void 	vufMatrixListLookAtNode::postConstructor()
{
	setExistWithoutOutConnections(true);
	setExistWithoutInConnections(true);
}
MStatus	vufMatrixListLookAtNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		std::shared_ptr<vufMatrixListData>	l_out_data;
		std::shared_ptr<vufMatrixListData>	l_in_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		VF_RM_GET_DATA_FROM_IN(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_in_attr, l_in_data);
		auto&	l_in_array	= l_in_data->m_internal_data->m_array_v;
		auto&	l_out_array = l_out_data->m_internal_data->m_array_v;
		auto	l_in_sz = l_in_array.size();
		auto	l_out_sz = l_out_array.size();
		if (l_in_sz <2)
		{
			l_out_data->m_internal_data = l_in_data->m_internal_data;
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (l_out_sz != l_in_sz)
		{
			l_out_array.resize(l_in_sz);
		}
		vufMatrix_4d l_origin_matr = l_in_array[0];
		vufVector_4d l_origin_dir = l_origin_matr.get_axis_x_4();
		for (int i = 0; i < l_in_sz -1; ++i)
		{
			vufMatrix_4d& l_next_matr = l_in_array[i + 1];
			vufVector_4d l_look_dir = l_next_matr.get_translation_4() - l_origin_matr.get_translation_4();
			auto l_quat = vufQuaternion_d::rotate_arc(l_origin_dir, l_look_dir);
			l_quat = l_origin_matr.get_quaternion() * l_quat;
			l_origin_matr.set_quaternion(l_quat);
			l_out_array[i] = l_origin_matr;

			l_origin_matr = l_next_matr;
			l_origin_dir = l_next_matr.get_axis_x_4();
		}
		l_out_array.back() = l_origin_matr;
		p_data.setClean(p_plug);
		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
MStatus	vufMatrixListLookAtNode::connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	if (p_plug_1 == g_data_in_attr)
	{
		MStatus l_status;
		MObject				l_data_obj;
		MFnDependencyNode	l_node(p_plug_1.node());
		std::shared_ptr<vufMatrixListData>	l_in_data;
		VF_RM_GET_DATA_FROM_PLUG(mpxMatrixListWrapper, vufMatrixListData, g_data_in_attr, l_in_data);
		l_in_data->m_internal_data = l_in_data->m_internal_data->get_copy();
	}
	return MPxNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);
}

