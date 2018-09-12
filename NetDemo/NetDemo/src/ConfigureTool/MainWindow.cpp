#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent){
    InitMainWindow();
}

void MainWindow::InitMainWindow(){
    this->setMinimumSize(600,400);
    m_pHLayout = new QHBoxLayout(this);
    m_pTabMain = new CTabMain(this);
    m_pHLayout->addWidget(m_pTabMain);
}
