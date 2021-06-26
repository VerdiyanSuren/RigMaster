#ifndef VF_RM_CURVE_QUAT_CLOSE_NODE_H
#define VF_RM_CURVE_QUAT_CLOSE_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveQuatCloseNode :public MPxNode
	{
	public:
		vufCurveQuatCloseNode();
		virtual ~vufCurveQuatCloseNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

		static const MTypeId	g_id;
		static const MString	g_type_name;

		static MObject	g_quaternion_compound_attr;
		static MObject	g_quaternion_division_attr;
		static MObject	g_quaternion_mode_attr;
		static MObject	g_quaternion_pin_start_attr;
		static MObject	g_quaternion_pin_start_value_attr;
		static MObject	g_quaternion_pin_end_attr;
		static MObject	g_quaternion_pin_end_value_attr;
		static MObject	g_quaternion_offset_attr;		

		static MObject	g_transfoms_attr;
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
		static MObject	g_data_store_attr;
	private:
		uint64_t		m_gen_id;
	};
}

#endif // !VF_RM_CURVE_QUAT_CLOSE_NODE_H
