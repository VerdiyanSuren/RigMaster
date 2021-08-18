#ifndef VF_RM_CRV_CLST_PRM_NODE_H
#define VF_RM_CRV_CLST_PRM_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveClosestParamNode :public MPxNode
	{
	public:
		vufCurveClosestParamNode() {}
		virtual ~vufCurveClosestParamNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_in_curve_attr;
		static MObject	g_in_xform_in_attr;
		static MObject  g_in_start_param;
		static MObject  g_in_end_param;
		static MObject  g_in_division_param;


		static MObject	g_out_param_attr;
	};
}
#endif // !VF_RM_CRV_CLST_PRM_NODE_H





