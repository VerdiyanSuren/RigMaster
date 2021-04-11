#ifndef VF_MAYA_LUA_EXPR_WND_H
#define VF_MAYA_LUA_EXPR_WND_H

#include <QtWidgets/QMainWindow>
//#include <highlighter.h>
#include <QtWidgets/QShortcut>
#include <QtWidgets/qaction.h>
#include <QtCore/QSettings>
#include <QtWidgets/QPlainTextEdit>
#include <maya/MString.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qlistwidget.h>
#include <map>
#include <vector>
#include <memory>

#include <data/vufMayaDataList.h>

//#include "files.h"
//#include "search.h"
//#include "utils/conversion.h"
//#include "utils/templates.h"

namespace vufRM
{
	class vufLuaTextEditor;
	class vufLuaExprWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		explicit vufLuaExprWindow(QWidget* parent = 0);
		~vufLuaExprWindow();
		// void openWith(QString);
	private:
		void create_actions();
		void create_menus();
	public:
		void refresh_expression_nodes(); // get list of expression nodes from maya
		void refresh_explorer();
		void text_changed(const QString& p_text , const vufLuaTextEditor* p_text_editor_ptr); //called when any script changed
	private:
		void port_text_changed(); // called when port script edited
		//void script_text_changed();

		std::string get_selected_item_name() const; // name of selected item in explorer
		//bool set_item_selected(const std::string&);
		std::string lua_connection_eval(std::string& expression_node_name, const std::string& p_lua_script, std::shared_ptr<vufMayaLuaPortInternalData > p_data);
	public slots:
		void selection_changed();
		void refresh_editor();
	private:
		struct vufExpresionPair
		{
			vufExpresionPair() {}
			vufExpresionPair(const vufExpresionPair& p_other)
				:m_port_node_name(p_other.m_port_node_name)
				, m_script_node_name(p_other.m_script_node_name)
			{}
			vufExpresionPair& operator=(const vufExpresionPair& p_other)
			{
				m_port_node_name	= p_other.m_port_node_name;
				m_script_node_name	= p_other.m_script_node_name;
				return *this;
			}

			std::string m_port_node_name;
			std::string m_script_node_name;
		};

		std::vector<std::string>				m_expression_nodes_names;
		std::map<std::string, vufExpresionPair>	m_expression_nodes_map;

		vufLuaTextEditor*	m_port_editor_ptr		= nullptr;
		vufLuaTextEditor*	m_script_editor_ptr		= nullptr;
		QListWidget*		m_lua_nodes_explorer	= nullptr;
	
	private:
		QAction* m_action_refresh_window = nullptr;

		QMenuBar	*m_menu_bar = nullptr;
		QMenu		*m_main_mau = nullptr;

	};
}

#endif // !VF_MAYA_LUA_EXPR_WND_H
