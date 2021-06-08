#ifndef VF_RM_CRV_CLST_PNT_NODE_H
#define VF_RM_CRV_CLST_PNT_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveClosestPointNode :public MPxNode
	{
	public:
		vufCurveClosestPointNode() {}
		virtual ~vufCurveClosestPointNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_in_curve_attr;
		static MObject	g_xform_in_attr;
		static MObject	g_translate_attr;
		static MObject	g_tangent_attr;
		static MObject	g_param_attr;
		static MObject	g_dist_attr;
	};
}
#endif // !VF_RM_CRV_CLST_PNT_NODE_H





