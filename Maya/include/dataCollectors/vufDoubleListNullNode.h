#ifndef VF_RM_DOUBLE_LST_NULL_NODE_H
#define VF_RM_DOUBLE_LST_NULL_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufDoubleListNullNode : public MPxNode
	{
	public:
		vufDoubleListNullNode();
		virtual ~vufDoubleListNullNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		//attributes
		static MObject	g_lock_attr;
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
		static MObject	g_data_store_attr;

		static const MString	g_type_name;
	private:
		bool			m_locked = true;
	};
}

#endif !VF_RM_DOUBLE_LST_NULL_NODE_H