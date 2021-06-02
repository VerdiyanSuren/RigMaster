#ifndef VF_TP_CURVE_RBLD_NODE_H
#define VF_TP_CURVE_RBLD_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveRebuildNode : public MPxNode
	{
	public:
		enum CrvType
		{
			k_rebuild_constant = 0
		};
		vufCurveRebuildNode();
		virtual ~vufCurveRebuildNode() {}

		static  void*	creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;


		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes
		static MObject	g_lock_attr;
		static MObject	g_pass_attr;
		static MObject	g_rebuild_compound_attr;
		static MObject	g_rebuild_always_attr;
		static MObject	g_rebuild_type_attr;
		static MObject	g_rebuild_samples_attr;

		static MObject	g_curve_container_in_attr;
		static MObject	g_curve_container_out_attr;
		static MObject	g_store_data_attr;

		uint64_t		m_gen_id;
	};
}
#endif // ! VF_TP_CURVE_RBLD_NODE_H
