#ifndef VF_MAYA_UTIL_H
#define VF_MAYA_UTIL_H

#include <maya/MFnDependencyNode.h>
#include <QtCore/qstring.h>
#include <string>

namespace vufRM
{
	class vufMayaUtils
	{
	public:
		// plugs
		static MString get_input_connected_node_name(const MFnDependencyNode& p_node, const MObject& p_attr);
		static MObject get_object_by_name(const MString& p_str);
		static std::string get_plug_type(const MPlug& p_plug);
		// string conversions
		static std::wstring		mstr_2_wstr(const MString& p_mstr);
		static std::string		mstr_2_str(const MString& p_mstr);
		static QString			mstr_2_qstr(const MString& p_mstr);

		static std::string		wstr_2_str(const std::wstring& p_wstr);
		static MString			wstr_2_mstr(const std::wstring& p_wstr);
		static QString			wstr_2_qstr(const std::wstring& p_wstr);

		static std::wstring		str_2_wstr(const std::string& p_str);
		static MString			str_2_mstr(const std::string& p_str);
		static QString			str_2_qstr(const std::string& p_str);

		static MString			qstr_2_mstr(const QString& p_qstr);
		static std::wstring		qstr_2_wstr(const QString& p_qstr);
		static std::string		qstr_2_str(const QString& p_qstr);


	};
}
#endif // !VF_MAYA_UTIL_H
