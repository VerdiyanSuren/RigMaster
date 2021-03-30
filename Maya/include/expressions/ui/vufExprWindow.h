#ifndef VF_MAYA_EXPR_WND_H
#define VF_MAYA_EXPR_WND_H

#include <QMainWindow>
#include <highlighter.h>
#include <QShortcut>
#include "QSettings"
#include <QPlainTextEdit>

//#include "files.h"
//#include "search.h"
//#include "utils/conversion.h"
//#include "utils/templates.h"


namespace vufRM
{
    class vufExprWindow : public QMainWindow
    {
        //Q_OBJECT
    public:
        explicit vufExprWindow(QWidget* parent = 0);
        ~vufExprWindow();
        // void openWith(QString);
    };
}

#endif // MAINWINDOW_H
