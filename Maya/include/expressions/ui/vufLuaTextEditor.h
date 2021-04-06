#ifndef VF_MAYA_LUA_EXPR_EDITOR_H
#define VF_MAYA_LUA_EXPR_EDITOR_H

#include <QtWidgets/qplaintextedit.h>

namespace vufRM
{
	class vufLuaExprWindow;
	class vufLuaTextEditor : public QPlainTextEdit
	{
	public:
		vufLuaTextEditor(vufLuaExprWindow* p_expr_wnd, QWidget* parent = 0);
		virtual void keyPressEvent(QKeyEvent* p_event) override;

	private:
		vufLuaExprWindow* m_wnd_ptr;
	};
}
#endif // !VF_MAYA_LUA_EXPR_EDITOR_H
