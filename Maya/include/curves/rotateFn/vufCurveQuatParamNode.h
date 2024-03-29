#ifndef VF_RM_CURVE_QUAT_PARAM_NODE_H
#define VF_RM_CURVE_QUAT_PARAM_NODE_H

#include <maya/MPxNode.h>
namespace vufRM
{
	class vufCurveQuatParamNode :public MPxNode
	{
	public:
		vufCurveQuatParamNode();
		virtual ~vufCurveQuatParamNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;
			
		static MObject	g_enable_attr;
		static MObject	g_doubles_attr;
		static MObject	g_transfoms_attr;
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_CURVE_QUAT_PARAM_NODE_H
