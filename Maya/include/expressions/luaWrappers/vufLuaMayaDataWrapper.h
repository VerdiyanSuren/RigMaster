#ifndef VF_LUA_MAYA_DATA_WRAWPPER_H
#define VF_LUA_MAYA_DATA_WRAWPPER_H

#include <vufLuaUserDataWrapper.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>
#include <maya/MEulerRotation.h>

namespace vufRM
{
	using vufLuaMVectorWrapper		= vuf::vufLuaDataWrapper<MVector>;
	using vufLuaMPointWrapper		= vuf::vufLuaDataWrapper<MPoint>;
	using vufLuaMMattrixWrapper		= vuf::vufLuaDataWrapper<MMatrix>;
	using vufLuaMVectorArrayWrapper = vuf::vufLuaDataWrapper<MVectorArray>;
	using vufLuaMPointArrayWrapper	= vuf::vufLuaDataWrapper<MPointArray>;
	using vufLuaMQuaternionWrapper	= vuf::vufLuaDataWrapper<MQuaternion>;
	using vufLuaMEulerRotaionWrapper = vuf::vufLuaDataWrapper<MEulerRotation>;

}
#endif // !VF_LUA_MAYA_DATA_WRAWPPER_H
