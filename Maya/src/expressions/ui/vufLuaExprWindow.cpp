#include <expressions/ui/vuLuafExprWindow.h>
#include <expressions/ui/vufLuaTextEditor.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <expressions/vufMayaLuaExpressionNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnPluginData.h>
//#include <maya/MPlugArray.h>
#include <utils/vufMayaUtils.h>
#include <expressions/vufMayaLuaTxtNode.h>
#include <data/vufMayaDataList.h>

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
		MString l_node_name		= l_node.name();
		auto l_port_node_name	= vufMayaUtils::get_input_connected_node_name(l_node, vufMayaLuaExpressionNode::g_ports_script_attr);
		auto l_script_node_name	= vufMayaUtils::get_input_connected_node_name(l_node, vufMayaLuaExpressionNode::g_script_attr);
		
		std::wstring l_node_name_wstr	= vufMayaUtils::mstr_2_wstr(l_node_name);
		std::wstring l_port_name_wstr	= vufMayaUtils::mstr_2_wstr(l_port_node_name);
		std::wstring l_script_name_wstr = vufMayaUtils::mstr_2_wstr(l_script_node_name);

		vufExpresionPair l_pair;
		l_pair.m_port_node_name		= l_port_name_wstr;
		l_pair.m_script_node_name	= l_port_name_wstr;
		m_expression_nodes_map[l_node_name_wstr] = l_pair;
		m_expression_nodes_names.push_back(l_node_name_wstr);
	}
}
void vufLuaExprWindow::refresh_explorer()
{
	m_lua_nodes_explorer->clear();
	if (m_expression_nodes_names.size() == 0)
	{
		return;
	}
	int l_selected_ndx = -1;

	for (uint64_t i = 0; i < m_expression_nodes_names.size(); ++i)
	{
		m_lua_nodes_explorer->addItem(vufMayaUtils::wstr_2_qstr( m_expression_nodes_names[i]));
		if (m_selected_lua == m_expression_nodes_names[i])
		{
			l_selected_ndx = i;
		}
	}
	if (l_selected_ndx != -1)
	{
		m_lua_nodes_explorer->setCurrentRow(l_selected_ndx);
	}	
}
void vufLuaExprWindow::text_changed(const QString& p_text, const vufLuaTextEditor* p_text_editor_ptr)
{
	MStatus l_status;
	int l_index = m_lua_nodes_explorer->currentRow();
	m_selected_lua = m_expression_nodes_names[l_index];

	if (p_text_editor_ptr == m_port_editor_ptr)
	{

	}
	if (p_text_editor_ptr == m_script_editor_ptr)
	{

	}
}

void vufLuaExprWindow::selection_changed()
{
	MStatus l_status;
	int l_index = m_lua_nodes_explorer->currentRow();
	m_selected_lua = m_expression_nodes_names[l_index];

	MString l_port_node_name	= vufMayaUtils::wstr_2_mstr( m_expression_nodes_map[m_selected_lua].m_port_node_name);
	MString l_script_node_name	= vufMayaUtils::wstr_2_mstr(m_expression_nodes_map[m_selected_lua].m_script_node_name);


	MObject l_port_obj		= vufMayaUtils::get_object_by_name(l_port_node_name);
	MObject l_script_obj	= vufMayaUtils::get_object_by_name(l_script_node_name);

	if (l_port_obj.isNull() == true)
	{
		VF_LOG_INFO("port object is absent");
		return;
	}
	if (l_script_obj.isNull() == true)
	{
		VF_LOG_INFO("script object is absent");
		return;
	}

	MFnDependencyNode l_port_node(l_port_obj, &l_status);
	MFnDependencyNode l_script_node(l_script_obj, &l_status);

	// read data
	std::shared_ptr<vuf::vufTxt> l_port_in_data		= get_inner_data<mpxMayaLuaTxtWrapper, vufMayaLuaTxtData, vuf::vufTxt>(	l_port_node, vufMayaLuaTxtNode::g_expression_attr);
	std::shared_ptr<vuf::vufTxt> l_script_in_data	= get_inner_data<mpxMayaLuaTxtWrapper, vufMayaLuaTxtData, vuf::vufTxt>( l_script_node, vufMayaLuaTxtNode::g_expression_attr);

	if (l_port_in_data == nullptr || l_script_in_data == nullptr)
	{
		VF_LOG_INFO("internal script object is absent");
		return;
	}
	QString l_port_txt		= vufMayaUtils::str_2_qstr(l_port_in_data->get_script());
	QString l_script_txt	= vufMayaUtils::str_2_qstr(l_script_in_data->get_script());

	m_port_editor_ptr->setPlainText(l_port_txt);
	m_script_editor_ptr->setPlainText(l_script_txt);
}
