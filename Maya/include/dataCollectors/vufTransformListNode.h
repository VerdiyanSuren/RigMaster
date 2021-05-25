#ifndef VF_TP_TRNSFRM_LST_NODE_H
#define VF_TP_TRNSFRM_LST_NODE_H

#include <maya/MPxNode.h>

namespace vufTP
{
	class vufTransformListNode : public MPxNode
	{
	public:
		vufTransformListNode();
		virtual ~vufTransformListNode() {}

		static  void*	creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		//virtual MStatus	connectionBroken(const MPlug& p_my_plug, const MPlug& p_other_plug, bool asSrc) override;

		static const MTypeId	g_id;
		//attributes
		static MObject	g_lock_attr;
		static MObject	g_transform_in_attr;
		static MObject	g_data_out_attr;

		static const MString	g_type_name;;
		uint64_t				m_gen_id;
	};

}
#endif // !VF_TP_TRNSFRM_LST_NODE_H
