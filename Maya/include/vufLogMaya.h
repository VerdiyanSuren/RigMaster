#ifndef VF_MAYA_LOG_H
#define VF_MAYA_LOG_H
#include <vufLog.h>

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
