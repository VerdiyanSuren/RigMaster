#ifndef VF_RM_CRV_COMPOSE_NODE_H
#define VF_RM_CRV_COMPOSE_NODE_H

#include <maya/MPxNode.h>

namespace vufRM
{
	class vufCurveComposeNode : public MPxNode
	{
	public:
		vufCurveComposeNode();
		virtual ~vufCurveComposeNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;


		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes
		static MObject	g_enable_attr;
		static MObject	g_operator_attr;
		static MObject	g_weight_a_attr;
		static MObject	g_weight_b_attr;

		static MObject	g_data_a_in_attr;
		static MObject	g_data_b_in_attr;
		static MObject	g_data_out_attr;
	private:
		uint64_t		m_gen_id;
	};
}
#endif // !VF_RM_CRV_COMPOSE_NODE_H
