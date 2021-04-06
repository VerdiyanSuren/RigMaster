#include "coreItems/vufTreeJsonObject.h"
#include "coreUtils/vufFileUtils.h"
#include "coreUtils/vufStringUtils.h"
#include "vufLog.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

using namespace vuf;
namespace pt = boost::property_tree;

vufTreeJsonObject::vufTreeJsonObject()
{
	m_tree_ptr = std::shared_ptr<pt::wptree>(new pt::wptree());
}
bool vufTreeJsonObject::save_to_json_file(const std::wstring& p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Save_to_json_file is failed. Property tree is absent.");
		return false;
	}

	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		pt::write_json(l_path, *l_pt.get(), std::locale(""));
		//pt::json_parser::write_json(l_path,*l_pt.get(),std::locale(""));
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Save_to_json_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::save_to_xml_file(const std::wstring& p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there isnt ptree object
		VF_LOG_ERR(L"Save_to_xml_file is failed. Property tree is absent.");
		return false;
	}

	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		//boost::property_tree::xml_writer_settings<wchar_t> l_settings(L'\t', 1);
		pt::xml_writer_settings<std::wstring> l_w(L' ', 3);
		pt::write_xml(l_path, *l_pt.get(), std::locale(""),l_w);
		//pt::write_xml(l_path, *l_pt.get(), std::locale(""), l_w);
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Save_to_xml_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::save_to_ini_file(const std::wstring & p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Save_to_ini_file is failed. Property tree is absent.");
		return false;
	}

	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		pt::write_ini(l_path, *l_pt.get(),0,std::locale(""));// , std::locale());
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Save_to_ini_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::save_to_info_file(const std::wstring& p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Save_to_info_file is failed. Property tree is absent.");
		return false;
	}

	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		pt::write_info(l_path, *l_pt.get(),std::locale(""));// , std::locale());
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Save_to_info_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}

