#ifndef VF_MAYA_DATA_LIST_H
#define VF_MAYA_DATA_LIST_H

#include <data/vufData.h>
#include <vufLuaWrapper.h>
#include <vufMatrix.h>
#include <expressions/data/vufMayaLuaPortInternalData.h>
#include <curves/vufCurveContatiner.h>
namespace vufRM
{

	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxMayaLuaTxtWrapper,		vufMayaLuaTxtData,			vuf::vufTxt);
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxMayaLuaPortWrapper,	vufMayaLuaPortData,			vufMayaLuaPortInternalData);
	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxCurveWrapper,			vufCurveContainerData_4d,	vufMath::vufCurveContainer_4d);
	//VF_RM_DECLARE_STANDART_DATA_CLASS(mpxTransformListWrapper,	vufTransformListData,	vufMath::vufMatrixArrayObject_4d);
	//add new datas here

	/*
	class vufMayaLuaTxtData :public vufData
	{
	public:
		VF_RM_DECLARE_DATA_BODY(vufMayaLuaTxtData);

		std::shared_ptr<vuf::vufTxt> m_txt_ptr = nullptr;
	};
	*/
	//using mpxMayaLuaTxtWrapper = vufDataTemplate<vufMayaLuaTxtData>;
}


#endif // !VF_MAYA_DATA_LIST_H
