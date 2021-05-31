#ifndef VF_TP_QUAT_NULL_NODE_H
#define VF_TP_QUAT_NULL_NODE_H

#include <maya/MPxNode.h>
//#include "math/curves/vufCurveContatiner.h"
#include "quatCurves/vufCurveData.h"
#include <memory>

namespace vufTP
{
	class vufCurveNullNode : public MPxNode
	{
	public:
		vufCurveNullNode():MPxNode() {}
		virtual ~vufCurveNullNode() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		//virtual MStatus	connectionBroken(const MPlug& p_my_plug, const MPlug& p_other_plug, bool asSrc) override ;

		static const MTypeId	g_id;
		//attributes
		static MObject	g_lock_attr;
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
		static MObject	g_store_data_attr;

		static const MString	g_type_name;

	private: 
		/// func(p_data, internal, input, out)
		void get_locked_ptr(MDataBlock& p_data,
							std::shared_ptr<vufCurveContainerData_4d>& p_internal,
							std::shared_ptr<vufCurveContainerData_4d>& p_input,
							std::shared_ptr<vufCurveContainerData_4d>& p_result );
		bool m_was_locked = true;
	};

}
#endif // !VF_TP_QUAT_NULL_NODE_H
