#ifndef VF_RM_CRV_SLIDE_NODE_H
#define VF_RM_CRV_SLIDE_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveSlideNode : public MPxNode
	{
	public:
		vufCurveSlideNode();
		virtual ~vufCurveSlideNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;


		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes
		static MObject	g_enable_attr;
		static MObject	g_offset_attr;

		static MObject	g_use_length_attr;
		static MObject	g_length_attr;
		static MObject	g_hardness_attr;

		static MObject	g_start_attr;
		static MObject	g_start_weight_attr;
		static MObject	g_end_attr;
		static MObject	g_end_weight_attr;

		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_CRV_COMPOSE_NODE_H