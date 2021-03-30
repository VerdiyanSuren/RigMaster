#ifndef VF_LOG_H
#define VF_LOG_H

#include <vufCoreInclude.h>
#include <memory>

// if you implement your own loger
// then use this in cpp file to link you logger
#define VF_DEFINE_CUSTOM_LOGGER(CUSTOM_LOG_CLASS) \
	std::shared_ptr<vuf::vufLog> vuf::vufLog::g_log =  std::shared_ptr<vuf::CUSTOM_LOG_CLASS>(new vuf::CUSTOM_LOG_CLASS);

// some predefined loggers
#define VF_DECLARE_STD_LOGGER()																					\
	namespace vuf																								\
	{																											\
		class VF_API vufLogStd :public vufLog																	\
		{																										\
		public:																									\
			virtual void log_info(		const VF_STRING& p_str)	override { std::cout << p_str << std::endl; }	\
			virtual void log_warning(	const VF_STRING& p_str)	override { std::cout << p_str << std::endl; }	\
			virtual void log_error(		const VF_STRING& p_str)	override { std::cout << p_str << std::endl; }	\
			static std::shared_ptr<vufLog> g_log;																\
		};																										\
	}

#define VF_LOG_DEFINE_STD_LOGGER()			\
	VF_DEFINE_CUSTOM_LOGGER(vufLogStd);

namespace vuf
{
	class VF_API vufLog
	{
	public:
		virtual void log_info(		const VF_STRING&)	= 0;

		virtual void log_warning(	const VF_STRING&)	= 0;
		virtual void log_error(		const VF_STRING&)	= 0;
		static std::shared_ptr<vufLog> g_log;
	};

}
#endif // !VF_LOG_H
