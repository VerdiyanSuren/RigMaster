#ifndef VF_RM_CRV_CMPMT_PRM_NODE_H
#define VF_RM_CRV_CMPMT_PRM_NODE_H
#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveComponentParamNode :public MPxNode
	{
	public:
		vufCurveComponentParamNode() {}
		virtual ~vufCurveComponentParamNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_in_curve_attr;
		static MObject	g_in_value_attr;
		static MObject	g_in_axis_attr;
		static MObject  g_in_start_param;
		static MObject  g_in_end_param;
		static MObject  g_in_division_param;

		static MObject	g_out_param_attr;
	};
}

#endif // !VF_RM_CRV_CMPMT_PRM_NODE_H