bool vufTreeJsonObject::load_from_json_file(const std::wstring& p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"load_from_json_file is failed. Property tree is absent.");
		return false;
	}
	
	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		pt::read_json(l_path, *l_pt.get(), std::locale(""));
		//pt::json_parser::write_json(l_path,*l_pt.get(),std::locale(""));
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("load_from_json_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::load_from_xml_file(const std::wstring& p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"load_from_xml_file is failed. Property tree is absent.");
		return false;
	}

	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		pt::read_xml(l_path, *l_pt.get(),0 ,std::locale(""));
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("load_from_xml_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::load_from_ini_file(const std::wstring& p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"load_from_ini_file is failed. Property tree is absent.");
		return false;
	}

	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		pt::read_ini(l_path, *l_pt.get(), std::locale(""));
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("load_from_ini_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::load_from_info_file(const std::wstring& p_file_path)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"load_from_info_file is failed. Property tree is absent.");
		return false;
	}

	std::string l_path = vufStringUtils::wstring_to_string(p_file_path);
	try
	{
		pt::read_info(l_path, *l_pt.get(), std::locale(""));
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("load_from_info_file is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}

bool vufTreeJsonObject::to_wstring(std::wstring& p_wstr)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"to_wstring is failed. Property tree is absent.");
		return false;
	}

	std::wstringstream l_wss;
	try
	{
		pt::xml_writer_settings<std::wstring> l_w(L' ', 3);
		pt::write_xml(l_wss, *l_pt.get(), l_w);// , std::locale(""));
		//pt::write_json(l_wss, *l_pt.get(),true);// , std::locale(""));
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("to_wstring is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	p_wstr = l_wss.str();
	return true;
}
bool vufTreeJsonObject::from_wstring(const std::wstring& p_wstr)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"from_wstring is failed. Property tree is absent.");
		return false;
	}
	std::wstringstream l_wss;
	l_wss << p_wstr;
	try
	{
		pt::read_xml(l_wss, *l_pt.get());
	}
	catch (std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("from_wstring is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}

bool vufTreeJsonObject::put(const std::wstring& p_key, vufTreeJsonObject p_tree_obj)
{
	std::shared_ptr<pt::wptree> l_pt		= std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	std::shared_ptr<pt::wptree> l_pt_target = std::static_pointer_cast<pt::wptree>(p_tree_obj.m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Put json object is failed. Property tree is null.");
		return false;
	}
	if (l_pt_target == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Put json object is failed. Target property tree is null.");
		return false;
	}
	
	try
	{
		l_pt->add_child(p_key, *l_pt_target.get());
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("Put json object is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::put(const std::wstring& p_key, int p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Put int into json object is failed. Property tree is null.");
		return false;
	}
	try
	{
		l_pt->put(p_key, p_value);

	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Put int into json object is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}

	return true;
}
bool vufTreeJsonObject::put(const std::wstring& p_key, double p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Put double into json object is failed. Property tree is null.");
		return false;
	}
	try
	{
		l_pt->put(p_key, p_value);

	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Put double into json object is failed.  ") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}
bool vufTreeJsonObject::put(const std::wstring& p_key, bool p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Put bool into json object is failed. Property tree is null..");
		return false;
	}
	try
	{
		l_pt->put(p_key, p_value);

	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Put bool into json object is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}

	return true;
}
bool vufTreeJsonObject::put(const std::wstring& p_key, const std::wstring& p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Put wstring into json object is failed. Property tree is null.");
		return false;
	}
	try
	{
		l_pt->put(p_key, p_value);
	}
	catch (const std::exception& p_e)
	{
		std::string l_str = p_e.what();
		l_str = std::string("Put wstring into json object is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	return true;
}

bool vufTreeJsonObject::get(const std::wstring& p_key, vufTreeJsonObject& p_tree_obj)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Get wptree is failed. Property tree is null.");
		return false;
	}
	pt::wptree l_wptree;
	try
	{
		l_wptree = l_pt->get_child(p_key);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("Get wptree is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}

	std::shared_ptr<pt::wptree> l_tree_ptr = std::shared_ptr<pt::wptree>(new pt::wptree(l_wptree));
	p_tree_obj.m_tree_ptr = l_tree_ptr;
	return true;
}
bool vufTreeJsonObject::get(const std::wstring& p_key, int& p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Get int is failed. Property tree is null.");
		return false;
	}
	int l_val;
	try
	{
		l_val = l_pt->get<int>(p_key);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("Get int is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	p_value = l_val;
	return true;
}
bool vufTreeJsonObject::get(const std::wstring& p_key, double& p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Get double is failed. Property tree is null.");
		return false;
	}
	double l_val;
	try
	{
		l_val = l_pt->get<double>(p_key);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("Get double is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	p_value = l_val;
	return true;
}
bool vufTreeJsonObject::get(const std::wstring& p_key, bool& p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Get bool is failed. Property tree is null.");
		return false;
	}
	bool l_val;
	try
	{
		l_val = l_pt->get<bool>(p_key);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("Get bool is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	p_value = l_val;
	return true;
}
bool vufTreeJsonObject::get(const std::wstring& p_key, std::wstring& p_value)
{
	std::shared_ptr<pt::wptree> l_pt = std::static_pointer_cast<pt::wptree>(m_tree_ptr);
	if (l_pt == nullptr)
	{
		// there is'nt ptree object
		VF_LOG_ERR(L"Get wstring is failed. Property tree is null.");
		return false;
	}
	std::wstring l_val;
	try
	{
		l_val = l_pt->get<std::wstring>(p_key);
	}
	catch (const std::exception& l_e)
	{
		std::string l_str = l_e.what();
		l_str = std::string("Get wstring is failed.") + l_str;
		VF_LOG_ERR(vufStringUtils::string_to_wstring(l_str));
		return false;
	}
	p_value = l_val;
	return true;
}

#ifdef VF_DBG
void vufTreeJsonObject::test_me()
{
	std::wcout << std::endl;
	int l_text_color		= VF_CONSOLE_COLOR_LIGHT_GREEN;
	int l_text_error_color	= VF_CONSOLE_COLOR_LIGHT_RED;
	int l_bgrnd_color		= VF_CONSOLE_COLOR_BLACK;
	int l_bgrnd_error_color = VF_CONSOLE_COLOR_BLACK;

	VF_CONSOLE_SET_COLOR(l_text_color, l_bgrnd_color);
	std::wcout << L"Test:> ----------Tree Json Object Start------------" << std::endl;
	vufTreeJsonObject l_obj, l_obj_child;
	vufTreeJsonObject l_obj_json, l_obj_xml, l_obj_ini, l_obj_info;
	if (l_obj_child.put(L"INT_VALUE", 10) == false)
	{
		VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
	}
	if (l_obj_child.put(L"DOUBLE_VALUE", 10.3643287) == false)
	{
		VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
	}
	if (l_obj_child.put(L"BOOL_VALUE", true) == false)
	{
		VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
	}
	if (l_obj_child.put(L"WSTRING_RUS", std::wstring(L"Вот это строка!")) == false)
		//if (obj_child.put(L"WSTRING_ENG", std::wstring(L"THIS IS A STRING!")) == false)
	{
		VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
	}
	if (l_obj_child.put(L"WSTRING_ENG", std::wstring(L"THIS IS A STRING!")) == false)
	{
		VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
	}

	if (l_obj.put(L"CHILD", l_obj_child) == false)
	{
		VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
	}
	std::wcout << L".................................................." << std::endl;
	std::wstring l_wstr_ptree;
	if (l_obj.to_wstring(l_wstr_ptree) == false)
	{
		VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
	}
	else
	{
		std::wcout << l_wstr_ptree << std::endl;
	}
	{
		std::wcout << L".................................................." << std::endl;
		if (l_obj.save_to_json_file(L"C:/temp/test.json") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.json saved." << std::endl;
		}
		if (l_obj.save_to_xml_file(L"C:/temp/test.xml") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.xml saved." << std::endl;
		}
		if (l_obj.save_to_ini_file(L"C:/temp/test.ini") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.ini saved." << std::endl;
		}
		if (l_obj.save_to_info_file(L"C:/temp/test.info") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.info saved." << std::endl;
		}
		std::wcout << L".................................................." << std::endl;
		if (l_obj_json.load_from_json_file(L"C:/temp/test.json") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.json loaded." << std::endl;
		}
		if (l_obj_xml.load_from_xml_file(L"C:/temp/test.xml") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.xml loaded." << std::endl;
		}
		if (l_obj_ini.load_from_ini_file(L"C:/temp/test.ini") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.ini loaded." << std::endl;
		}
		if (l_obj_info.load_from_info_file(L"C:/temp/test.info") == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << L"C:/temp/test.info loaded." << std::endl;
		}
	}
	{
		std::wcout << L".................................................." << std::endl;
		std::wcout << L"READED JSON:>" << std::endl;
		std::wstring l_json_readed;
		if (l_obj_json.to_wstring(l_json_readed) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << l_json_readed << std::endl;

		}
		std::wcout << L".................................................." << std::endl;
		std::wcout << L"READED XML:>" << std::endl;
		std::wstring l_xml_readed;
		if (l_obj_xml.to_wstring(l_xml_readed) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << l_xml_readed << std::endl;
		}
		std::wcout << L".................................................." << std::endl;
		std::wcout << L"READED INI:>" << std::endl;
		std::wstring l_ini_readed;
		if (l_obj_ini.to_wstring(l_ini_readed) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << l_ini_readed << std::endl;
		}
		std::wcout << L".................................................." << std::endl;
		std::wcout << L"READED INFO:>" << std::endl;
		std::wstring l_info_readed;
		if (l_obj_info.to_wstring(l_info_readed) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			std::wcout << l_info_readed << std::endl;
		}
		std::wcout << L".................................................." << std::endl;
	}
	//check readed from json file values
	{
		std::wcout << L".................................................." << std::endl;
		vufTreeJsonObject l_child;
		if (l_obj_json.get(L"CHILD", l_child) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			int l_int;
			double l_double;
			bool l_bool;
			std::wstring l_rus, l_eng;
			if (l_child.get(L"INT_VALUE", l_int) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed json int: " << l_int << std::endl;
			}
			if (l_child.get(L"DOUBLE_VALUE", l_double) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed json double: " << l_double << std::endl;
			}
			if (l_child.get(L"BOOL_VALUE", l_bool) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed json bool: " << l_bool << std::endl;
			}
			if (l_child.get(L"WSTRING_RUS", l_rus) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed json wstring rus: " << l_rus << std::endl;
			}
			if (l_child.get(L"WSTRING_ENG", l_eng) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed json wstring eng: " << l_eng << std::endl;
			}
		}

	}
	// check readed from xmal
	{
		std::wcout << L".................................................." << std::endl;
		vufTreeJsonObject l_child;
		if (l_obj_xml.get(L"CHILD", l_child) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			int l_int;
			double l_double;
			bool l_bool;
			std::wstring l_rus, l_eng;
			if (l_child.get(L"INT_VALUE", l_int) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed xml int: " << l_int << std::endl;
			}
			if (l_child.get(L"DOUBLE_VALUE", l_double) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed xml double: " << l_double << std::endl;
			}
			if (l_child.get(L"BOOL_VALUE", l_bool) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed xml bool: " << l_bool << std::endl;
			}
			if (l_child.get(L"WSTRING_RUS", l_rus) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed xml wstring rus: " << l_rus << std::endl;
			}
			if (l_child.get(L"WSTRING_ENG", l_eng) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed xml wstring eng: " << l_eng << std::endl;
			}
		}

	}
	// check readed from ini
	{
		std::wcout << L".................................................." << std::endl;
		vufTreeJsonObject l_child;
		if (l_obj_ini.get(L"CHILD", l_child) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			int l_int;
			double l_double;
			bool l_bool;
			std::wstring l_rus, l_eng;
			if (l_child.get(L"INT_VALUE", l_int) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed ini int: " << l_int << std::endl;
			}
			if (l_child.get(L"DOUBLE_VALUE", l_double) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed ini double: " << l_double << std::endl;
			}
			if (l_child.get(L"BOOL_VALUE", l_bool) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed ini bool: " << l_bool << std::endl;
			}
			if (l_child.get(L"WSTRING_RUS", l_rus) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed ini wstring rus: " << l_rus << std::endl;
			}
			if (l_child.get(L"WSTRING_ENG", l_eng) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed ini wstring eng: " << l_eng << std::endl;
			}
		}
	}
	// check readed from info
	{
		std::wcout << L".................................................." << std::endl;
		vufTreeJsonObject l_child;
		if (l_obj_info.get(L"CHILD", l_child) == false)
		{
			VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
		}
		else
		{
			int l_int;
			double l_double;
			bool l_bool;
			std::wstring l_rus, l_eng;
			if (l_child.get(L"INT_VALUE", l_int) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed info int: " << l_int << std::endl;
			}
			if (l_child.get(L"DOUBLE_VALUE", l_double) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed info double: " << l_double << std::endl;
			}
			if (l_child.get(L"BOOL_VALUE", l_bool) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed info bool: " << l_bool << std::endl;
			}
			if (l_child.get(L"WSTRING_RUS", l_rus) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed info wstring rus: " << l_rus << std::endl;
			}
			if (l_child.get(L"WSTRING_ENG", l_eng) == false)
			{
				VF_CONSOLE_PUT_ERROR_STR(l_text_error_color, l_bgrnd_error_color, l_text_color, l_bgrnd_color);
			}
			else
			{
				std::wcout << L"Readed info wstring eng: " << l_eng << std::endl;
			}
		}
	}

	std::wcout << L"Test:> -----------Tree Json Object End------------" << std::endl;
	VF_CONSOLE_RESET_COLOR();
	std::wcout << std::endl;

}
#endif

