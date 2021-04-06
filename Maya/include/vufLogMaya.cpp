#include <vufLogMaya.h>
#include <maya/MGlobal.h>
#include <utils/vufMayaUtils.h>
using namespace vuf;
using namespace vufRM;

void vufLogMaya::info(const std::string& p_str, const char* p_file, int p_line)
{
	MString l_str = vufMayaUtils::str_2_mstr(p_str);
	if (p_line != -1)
	{
		l_str += "	in line "; 
		l_str += p_line;
	}
	if (p_file != nullptr)
	{
		l_str += "	in file ";
		l_str += MString(p_file);
	}
	MGlobal::displayInfo(l_str);
}
void vufLogMaya::warning(const std::string& p_str, const char* p_file, int p_line)
{
	MString l_str = vufMayaUtils::str_2_mstr(p_str);
	if (p_line != -1)
	{
		l_str += "	in line ";
		l_str += p_line;
	}
	if (p_file != nullptr)
	{
		l_str += "	in file ";
		l_str += MString(p_file);
	}
	MGlobal::displayWarning(l_str);
}
void vufLogMaya::error(const std::string& p_str, const char* p_file, int p_line)
{
	MString l_str = vufMayaUtils::str_2_mstr(p_str);
	if (p_line != -1)
	{
		l_str += "	in line ";
		l_str += p_line;
	}
	if (p_file != nullptr)
	{
		l_str += "	in file ";
		l_str += MString(p_file);
	}
	MGlobal::displayError(l_str);
}

void vufLogMaya::info(const std::wstring& p_str, const char* p_file, int  p_line)
{
	MString l_str = vufMayaUtils::wstr_2_mstr(p_str);
	if (p_line != -1)
	{
		l_str += "	in line ";
		l_str += p_line;
	}
	if (p_file != nullptr)
	{
		l_str += "	in file ";
		l_str += MString(p_file);
	}
	MGlobal::displayInfo(l_str);
}
void vufLogMaya::warning(const std::wstring& p_str, const char* p_file, int p_line)
{
	MString l_str = vufMayaUtils::wstr_2_mstr(p_str);
	if (p_line != -1)
	{
		l_str += "	in line ";
		l_str += p_line;
	}
	if (p_file != nullptr)
	{
		l_str += "	in file ";
		l_str += MString(p_file);
	}
	MGlobal::displayWarning(l_str);
}
void vufLogMaya::error(const std::wstring& p_str, const char* p_file, int  p_line)
{
	MString l_str = vufMayaUtils::wstr_2_mstr(p_str);
	if (p_line != -1)
	{
		l_str += "	in line ";
		l_str += p_line;
	}
	if (p_file != nullptr)
	{
		l_str += "	in file ";
		l_str += MString(p_file);
	}
	MGlobal::displayError(l_str);
}