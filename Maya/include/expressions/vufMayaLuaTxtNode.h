#ifndef VF_MAYA_LUA_TXT_NODE_H
#define VF_MAYA_LUA_TXT_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufMayaLuaTxtNode :public MPxNode
	{
	public:
		vufMayaLuaTxtNode();
		virtual ~vufMayaLuaTxtNode() {}

		static  void*	creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_expression_attr;
		static MObject	g_demand_attr;

	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_MAYA_LUA_TXT_NODE_H
