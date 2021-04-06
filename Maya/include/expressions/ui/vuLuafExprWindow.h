#ifndef VF_MAYA_LUA_EXPR_WND_H
#define VF_MAYA_LUA_EXPR_WND_H

#include <QtWidgets/QMainWindow>
//#include <highlighter.h>
#include <QtWidgets/QShortcut>
#include <QtCore/QSettings>
#include <QtWidgets/QPlainTextEdit>
#include <maya/MString.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qlistwidget.h>
#include <map>
#include <vector>

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
		void refresh_expression_nodes();
		void refresh_explorer();
		void text_changed(const QString& p_text , const vufLuaTextEditor* p_text_editor_ptr);
	public slots:
		void selection_changed();

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
				m_port_node_name = p_other.m_port_node_name;
				m_script_node_name = p_other.m_script_node_name;
				return *this;
			}

			std::wstring m_port_node_name;
			std::wstring m_script_node_name;
		};

		std::vector<std::wstring>					m_expression_nodes_names;
		std::map<std::wstring, vufExpresionPair>	m_expression_nodes_map;
		std::wstring								m_selected_lua;

		vufLuaTextEditor*	m_port_editor_ptr		= nullptr;
		vufLuaTextEditor*	m_script_editor_ptr		= nullptr;
		QListWidget*		m_lua_nodes_explorer	= nullptr;
	};
}

#endif // !VF_MAYA_LUA_EXPR_WND_H
