#include <dataCollectors/vufMatrixListDecomposeNode.h>
#include <data/vufMayaDataList.h>
#include <vufMayaGlobalIncludes.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>

#include <vufObjectArrayFn.h>

using namespace vufRM;
using namespace vufMath;

MObject	vufMatrixListDecomposeNode::g_use_parent_attr;
MObject	vufMatrixListDecomposeNode::g_look_rotation_attr;
MObject	vufMatrixListDecomposeNode::g_data_in_attr;
MObject	vufMatrixListDecomposeNode::g_data_out_attr;

vufMatrixListDecomposeNode::vufMatrixListDecomposeNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}

void* vufMatrixListDecomposeNode::creator()
{
	return new vufMatrixListDecomposeNode();
}
MStatus	vufMatrixListDecomposeNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute	l_typed_attr_fn;
	MFnMatrixAttribute	l_matrix_attr_fn;
	MFnNumericAttribute l_numeric_attr_fn;
	// Use first matrix as parent

	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_use_parent_attr,	"useParent", "up", kInt, -1);
	VF_RM_CREATE_STORABLE_NUMERIC_ATTR(g_look_rotation_attr, "lookNext", "ln", kBoolean, true);
	// look at correction
	
	// Input Data
	g_data_in_attr = l_typed_attr_fn.create("outMatrixList", "oml", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));

	// Output Data
	g_data_out_attr = l_matrix_attr_fn.create("outxForm", "oxf", MFnMatrixAttribute::kDouble, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_matrix_attr_fn.setWritable(true);
	l_matrix_attr_fn.setStorable(true);
	l_matrix_attr_fn.setArray(true);
	l_matrix_attr_fn.setUsesArrayDataBuilder(true);

	// Add Attributes
	l_status = addAttribute(g_use_parent_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_look_rotation_attr);		CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_in_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);			CHECK_MSTATUS_AND_RETURN_IT(l_status);
	
	// Attributes Affects
	l_status = attributeAffects(g_use_parent_attr,		g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_look_rotation_attr,	g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr,			g_data_out_attr);	 CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
void 	vufMatrixListDecomposeNode::postConstructor()
{
	setExistWithoutOutConnections(true);
	setExistWithoutInConnections(true);
}
MStatus	vufMatrixListDecomposeNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		bool	l_look = p_data.inputValue(g_look_rotation_attr).asBool();
		int		l_parent = p_data.inputValue(g_use_parent_attr).asInt();
		std::shared_ptr<vufMatrixListData>	l_in_data;
		VF_RM_GET_DATA_FROM_IN(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_in_attr, l_in_data);
		auto&	l_array = l_in_data->m_internal_data->m_array_v;
		auto	l_sz = l_array.size();
		if (l_sz == 0)
		{
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (l_sz == 1)
		{
			MArrayDataHandle	l_out_array			= p_data.outputValue(g_data_out_attr);
			MArrayDataBuilder	l_out_array_bldr	= l_out_array.builder(&l_status);
			MDataHandle			l_out_hndl			= l_out_array_bldr.addElement(0, &l_status);
			if (l_status != MS::kSuccess)
			{
				VF_LOG_ERR("Failed Build output value");
			}
			l_out_hndl.set(*( (MMatrix*)(&l_array[0]) ));
			l_out_array.set(l_out_array_bldr);
			l_out_array.setAllClean();			
			p_data.setClean(p_plug);
			return MS::kSuccess;
		}
		if (l_parent > l_sz - 1)
		{
			l_parent = -1;
		}
		if (m_cach_array.length() != l_sz)
		{
			m_cach_array.setLength(l_sz);
		}
		if (l_look == true)
		{
			vufMatrix_4d l_origin_matr	= l_array[0];
			vufVector_4d l_origin_dir	= l_origin_matr.get_axis_x_4();
			for (int i = 0; i < (int)(l_sz-1); ++i)
			{
				vufMatrix_4d& l_next_matr	= l_array[i];
				vufVector_4d l_look_dir		= l_next_matr.get_translation_4() -	l_origin_matr.get_translation_4();
				auto l_quat = vufQuaternion_d::rotate_arc(l_origin_dir, l_look_dir);
				l_quat = l_origin_matr.get_quaternion()* l_quat;
				l_origin_matr.set_quaternion(l_quat);
				m_cach_array[i] = *((MMatrix*)(&l_origin_matr));
				l_origin_matr = l_next_matr;
			}
			m_cach_array[l_sz-1] = *((MMatrix*)(&l_array[l_sz-1]));
		}
		else
		{
			for (int i = 0; i < (int)(l_sz - 1); ++i)
			{
				m_cach_array[i] = *((MMatrix*)(&l_array[i]));
			}
		}
		if (l_parent >= 0)
		{
			//move
		}

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
MStatus	vufMatrixListDecomposeNode::connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	MStatus l_status;
	MObject				l_data_obj;
	MFnDependencyNode	l_node(p_plug_1.node());
	std::shared_ptr<vufMatrixListData>	l_in_data;
	VF_RM_GET_DATA_FROM_PLUG(mpxMatrixListWrapper, vufMatrixListData, g_data_in_attr, l_in_data);
	l_in_data->m_internal_data = l_in_data->m_internal_data->get_copy();

	return MPxNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);
}


