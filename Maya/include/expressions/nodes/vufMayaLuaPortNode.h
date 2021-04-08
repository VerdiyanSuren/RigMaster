#ifndef VF_MAYA_LUA_PORT_NODE_H
#define VF_MAYA_LUA_PORT_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	/*
	* Class to fill port structure and connection
	*/	
	class vufMayaLuaPortNode :public MPxNode
	{
	public:
		vufMayaLuaPortNode();
		virtual ~vufMayaLuaPortNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_port_attr;
		static MObject	g_demand_attr;

	private:
		uint64_t		m_gen_id;
	};

}
#endif // !VF_MAYA_LUA_PORT_NODE_H
