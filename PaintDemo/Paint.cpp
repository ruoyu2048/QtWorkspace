#include "Paint.h"
#include "ui_Paint.h"

#include <QDebug>

Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);

    InitMainWindow();
    Connections();
}

Paint::~Paint()
{
    delete ui;
}

void Paint::InitMainWindow()
{
    m_pBtnUndo = new QPushButton("撤销");
    m_pBtnClear = new QPushButton("清除");
    m_pBtnSave = new QPushButton("保存");
    m_pBtnRecover = new QPushButton("恢复");
    m_pBtnUndo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pBtnClear->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pBtnSave->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pBtnRecover->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pBtnsSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);

    m_pHBtnsLayout = new QHBoxLayout();
    m_pHBtnsLayout->addWidget(m_pBtnUndo);
    m_pHBtnsLayout->addWidget(m_pBtnClear);
    m_pHBtnsLayout->addWidget(m_pBtnSave);
    m_pHBtnsLayout->addWidget(m_pBtnRecover);
    m_pHBtnsLayout->addSpacerItem(m_pBtnsSpacer);

    m_pPaintView = new CPaintView(this);
    m_pPaintView->setAutoFillBackground(true);
    m_pPaintView->show();

    m_pMainLayout = new QGridLayout(this);
    m_pMainLayout->addLayout(m_pHBtnsLayout,0,0,1,5);
    m_pMainLayout->addWidget(m_pPaintView,1,0,1,5);

    setLayout(m_pMainLayout);
    showMaximized();
}

void Paint::Connections()
{
    connect(m_pBtnUndo,SIGNAL(pressed()),this,SLOT(BtnUndo()));
    connect(m_pBtnClear,SIGNAL(pressed()),this,SLOT(BtnClear()));
    connect(m_pBtnSave,SIGNAL(pressed()),this,SLOT(BtnSave()));
    connect(m_pBtnRecover,SIGNAL(pressed()),this,SLOT(BtnRecover()));
}

void Paint::BtnUndo(){
    m_pPaintView->PVUndo();
}

void Paint::BtnClear(){
    m_pPaintView->PVClear();
}

void Paint::BtnSave(){
    m_pPaintView->PVSave();
}

void Paint::BtnRecover(){
    m_pPaintView->PVRecover();
}
