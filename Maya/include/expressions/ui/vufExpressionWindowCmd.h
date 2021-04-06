#ifndef VF_MAYA_EXPR_WND_CMD_H
#define VF_MAYA_EXPR_WND_CMD_H

#include <maya/MPxCommand.h>

namespace vufRM
{
	class vufLuaExprWindow;
	class vufExpressionWindowCmd :public MPxCommand
	{
	public:
		vufExpressionWindowCmd() {}
		virtual ~vufExpressionWindowCmd() {}
		static void*		creator();
		static MSyntax		newSyntax();
		virtual MStatus     doIt(const MArgList& args) override;
		virtual bool        isUndoable() const override { return false; }

		static MString g_type_name;
		static void release_wnd();
	private:
		static vufLuaExprWindow* g_expr_wnd;
	};
}

#endif // !VF_RM
