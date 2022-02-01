#ifndef VF_MAYA_DATA_LIST_H
#define VF_MAYA_DATA_LIST_H

#include <data/vufData.h>
#include <vufLuaWrapper.h>
#include <math/vufMatrix.h>
#include <expressions/data/vufMayaLuaPortInternalData.h>
#include <curves/vufCurveContatiner.h>
#include <vufObjectArrayFn.h>
#include <math/vufMatrix.h>
//#include <curves/quaternionFn/vufCurveQuaternionFn.h>
#include <vector>
namespace vufRM
{
	// Lua
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxMayaLuaTxtWrapper,		vufMayaLuaTxtData,			vuf::vufTxt);
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxMayaLuaPortWrapper,	vufMayaLuaPortData,			vufMayaLuaPortInternalData);
	//VF_RM_DECLARE_STANDART_DATA_CLASS(mpxMayaLuaPortWrapper, vufMayaLuaPortData, vufMayaLuaPortInternalData);
	// Transform List
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxMatrixListWrapper,		vufMatrixListData,			vufMath::vufObjectArray<vufMath::vufMatrix_4d>);
	// Curves
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxCurveWrapper,			vufCurveData,				vufMath::vufCurveContainer_4d);
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxCurveQuatWrapper,		vufCurveQuatData,			vufMath::vufCurveQuaternionFn_4d);
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxCurveScaleWrapper,		vufCurveScaleData,			vufMath::vufCurveScaleFn_4d);
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxCurveRebuildWrapper,	vufCurveRebuildData,		vufMath::vufCurveRebuildFn_4d);
	//VF_RM_DECLARE_STANDART_DATA_CLASS(mpxCurveRemapWrapper,		vufCurveRemapData,			vufMath::vufCurveRemapFn<double>);

}


#endif // !VF_MAYA_DATA_LIST_H
