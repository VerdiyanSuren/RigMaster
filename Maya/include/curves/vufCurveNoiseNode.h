#ifndef VF_RM_CRV_NOISE_NODE_H
#define VF_RM_CRV_NOISE_NODE_H

#include <maya/MPxNode.h>
#include <maya/MMatrixArray.h>
#include <curves/vufCurvesInclude.h>
#include <vufMayaGlobalIncludes.h>
#include <curves/vufMayaCurvesInclude.h>

namespace vufRM
{
	class vufCurveNoiseNode : public MPxNode
	{
	public:
		vufCurveNoiseNode();
		virtual ~vufCurveNoiseNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		virtual MStatus	connectionBroken(const MPlug& p_plug_1, const MPlug& p_plug_2, bool p_as_src) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes
		static MObject	g_enable_attr;
		static MObject	g_use_quat_attr;
		static MObject	g_use_scale_attr;
		static MObject	g_time_attr;
		static MObject	g_speed_attr;
		static MObject	g_amount_x_attr;
		static MObject	g_amount_y_attr;
		static MObject	g_amount_z_attr;
		static MObject	g_scale_x_attr;
		static MObject	g_scale_y_attr;
		static MObject	g_scale_z_attr;
		static MObject	g_offset_x_attr;
		static MObject	g_offset_y_attr;
		static MObject	g_offset_z_attr;

		VF_RM_CRV_NODE_DECLARE_REBUILD_ATTR();

		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_CRV_NOISE_NODE_H
