#ifndef VF_MAYA_LUA_EXPRESSION_NODE_H
#define VF_MAYA_LUA_EXPRESSION_NODE_H

#include <maya/MPxNode.h>
#include <vufLuaWrapper.h>

namespace vufRM
{
	class vufMayaLuaExpressionNode :public MPxNode
	{
	public:
		vufMayaLuaExpressionNode();
		virtual ~vufMayaLuaExpressionNode();

		static  void*	creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_script_attr;
		static MObject	g_ports_script_attr;

		static MObject	g_input_ports_attr;
		static MObject	g_input_time_attr;
		static MObject	g_input_number_attr;
		static MObject	g_input_angle_attr;
		static MObject	g_input_matrix_attr;
		static MObject	g_input_mesh_attr;
		static MObject	g_input_curve_attr;
		static MObject	g_input_surface_attr;

		static MObject	g_output_ports_attr;
		static MObject	g_output_number_attr;
		static MObject	g_output_angle_attr;
		static MObject	g_output_matrix_attr;
	private:
		uint64_t			m_script_hash		= 0;
		uint64_t			m_scropt_port_hash	= 0;
		vuf::luaWrapper		m_lua_machine;
	};
}


#endif // !VF_MAYA_LUA_EXPRESSION_NODE_H
