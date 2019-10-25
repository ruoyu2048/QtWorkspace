#ifndef RUNTEST_H
#define RUNTEST_H

#include <QWidget>

#include "SQLite.h"

namespace Ui {
class RunTest;
}

enum TestType{
    AutoTest,
    SequenceTest
};

class RunTest : public QWidget
{
    Q_OBJECT

public:
    explicit RunTest(QWidget *parent = nullptr);
    ~RunTest();
    void resetTestType(TestType testType);

private:
    void initMainWindow();
    void initSqlite();
    void updateProjectList(TestType testType);

private slots:
    void on_btnStartTest_clicked();

    void on_btnRefreshList_clicked();

private:
    Ui::RunTest *ui;
    TestType m_TestType;

    SQLite* m_pSqlite;
};

#endif // RUNTEST_H
