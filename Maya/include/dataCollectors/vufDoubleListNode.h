#ifndef VF_RM_DOUBLE_LST_NODE_H
#define VF_RM_DOUBLE_LST_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufDoubleListNode : public MPxNode
	{
	public:
		vufDoubleListNode();
		virtual ~vufDoubleListNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		//virtual MStatus	connectionBroken(const MPlug& p_my_plug, const MPlug& p_other_plug, bool asSrc) override;

		static const MTypeId	g_id;
		//attributes
		static MObject	g_double_in_attr;
		static MObject	g_data_out_attr;

		static const MString	g_type_name;
	private:
	};
}
#endif // !VF_RM_DOUBLE_LST_NODE_H
