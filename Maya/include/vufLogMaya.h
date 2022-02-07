#ifndef VF_MAYA_LOG_H
#define VF_MAYA_LOG_H
#include <vufLog.h>
#include <utils/vufMayaUtils.h>

#define VF_MAYA_LOG_INFO(MSTRING) \
{\
	auto l_str = vufMayaUtils::mstr_2_str(MSTRING);\
	VF_LOG_INFO(l_str);\
}
#define VF_MAYA_LOG_WARN(MSTRING) \
{\
	auto l_str = vufMayaUtils::mstr_2_str(MSTRING);\
	VF_LOG_WARN(l_str);\
}
#define VF_MAYA_LOG_ERR(MSTRING) \
{\
	auto l_str = vufMayaUtils::mstr_2_str(MSTRING);\
	VF_LOG_ERR(l_str);\
}
#define VF_MAYA_NODE_LOG_INFO(MSTRING)	\
{	\
	auto l_str = vufMayaUtils::mstr_2_str(this->name() +":> " + MSTRING);\
	VF_LOG_INFO(l_str);\
}
#define VF_MAYA_NODE_LOG_WARN(MSTRING)	\
{	\
	auto l_str = vufMayaUtils::mstr_2_str(this->name() +":> " + MSTRING);\
	VF_LOG_WARN(l_str);\
}
#define VF_MAYA_NODE_LOG_ERR(MSTRING)	\
{	\
	auto l_str = vufMayaUtils::mstr_2_str(this->name() +":> " + MSTRING);\
	VF_LOG_ERR(l_str);\
}


namespace vuf
{

	class  vufLogMaya :public vuf::vufLog
	{
	public:
		virtual void info(		const std::string&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void warning(	const std::string&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void error(		const std::string&, const char* p_file = nullptr, int p_line = -1) override;

		virtual void info(		const std::wstring&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void warning(	const std::wstring&, const char* p_file = nullptr, int p_line = -1) override;
		virtual void error(		const std::wstring&, const char* p_file = nullptr, int p_line = -1) override;

	};

}
#endif // !VF_MAYA_LOG_H
