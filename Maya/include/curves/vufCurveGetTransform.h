#ifndef VF_CRV_GET_TRNSFRM_H
#define VF_CRV_GET_TRNSFRM_H

#include <maya/MPxNode.h>

namespace vufTP
{
	class vufCurveGetTransformNode : public MPxNode
	{
	public:
		enum
		{
			k_x = 0,
			k_y = 1,
			k_z = 2
		};
		vufCurveGetTransformNode():MPxNode(){}
		virtual ~vufCurveGetTransformNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

	
		static MObject	g_parent_xform_attr;
		static MObject	g_in_curve_attr;
		static MObject	g_axis_attr;
		static MObject	g_param_attr;
		static MObject	g_param_pos_attr;
		static MObject	g_param_rot_attr;
		static MObject	g_param_scl_attr;

		static MObject	g_res_compound_attr;

		static MObject	g_res_pos_attr;

		static MObject	g_res_rot_attr;
		static MObject	g_res_rot_x_attr;
		static MObject	g_res_rot_y_attr;
		static MObject	g_res_rot_z_attr;

		static MObject	g_res_scl_attr;

		static MObject	g_res_xform;
		static MObject	g_res_vufTransform;
	};

}
#endif // !VF_CRV_GET_TRNSFRM_H
