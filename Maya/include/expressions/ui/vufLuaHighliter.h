#ifndef VF_MAYA_LUA_HIGHLITER_H
#define VF_MAYA_LUA_HIGHLITER_H

#include <QtGui/qsyntaxhighlighter.h>

namespace vufRM
{
	class vufLuaHighliter : public QSyntaxHighlighter
	{
	public:
		vufLuaHighliter(QTextDocument* parent);
	};
}
#endif // !VF_MAYA_LUA_HIGHLITER_H
