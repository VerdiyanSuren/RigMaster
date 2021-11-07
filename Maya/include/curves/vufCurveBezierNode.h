#ifndef VF_RM_BEZIER_OPEN_CRV_NODE_H
#define VF_RM_BEZIER_OPEN_CRV_NODE_H


#include <maya/MPxNode.h>
#include <curves/vufMayaCurvesInclude.h>
#include <vufMayaGlobalIncludes.h>

namespace vufRM
{
	class vufCurveBezierNode : public MPxNode
	{
	public:		
		vufCurveBezierNode();
		virtual ~vufCurveBezierNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;


		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes

		// Curve
		static MObject	g_curve_compound_attr;
		static MObject	g_close_attr;
		static MObject	g_degree_attr;
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
		static MObject	g_params_out_attr;

	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_BEZIER_OPEN_CRV_NODE_H
