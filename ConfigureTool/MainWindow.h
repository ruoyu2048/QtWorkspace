#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include "CTabMain.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void InitMainWindow();

private:
    QGridLayout*        m_pGLayout;
    QHBoxLayout*        m_pHLayout;
    QPushButton*        m_pBtnSaveSet;//保存设置
    CTabMain*           m_pTabMain;
signals:

public slots:
};

#endif // MAINWINDOW_H
