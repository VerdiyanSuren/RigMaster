#include <expressions/ui/vufExpressionWindowCmd.h>
#include <expressions/ui/vuLuafExprWindow.h>
#include <maya/MSyntax.h>
#include <maya/MQtUtil.h>
#include <QtCore/qpointer.h>

using namespace vufRM;

vufLuaExprWindow*	vufExpressionWindowCmd::g_expr_wnd = nullptr;
MString				vufExpressionWindowCmd::g_type_name("vufLuaExpressionWindow");

void* vufExpressionWindowCmd::creator()
{
	return new vufExpressionWindowCmd();
}
MSyntax		vufExpressionWindowCmd::newSyntax()
{
	MSyntax l_syntax;
	return l_syntax;
}
MStatus     vufExpressionWindowCmd::doIt(const MArgList& p_args)
{
	MStatus l_status;
	QWidget* l_maya_widget = MQtUtil::mainWindow();

	if (g_expr_wnd == nullptr)
	{
		g_expr_wnd = new vufLuaExprWindow(l_maya_widget);
		//button->connect(button, SIGNAL(clicked(bool)), button, SLOT(createHelix(bool)));
		g_expr_wnd->show();
		return MS::kSuccess;
	}
	g_expr_wnd->showNormal();
	g_expr_wnd->raise();
	return MS::kSuccess;
}
void vufExpressionWindowCmd::release_wnd()
{
	if (g_expr_wnd != nullptr)
	{
		delete g_expr_wnd;
		g_expr_wnd = nullptr;
	}
}

