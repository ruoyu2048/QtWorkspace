#ifndef WIRESHELL_H
#define WIRESHELL_H

#include <QMainWindow>
#include <QGridLayout>

#include "RunTest.h"
#include "TestDisplay.h"

namespace Ui {
class WireShell;
}

class WireShell : public QMainWindow
{
    Q_OBJECT

public:
    explicit WireShell(QWidget *parent = nullptr);
    ~WireShell();

private:
    void initMainToolBar();
    void initMainWindow();
    void initConnection();

private slots:
    void onActionAutoTest();
    void onActionSequenceTest();
    void onActionUserManage();
    void onActionLogManage();

private:
    Ui::WireShell *ui;

    QWidget*    m_pMainWidget;
    QGridLayout* m_pMainLayout;

    RunTest* m_pRunTest;
    TestDisplay* m_pTestDisplay;

};

#endif // WIRESHELL_H
