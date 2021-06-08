#ifndef VF_RM_FROM_MAYA_CRV_NODE_H
#define VF_RM_FROM_MAYA_CRV_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufFromMayaCurveNode : public MPxNode
	{
	public:
		vufFromMayaCurveNode();
		virtual ~vufFromMayaCurveNode() {}

		static void*	creator();
		static MStatus	initialize();
		virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId g_id;
		static const MString	g_type_name;

		//attributes
		static MObject	g_lock_attr;

		static MObject	g_maya_curve_attr;
		static MObject	g_data_out_attr;

		uint64_t		m_gen_id;

	};
}

#endif // !VF_TP_FROM_MAYA_CRV_H
