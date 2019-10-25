#include "WireShell.h"
#include <QApplication>
#include "LoginDlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    LoginDlg loginDlg;
//    if( QDialog::Accepted==loginDlg.exec() ){
//        WireShell w;
//        w.show();
//    }
    WireShell w;
    w.show();
    return a.exec();
}
