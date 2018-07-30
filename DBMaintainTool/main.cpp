#include "DBMaintainToolDlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBMaintainToolDlg w;
    w.setWindowTitle("数据库维护工具");
    w.setFixedSize(400,146);
    w.show();

    return a.exec();
}
