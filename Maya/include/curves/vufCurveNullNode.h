#ifndef VF_RM_CURVE_NULL_NODE_H
#define VF_RM_CURVE_NULL_NODE_H

#include <maya/MPxNode.h>
//#include "math/curves/vufCurveContatiner.h"
#include <data/vufMayaDataList.h>
#include <memory>

namespace vufRM
{
	class vufCurveNullNode : public MPxNode
	{
	public:
		vufCurveNullNode():MPxNode() {}
		virtual ~vufCurveNullNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual void 	postConstructor() override;
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		virtual MStatus	connectionBroken(const MPlug& p_my_plug, const MPlug& p_other_plug, bool asSrc) override ;

		static const MTypeId	g_id;
		//attributes
		static MObject	g_lock_attr;
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
		static MObject	g_store_data_attr;

		static const MString	g_type_name;

	private: 
		bool			m_locked = true;
	};

}
#endif // !VF_RM_CURVE_NULL_NODE_H
