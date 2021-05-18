#ifndef VF_LUA_MAYA_DATA_WRAWPPER_H
#define VF_LUA_MAYA_DATA_WRAWPPER_H

#include <vufLuaUserDataWrapper.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>
#include <maya/MEulerRotation.h>

#include <maya/MVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>

namespace vufRM
{
	using vufLuaMVectorWrapper			= vuf::vufLuaDataWrapper<MVector>;
	using vufLuaMPointWrapper			= vuf::vufLuaDataWrapper<MPoint>;
	using vufLuaMMattrixWrapper			= vuf::vufLuaDataWrapper<MMatrix>;
	using vufLuaMQuaternionWrapper		= vuf::vufLuaDataWrapper<MQuaternion>;
	using vufLuaMEulerRotaionWrapper	= vuf::vufLuaDataWrapper<MEulerRotation>;

	using vufLuaMVectorArrayWrapper		= vuf::vufLuaDataWrapper<MVectorArray>;
	using vufLuaMPointArrayWrapper		= vuf::vufLuaDataWrapper<MPointArray>;
	using vufLuaMIntArrayWrapper		= vuf::vufLuaDataWrapper<MIntArray>;
	using vufLuaMDoubleArrayWrapper		= vuf::vufLuaDataWrapper<MDoubleArray>;

}
#endif // !VF_LUA_MAYA_DATA_WRAWPPER_H
