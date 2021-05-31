#ifndef VF_TP_TO_MAYA_CRV_NODE_H
#define VF_TP_TO_MAYA_CRV_NODE_H

#include <maya/MPxNode.h>

namespace vufTP
{
	class vufToMayaCurveNode : public MPxNode
	{
	public:
		vufToMayaCurveNode() {}
		virtual ~vufToMayaCurveNode() {}

		static void*	creator();
		static MStatus	initialize();
		virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		//attributes
		static MObject	g_lock_attr;

		static MObject	g_data_in_attr;
		static MObject	g_maya_curve_attr;

	};
}

#endif // !VF_TP_FROM_MAYA_CRV_H
