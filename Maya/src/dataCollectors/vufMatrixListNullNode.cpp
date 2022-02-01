#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>

#include <dataCollectors/vufMatrixListNullNode.h>
#include <data/vufData.h>
#include <vufMayaGlobalIncludes.h>
//#include <maya/MMatrix.h>
//#include <maya/MMatrixArray.h>
#include <data/vufMayaDataList.h>
#include <maya/MFnMatrixArrayData.h>

using namespace vufRM;

MObject	vufMatrixListNullNode::g_lock_attr;
MObject	vufMatrixListNullNode::g_data_in_attr;
MObject	vufMatrixListNullNode::g_data_out_attr;
MObject	vufMatrixListNullNode::g_data_store_attr;


vufMatrixListNullNode::vufMatrixListNullNode() :MPxNode()
{
	m_gen_id = ++g_unique_id;
}

void* vufMatrixListNullNode::creator()
{
	return new vufMatrixListNullNode();
}
MStatus	vufMatrixListNullNode::initialize()
{
	MStatus l_status;
	MFnTypedAttribute		l_typed_attr_fn;
	MFnNumericAttribute		l_numeric_attr_fn;
	
	VF_RM_CREATE_AND_ADD_LOCK_ATTR();
	// Input Data
	g_data_in_attr	= l_typed_attr_fn.create("inMatrixList", "iml", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	CHECK_MSTATUS(l_typed_attr_fn.setWritable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setReadable(false));
	CHECK_MSTATUS(l_typed_attr_fn.setStorable(true));
	CHECK_MSTATUS(l_typed_attr_fn.setHidden(false));
	CHECK_MSTATUS(l_typed_attr_fn.setKeyable(true));
	// Internal Data to store and lock
	g_data_store_attr = l_typed_attr_fn.create("hiddenML", "hml", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setConnectable(false);
	l_typed_attr_fn.setHidden(true);
	// Output Data
	g_data_out_attr = l_typed_attr_fn.create("outMatrixList", "oml", mpxMatrixListWrapper::g_id, MObject::kNullObj, &l_status);
	//g_data_out_attr = l_typed_attr_fn.create("TransformList","tl", mpxTransformListWrapper::g_id, MObject::kNullObj, &l_status);// , MFnPluginData().create(vufUniversalData::id, &status));
	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_typed_attr_fn.setStorable(true);
	l_typed_attr_fn.setWritable(false);

	// Add Attributes
	l_status = addAttribute(g_data_in_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = addAttribute(g_data_store_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	// Attribute affects
	l_status = attributeAffects(g_lock_attr,	g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);
	l_status = attributeAffects(g_data_in_attr, g_data_out_attr);	CHECK_MSTATUS_AND_RETURN_IT(l_status);

	return MS::kSuccess;
}
void 	vufMatrixListNullNode::postConstructor()
{
	setExistWithoutOutConnections(true);
	setExistWithoutInConnections(true);
}

MStatus	vufMatrixListNullNode::compute(const MPlug& p_plug, MDataBlock& p_data)
{
	if (p_plug == g_data_out_attr)
	{
		MStatus l_status;
		bool	l_locked = p_data.inputValue(g_lock_attr).asBool();
		// if unlocked just pass
		if (l_locked == false) 
		{
			//VF_LOG_INFO("UNLOCKED");
			m_locked = false;
			std::shared_ptr<vufMatrixListData>	l_out_data;
			std::shared_ptr<vufMatrixListData>	l_in_data;
			VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_out_attr,	l_out_data, m_gen_id);
			VF_RM_GET_DATA_FROM_IN(						mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_in_attr,	l_in_data);
			if (l_in_data != nullptr)
			{
				l_out_data->m_internal_data = l_in_data->m_internal_data;
			}			
			p_data.setClean(g_data_out_attr);
			return MS::kSuccess;
		}
		// if just locked
		if (m_locked == false)
		{
			//VF_LOG_INFO("JUST LOCKED");
			m_locked = true;
			std::shared_ptr<vufMatrixListData>	l_out_data;
			std::shared_ptr<vufMatrixListData>	l_store_data;
			std::shared_ptr<vufMatrixListData>	l_in_data;
			VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_out_attr,	l_out_data,		m_gen_id);
			VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_store_attr, l_store_data, m_gen_id);
			VF_RM_GET_DATA_FROM_IN(						mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_in_attr,	l_in_data);
			
			l_store_data->m_internal_data	= l_in_data->m_internal_data->get_copy();
			l_out_data->m_internal_data		= l_store_data->m_internal_data;
			p_data.setClean(g_data_out_attr);
			p_data.setClean(g_data_store_attr);

			return MS::kSuccess;
		}
		//VF_LOG_INFO("LOCKED");
		// if locked
		std::shared_ptr<vufMatrixListData>	l_out_data;
		std::shared_ptr<vufMatrixListData>	l_store_data;
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_out_attr, l_out_data, m_gen_id);
		VF_RM_GET_DATA_FROM_OUT_AND_MAKE_REF_UNIQUE(mpxMatrixListWrapper, vufMatrixListData, p_data, g_data_store_attr, l_store_data, m_gen_id);
		l_out_data->m_internal_data = l_store_data->m_internal_data;		
		p_data.setClean(g_data_out_attr);

		return MS::kSuccess;
	}
	return MS::kUnknownParameter;
}
MStatus	vufMatrixListNullNode::connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src)
{
	MStatus l_status;
	MObject				l_data_obj;
	MFnDependencyNode	l_node(p_plug_1.node());
	std::shared_ptr<vufMatrixListData>	l_in_data;
	VF_RM_GET_DATA_FROM_PLUG(mpxMatrixListWrapper, vufMatrixListData, g_data_in_attr,		l_in_data);
	l_in_data->m_internal_data		= l_in_data->m_internal_data->get_copy();

	return MPxNode::connectionBroken(p_plug_1, p_plug_2, p_as_src);
}
