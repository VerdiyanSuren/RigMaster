#ifndef VF_HRCHY_DOC_MNGR_H
#define VF_HRCHY_DOC_MNGR_H

#include <string>
#include <memory>
#include "vufCoreInclude.h"

namespace  vuf
{	
	class VF_API vufTreeJsonObject
	{
	public:
		vufTreeJsonObject();
		bool save_to_json_file(		const std::wstring& file_path);
		bool save_to_xml_file(		const std::wstring& file_path);
		bool save_to_ini_file(		const std::wstring& file_path);
		bool save_to_info_file(		const std::wstring& file_path);

		bool load_from_json_file(	const std::wstring& file_path);
		bool load_from_xml_file(	const std::wstring& file_path);
		bool load_from_ini_file(	const std::wstring& file_path);
		bool load_from_info_file(	const std::wstring& file_path);

		bool to_wstring(std::wstring&);
		bool from_wstring(const std::wstring&);
		//bool load_from_xml_file(const std::wstring& file_path);

		bool put(const std::wstring& p_key, vufTreeJsonObject p_tree_obj);
		bool put(const std::wstring& p_key, int p_value);
		bool put(const std::wstring& p_key, double p_value);
		bool put(const std::wstring& p_key, bool p_value);
		bool put(const std::wstring& p_key, const std::wstring& p_value);

		bool get(const std::wstring& p_key, vufTreeJsonObject& p_tree_obj);
		bool get(const std::wstring& p_key, int& p_value);
		bool get(const std::wstring& p_key, double& p_value);
		bool get(const std::wstring& p_key, bool& p_value);
		bool get(const std::wstring& p_key, std::wstring& p_value);


		//vufTreeObject get_child(const std::wstring& key);
		//void put_child(vufTreeObject*);

	private:
		std::shared_ptr<void> m_tree_ptr = nullptr;
	};
}
#endif