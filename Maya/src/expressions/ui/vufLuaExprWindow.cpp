#include <expressions/ui/vuLuafExprWindow.h>
#include <expressions/ui/vufLuaTextEditor.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnPluginData.h>
//#include <maya/MPlugArray.h>
#include <utils/vufMayaUtils.h>
#include <expressions/nodes/vufMayaLuaExpressionNode.h>
#include <expressions/nodes/vufMayaLuaTxtNode.h>
#include <expressions/nodes/vufMayaLuaPortNode.h>
#include <expressions/luaWrappers/vufMayaLuaPorts.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
//#include <maya/MDependen>
using namespace vufRM;

vufLuaExprWindow::vufLuaExprWindow(QWidget* parent) : QMainWindow(parent)
{
	QSplitter* l_vertical_splitter		= new QSplitter(Qt::Vertical);
	QSplitter *l_horizontal_splitter	= new QSplitter(Qt::Horizontal);
	
	m_port_editor_ptr					= new vufLuaTextEditor(this);
	m_script_editor_ptr					= new vufLuaTextEditor(this);
	m_lua_nodes_explorer				= new QListWidget();
	l_vertical_splitter->addWidget(m_port_editor_ptr);
	l_vertical_splitter->addWidget(m_script_editor_ptr);

	l_horizontal_splitter->addWidget(m_lua_nodes_explorer);
	l_horizontal_splitter->addWidget(l_vertical_splitter);
	
	setCentralWidget(l_horizontal_splitter);

	connect(m_lua_nodes_explorer, SIGNAL(itemSelectionChanged()),this, SLOT(selection_changed()));
	
	refresh_expression_nodes();
	refresh_explorer();
}
vufLuaExprWindow::~vufLuaExprWindow()
{

}
void vufLuaExprWindow::refresh_expression_nodes()
{
	MStatus l_status;
	m_expression_nodes_names.clear();
	m_expression_nodes_map.clear();

	MItDependencyNodes l_iterator(MFn::kPluginDependNode, &l_status);
	for (; !l_iterator.isDone(); l_iterator.next())
	{
		MFnDependencyNode l_node(l_iterator.thisNode());
		if (l_node.typeId() != vufMayaLuaExpressionNode::g_id) 
		{
			continue;
		}
		MString l_node_name			= l_node.name();
		MString l_port_node_name	= vufMayaUtils::get_input_connected_node_name(l_node, vufMayaLuaExpressionNode::g_ports_script_attr);
		MString l_script_node_name	= vufMayaUtils::get_input_connected_node_name(l_node, vufMayaLuaExpressionNode::g_script_attr);
		
		// To Do check absentconnected node
		std::string l_node_name_str		= vufMayaUtils::mstr_2_str(l_node_name);
		std::string l_port_name_str		= vufMayaUtils::mstr_2_str(l_port_node_name);
		std::string l_script_name_str	= vufMayaUtils::mstr_2_str(l_script_node_name);

		vufExpresionPair l_pair;
		l_pair.m_port_node_name		= l_port_name_str;
		l_pair.m_script_node_name	= l_script_name_str;
		m_expression_nodes_map[l_node_name_str] = l_pair;
		m_expression_nodes_names.push_back(l_node_name_str);
	}
}
void		vufLuaExprWindow::refresh_explorer()
{
	std::string l_selected_node_name = get_selected_item_name();

	m_lua_nodes_explorer->clear();
	if (m_expression_nodes_names.size() == 0)
	{
		return;
	}
	int l_selected_ndx = -1;

	for (uint64_t i = 0; i < m_expression_nodes_names.size(); ++i)
	{
		m_lua_nodes_explorer->addItem(vufMayaUtils::str_2_qstr( m_expression_nodes_names[i]));
		if (l_selected_node_name == m_expression_nodes_names[i])
		{
			l_selected_ndx = i;
		}
	}
	if (l_selected_ndx != -1)
	{
		m_lua_nodes_explorer->setCurrentRow(l_selected_ndx);	
		m_lua_nodes_explorer->item(l_selected_ndx)->setSelected(true);
	}	
}
void		vufLuaExprWindow::text_changed(const QString& p_text, const vufLuaTextEditor* p_text_editor_ptr)
{	
	if (p_text_editor_ptr == m_port_editor_ptr)
	{
		port_text_changed();
		return;
	}
	if (p_text_editor_ptr == m_script_editor_ptr)
	{

	}
}
void		vufLuaExprWindow::port_text_changed()
{
	MStatus l_status;
	// Get selected expression node from explorer 
	auto l_items_list = m_lua_nodes_explorer->selectedItems();
	if (l_items_list.size() == 0)
	{
		VF_LOG_ERR("Error to get selected item from explorer");
		return;
	}
	std::string l_expr_node_name = vufMayaUtils::qstr_2_str(l_items_list.first()->text());
	QString l_txt = m_port_editor_ptr->toPlainText();
	
	// Get_port node 
	MString l_port_node_name = vufMayaUtils::str_2_mstr(m_expression_nodes_map[l_expr_node_name].m_port_node_name);
	MObject l_port_obj = vufMayaUtils::get_object_by_name(l_port_node_name);
	if (l_port_obj.isNull() == true)
	{
		VF_LOG_ERR("port object is absent");
		return;
	}
	MFnDependencyNode l_port_node(l_port_obj, &l_status);
	std::shared_ptr<vufMayaLuaPortInternalData > l_port_in_data = get_inner_data<mpxMayaLuaPortWrapper, vufMayaLuaPortData, vufMayaLuaPortInternalData>(l_port_node, vufMayaLuaPortNode::g_port_attr);
	if (l_port_in_data == nullptr)
	{
		VF_LOG_ERR("internal script object is absent");
		return;
	}
	std::string l_script = vufMayaUtils::qstr_2_str(l_txt);
	//lua machine to connect nodes
	std::string l_string_to_python = lua_connection_eval(l_expr_node_name, l_script, l_port_in_data);
	// eval puthon script to connect nodes



	l_port_in_data->m_lua_txt.set_txt(vufMayaUtils::qstr_2_str(l_txt));
	//VF_LOG_INFO(vufMayaUtils::qstr_2_str(l_txt));

	// demand 
	MPlug l_demand_plug = l_port_node.findPlug(vufMayaLuaTxtNode::g_demand_attr, true, &l_status);
	double l_val;
	l_demand_plug.getValue(l_val);
	l_val = l_val != .0 ? .0 : 1;
	l_demand_plug.setValue(l_val);

}
std::string vufLuaExprWindow::get_selected_item_name() const
{
	std::string l_selected_node_name;
	auto l_items_list = m_lua_nodes_explorer->selectedItems();
	if (l_items_list.size() != 0)
	{
		return  vufMayaUtils::qstr_2_str(l_items_list.first()->text());
	}
	return std::string();
}

