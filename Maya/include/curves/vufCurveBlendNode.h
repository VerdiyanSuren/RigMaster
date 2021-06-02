#ifndef VF_RM_BLEND_CRV_NODE_H
#define VF_RM_BLEND_CRV_NODE_H

#include <maya/MPxNode.h>
#include <curves/vufCurvesInclude.h>

namespace vufRM
{
	class vufCurveBlendNode : public MPxNode
	{
	public:
		vufCurveBlendNode();
		virtual ~vufCurveBlendNode() {}
		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;


		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_curve_container_1_attr;
		static MObject	g_curve_container_2_attr;
		static MObject	g_weight_attr;
		static MObject	g_data_out_attr;

	private:
		uint64_t		m_gen_id;

	};
}

#endif !VF_TP_BLEND_CRV_NODE_H
