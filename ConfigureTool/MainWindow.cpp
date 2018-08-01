#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    InitMainWindow();
}

void MainWindow::InitMainWindow()
{
    InitMainTab();
}

void MainWindow::InitMainTab()
{
    m_pGLayout = new QGridLayout(this);
    m_pTabMain = new QTabWidget(this);
    m_pTabMain->addTab(new QTreeWidget(),"aa");
    m_pTabMain->addTab(new QTreeWidget(),"bb");
    m_pTabMain->addTab(new QTreeWidget(),"cc");

    m_pGLayout->addWidget(m_pTabMain,0,0,1,1);
}
