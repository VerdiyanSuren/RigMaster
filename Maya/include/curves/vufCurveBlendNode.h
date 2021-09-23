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
		virtual MStatus connectionMade(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;
		virtual MStatus connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;
		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_curve_container_1_attr;	// curve A
		static MObject	g_curve_container_2_attr;	// curve B
		static MObject	g_use_fcurve_attr;			// use blending function
		static MObject	g_fcurve_attr;				// function curve f(t)
		static MObject	g_weight_attr;				// global weight attribute
		static MObject	g_data_out_attr;			// result curve

	private:
		uint64_t		m_gen_id;

	};
}

#endif !VF_TP_BLEND_CRV_NODE_H
