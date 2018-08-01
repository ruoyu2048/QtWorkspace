#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMainWindow()
{
    InitToolBar();
    InitMainView();
}

void MainWindow::InitToolBar()
{
    m_pActSave = new QAction(QIcon(":/images/save.png"),tr("保存"));
    m_pActSave->setToolTip(tr("保存"));
    m_pActSave->setCheckable(true);

    m_pActSelected = new QAction(QIcon(":/images/pointer.png"),tr("选择"));
    m_pActSelected->setToolTip(tr("选择图元"));
    m_pActSelected->setCheckable(true);
    m_pActSelected->setChecked(true);

    m_pActLine = new QAction(QIcon(":/images/linepointer.png"),tr("画线"));
    m_pActLine->setToolTip(tr("画线"));
    m_pActLine->setCheckable(true);

    m_pActEllipse = new QAction(QIcon(":/images/circle.png"),tr("画椭圆"));
    m_pActEllipse->setToolTip(tr("画椭圆"));
    m_pActEllipse->setCheckable(true);

    m_pActRectangle = new QAction(QIcon(":/images/rectangle.png"),tr("画矩形"));
    m_pActRectangle->setToolTip(tr("画矩形"));
    m_pActRectangle->setCheckable(true);

    m_pActRotation = new QAction(QIcon(":/images/rotation.png"),tr("旋转图元"));
    m_pActRotation->setToolTip(tr("旋转图元"));
    m_pActRotation->setCheckable(true);

    ui->mainToolBar->addAction(m_pActSave);
    ui->mainToolBar->addAction(m_pActSelected);
    ui->mainToolBar->addAction(m_pActLine);
    ui->mainToolBar->addAction(m_pActEllipse);
    ui->mainToolBar->addAction(m_pActRectangle);
    ui->mainToolBar->addAction(m_pActRotation);

    connect(m_pActSave,SIGNAL(triggered()),this,SLOT(ActSave()));
    connect(m_pActSelected,SIGNAL(triggered()),this,SLOT(ActSelected()));
    connect(m_pActLine,SIGNAL(triggered()),this,SLOT(ActLine()));
    connect(m_pActEllipse,SIGNAL(triggered()),this,SLOT(ActEllipse()));
    connect(m_pActRectangle,SIGNAL(triggered()),this,SLOT(ActRectangle()));
    connect(m_pActRotation,SIGNAL(triggered()),this,SLOT(ActRotation()));
}

void MainWindow::InitMainView()
{
    m_pDrawView = new QGraphicsView(this);
    this->setCentralWidget(m_pDrawView);
}

//private slots:
void MainWindow::ActSave()
{

}

void MainWindow::ActSelected()
{

}

void MainWindow::ActLine()
{

}

void MainWindow::ActEllipse()
{

}

void MainWindow::ActRectangle()
{

}

void MainWindow::ActRotation()
{

}
