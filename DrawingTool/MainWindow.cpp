#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QtSvg/QSvgGenerator>

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
    //m_pActSave->setCheckable(true);

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

    m_pActToFront = new QAction(QIcon(":/images/bringtofront.png"),tr("旋转图元"));
    m_pActToFront->setToolTip(tr("旋转图元"));

    m_pActToBack = new QAction(QIcon(":/images/sendtoback.png"),tr("旋转图元"));
    m_pActToBack->setToolTip(tr("旋转图元"));

    ui->mainToolBar->addAction(m_pActSave);
    ui->mainToolBar->addAction(m_pActSelected);
    ui->mainToolBar->addAction(m_pActLine);
    ui->mainToolBar->addAction(m_pActEllipse);
    ui->mainToolBar->addAction(m_pActRectangle);
    ui->mainToolBar->addAction(m_pActRotation);
    ui->mainToolBar->addAction(m_pActToFront);
    ui->mainToolBar->addAction(m_pActToBack);

    m_pActGroup = new QActionGroup(this);
    m_pActGroup->addAction(m_pActSelected);
    m_pActGroup->addAction(m_pActLine);
    m_pActGroup->addAction(m_pActEllipse);
    m_pActGroup->addAction(m_pActRectangle);
    m_pActGroup->addAction(m_pActRotation);
    m_pActSelected->setChecked(true);

    connect(m_pActSave,SIGNAL(triggered()),this,SLOT(ActSave()));

    connect(m_pActSelected,SIGNAL(triggered()),this,SLOT(ActSelected()));
    connect(m_pActLine,SIGNAL(triggered()),this,SLOT(ActLine()));
    connect(m_pActEllipse,SIGNAL(triggered()),this,SLOT(ActEllipse()));
    connect(m_pActRectangle,SIGNAL(triggered()),this,SLOT(ActRectangle()));
    connect(m_pActRotation,SIGNAL(triggered()),this,SLOT(ActRotation()));

    connect(m_pActToFront,SIGNAL(triggered()),this,SLOT(ActBringToFront()));
    connect(m_pActToBack,SIGNAL(triggered()),this,SLOT(ActSendToBack()));

    connect(m_pActSave,SIGNAL(triggered()),this,SLOT(ActAddShape()));
    connect(m_pActSelected,SIGNAL(triggered()),this,SLOT(ActAddShape()));
    connect(m_pActLine,SIGNAL(triggered()),this,SLOT(ActAddShape()));
    connect(m_pActEllipse,SIGNAL(triggered()),this,SLOT(ActAddShape()));
    connect(m_pActRectangle,SIGNAL(triggered()),this,SLOT(ActAddShape()));
    connect(m_pActRotation,SIGNAL(triggered()),this,SLOT(ActAddShape()));

    m_pTimer = new QTimer(this);
    //应将此处更改为信号槽
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(UpdateActions()));
    m_pTimer->start(100);
}

void MainWindow::InitMainView()
{
    m_pDrawView = new QGraphicsView(this);

    //初始化场景
    m_pDrawScene = new DrawScene(this);
    //设置场景区域
    m_pDrawScene->setSceneRect(QRectF(-800 / 2 , -600 / 2 , 800, 600));
    //视图绑定场景
    m_pDrawView->setScene(m_pDrawScene);
    //设置渲染碰撞类型(反锯齿状绘图)
    m_pDrawView->setRenderHint(QPainter::Antialiasing);
    //设置场景的背景图
    m_pDrawScene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    //场景更新
    m_pDrawScene->update();
    //视图更新
    m_pDrawView->update();

    this->setCentralWidget(m_pDrawView);
}

//private slots:
void MainWindow::ActSave()
{
    QString path;
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save SVG"),
        path, tr("SVG files (*.svg)"));

    if (newPath.isEmpty())
        return;

    path = newPath;

//![configure SVG generator]
    QSvgGenerator generator;
    generator.setFileName(path);
    generator.setSize(QSize(800, 600));
    generator.setViewBox(QRect(0, 0, 800, 600));
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                                "Example provided with Qt."));
//![configure SVG generator]
//![begin painting]
    QPainter painter;
    painter.begin(&generator);
//![begin painting]
//!
    m_pDrawScene->clearSelection();
    m_pDrawScene->render(&painter);
//![end painting]
    painter.end();
}

void MainWindow::ActSelected(){
    m_pActSelected->setChecked(true);
}

void MainWindow::ActLine(){
    m_pActLine->setChecked(true);
}

void MainWindow::ActEllipse(){
    m_pActEllipse->setChecked(true);
}

void MainWindow::ActRectangle(){
    m_pActRectangle->setChecked(true);
}

void MainWindow::ActRotation(){
    m_pActRotation->setChecked(true);
}

void MainWindow::ActBringToFront()
{
    //设置QGraphicsItem中的栈顺序
}

void MainWindow::ActSendToBack()
{
    //设置QGraphicsItem中的栈顺序
}

void MainWindow::ActAddShape()
{
    if ( sender() == m_pActEllipse )
        DrawTool::c_drawShape = ellipse;
    if ( sender() == m_pActRectangle )
        DrawTool::c_drawShape = rectangle;
    if ( sender() == m_pActSelected )
        DrawTool::c_drawShape = selection;
    if (sender() == m_pActRotation )
        DrawTool::c_drawShape = rotation;
    if ( sender() == m_pActLine )
        DrawTool::c_drawShape = line;
}

void MainWindow::UpdateActions()
{
    if( selection == DrawTool::c_drawShape )
        m_pActSelected->setChecked(true);
}
