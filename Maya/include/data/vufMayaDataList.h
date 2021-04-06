#ifndef VF_MAYA_DATA_LIST_H
#define VF_MAYA_DATA_LIST_H

#include <data/vufData.h>
#include <vufLuaWrapper.h>

namespace vufRM
{

	VF_RM_DECLARE_STANDART_DATA_CLASS(mpxMayaLuaTxtWrapper, vufMayaLuaTxtData, vuf::vufTxt);
	
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
