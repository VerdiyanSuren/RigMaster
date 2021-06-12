#ifndef VF_RM_QUAT_CRV_NODE_H
#define VF_RM_QUAT_CRV_NODE_H

#include <maya/MPxNode.h>
#include <maya/MMatrixArray.h>
#include <curves/vufCurvesInclude.h>

namespace vufRM
{
	class vufCurveBSplineNode : public MPxNode
	{
	public:
		enum CrvDegree
		{
			k_none		= 0,
			k_linear	= 1,
			k_quadratic	= 2,
			k_cubic		= 3,
			k_tetra		= 4,
			k_penta		= 5
		};
		enum CrvType
		{
			k_bspline	= 0,
			k_bezier	= 1
		};
		vufCurveBSplineNode();
		virtual ~vufCurveBSplineNode() {}

		static  void*	creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		
		
		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes

		// Curve
		static MObject	g_curve_compound_attr;
		static MObject	g_close_attr;
		static MObject	g_degree_attr;
		static MObject	g_rebuild_store_attr;
		static MObject	g_quaternion_store_attr;
		static MObject	g_scale_store_attr;
		//static MObject	g_remap_store_attr;
		// Quaternion
		VF_RM_CRV_NODE_DECLARE_QUATERNIONS_ATTR();
		// Rebuild
		VF_RM_CRV_NODE_DECLARE_REBUILD_ATTR();
		// Scale
		VF_RM_CRV_NODE_DECLARE_SCALE_ATTR();

		static MObject	g_transfoms_attr;
		static MObject	g_data_out_attr;

	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_QUAT_CRV_NODE_H
