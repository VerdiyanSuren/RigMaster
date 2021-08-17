#ifndef VF_RM_CURVE_SWITCH_H
#define VF_RM_CURVE_SWITCH_H

#include <maya/MPxNode.h>
//#include "math/curves/vufCurveContatiner.h"
//#include <data/vufMayaDataList.h>

namespace vufRM
{
	class vufCurveSwitch : public MPxNode
	{
	public:
		vufCurveSwitch() :MPxNode() {}
		virtual ~vufCurveSwitch() {}

		static  void* creator();
		static  MStatus	initialize();
		virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;
		//virtual MStatus	connectionBroken(const MPlug& p_my_plug, const MPlug& p_other_plug, bool asSrc) override ;

		static const MTypeId	g_id;
		static const MString	g_type_name;
		//attributes
		static MObject	g_choose_attr;
		static MObject	g_data_in_attr;
		static MObject	g_data_out_attr;
	};

#endif // ! VF_RM_CURVE_SWITCH_H