std::string vufLuaExprWindow::lua_connection_eval(std::string& expression_node_name, const std::string& p_lua_script, std::shared_ptr<vufMayaLuaPortInternalData > p_data)
{
	std::string l_node_name = expression_node_name;
	p_data->clear_ports();
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	vufMayaLuaPorts::register_ports_for_editor(L, &l_node_name, p_data.get());
	int l_result = luaL_dostring(L, p_lua_script.c_str());
	if (l_result != LUA_OK)
	{
		std::string l_err = lua_tostring(L, -1);
		VF_LOG_ERR(l_err);
	}
	lua_close(L);
	return std::string();
}

void vufLuaExprWindow::selection_changed()
{
	auto l_items_list = m_lua_nodes_explorer->selectedItems();
	if (l_items_list.size() == 0)
	{
		VF_LOG_ERR("Fialed to get selection node");
	}
	MStatus l_status;
	std::string l_expr_node_name = vufMayaUtils::qstr_2_str(l_items_list.first()->text());

	//VF_LOG_INFO(l_expr_node_name);
	//VF_LOG_INFO(m_expression_nodes_map[l_expr_node_name].m_port_node_name);
	//VF_LOG_INFO(m_expression_nodes_map[l_expr_node_name].m_script_node_name);

	MString l_port_node_name	= vufMayaUtils::str_2_mstr( m_expression_nodes_map[l_expr_node_name].m_port_node_name);
	MString l_script_node_name	= vufMayaUtils::str_2_mstr(m_expression_nodes_map[l_expr_node_name].m_script_node_name);


	MObject l_port_obj		= vufMayaUtils::get_object_by_name(l_port_node_name);
	MObject l_script_obj	= vufMayaUtils::get_object_by_name(l_script_node_name);

	if (l_port_obj.isNull() == true)
	{
		VF_LOG_ERR("port object is absent");
		return;
	}
	if (l_script_obj.isNull() == true)
	{
		VF_LOG_ERR("script object is absent");
		return;
	}

	MFnDependencyNode l_port_node(l_port_obj, &l_status);
	if (l_status != MS::kSuccess)
	{
		VF_LOG_ERR("Failed to set dependecy node");
		return;
	}
	MFnDependencyNode l_script_node(l_script_obj, &l_status);
	if (l_status != MS::kSuccess)
	{
		VF_LOG_ERR("Failed to set dependecy node");
		return;
	}

	// read data
	std::shared_ptr<vufMayaLuaPortInternalData> l_port_in_data	= get_inner_data<mpxMayaLuaPortWrapper, vufMayaLuaPortData, vufMayaLuaPortInternalData>(	l_port_node, vufMayaLuaPortNode::g_port_attr);
	std::shared_ptr<vuf::vufTxt> l_script_in_data	= get_inner_data<mpxMayaLuaTxtWrapper, vufMayaLuaTxtData, vuf::vufTxt>( l_script_node, vufMayaLuaTxtNode::g_expression_attr);

	if (l_port_in_data == nullptr || l_script_in_data == nullptr)
	{
		VF_LOG_ERR("internal script object is absent");
		return;
	}
	QString l_port_txt		= vufMayaUtils::str_2_qstr(l_port_in_data->m_lua_txt.get_script());
	QString l_script_txt	= vufMayaUtils::str_2_qstr(l_script_in_data->get_script());

	//VF_LOG_INFO(l_port_in_data->m_lua_txt.get_script());
	//VF_LOG_INFO(vufMayaUtils::qstr_2_str(l_port_txt));
	m_port_editor_ptr->setPlainText(l_port_txt);
	m_script_editor_ptr->setPlainText(l_script_txt);
}
