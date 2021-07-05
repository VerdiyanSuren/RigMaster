#ifndef VF_RM_CURVE_QUAT_2ENDS_NODE_H
#define VF_RM_CURVE_QUAT_2ENDS_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveQuat2EndsNode :public MPxNode
	{
	public:
		vufCurveQuat2EndsNode();
		virtual ~vufCurveQuat2EndsNode() {}
		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_enable_attr;
		//static MObject	g_quaternion_compound_attr;
		static MObject	g_division_attr;
		//static MObject	g_quaternion_mode_attr;
		static MObject	g_fcurve_attr;
		static MObject	g_blend_curve_attr;
		static MObject	g_twist_start_attr;
		static MObject	g_twist_end_attr;

		static MObject	g_transfoms_start_attr;
		static MObject	g_transfoms_end_attr;
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
	private:
		uint64_t		m_gen_id;
	};
}

#endif // !VF_RM_CURVE_QUAT_2ENDS_NODE_H
