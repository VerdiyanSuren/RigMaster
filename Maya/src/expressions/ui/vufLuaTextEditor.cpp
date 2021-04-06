#include <expressions/ui/vufLuaTextEditor.h>
#include <iostream>

using namespace vufRM;

vufLuaTextEditor::vufLuaTextEditor(vufLuaExprWindow* p_expr_wnd, QWidget* parent) 
	:QPlainTextEdit(parent)
	, m_wnd_ptr(p_expr_wnd)
{}
void vufLuaTextEditor::keyPressEvent(QKeyEvent* p_event)
{
	if ( (p_event->key() == Qt::Key_Return)  && 
		 (p_event->modifiers() & Qt::ControlModifier) )
	{
		std::cout << "Got it" << std::endl;
		// optional: if the QPlainTextEdit should do its normal action 
		// even when the return button is pressed, uncomment the following line
		// QPlainTextEdit::keyPressEvent( event )
		m_wnd_ptr->text_changed(toPlainText(), this);
		/* do your stuff here */
		p_event->accept();
	}
	else
	{
		QPlainTextEdit::keyPressEvent(p_event);
	}
}
