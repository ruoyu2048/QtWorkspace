#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "PluginManager.h"
#include "PluginInterface.h"

#include <QMessageBox>
#include <QMdiSubWindow>
#include <QDockWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    iniForm();
    loadPlugin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniForm()
{
    setMinimumSize(QSize(1024,768));
    QObject::connect(ui->mainToolBar, SIGNAL(actionTriggered(QAction*)), SLOT(actionTriggeredSlots(QAction* )));
}

void MainWindow::loadPlugin()
{
    PluginManager *manager = new PluginManager(this);
    QMap<QString, PluginInterface*> map = manager->FindPlugin();
    QMap<QString, PluginInterface*>::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {
        i.value()->widget()->setWindowTitle(i.key());
        _MapWidget[i.key()] = i.value()->widget();
        _MapDockWidgetArea[i.key()] = i.value()->duck_DockWidgetArea();

        if(i.value()->duck_Widget() == DUCK_WIDGETTYPE::MDIAREA){
            ui->mainToolBar->addAction(QIcon(i.value()->iconPath()),i.value()->name());
            ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        }
        if(i.value()->duck_Widget() == DUCK_WIDGETTYPE::DOCKWIDGET){
            addDockWidget(i.value()->duck_DockWidgetArea(), Duck_Widget(i.value()->widget()));
            ui->menuView->addAction(Duck_Widget(i.value()->widget())->toggleViewAction());

        }
        ++i;
    }
    ui->mainToolBar->toolButtonStyleChanged(Qt::ToolButtonTextUnderIcon);
    delete manager;
}

void MainWindow::loadPluginWidget()
{
    PluginManager *manager = new PluginManager(this);
    QMap<QString, PluginInterface*> map = manager->FindPlugin();
    QMap<QString, PluginInterface*>::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {
        _MapWidget[i.key()] = i.value()->widget();
        _MapDockWidgetArea[i.key()] = i.value()->duck_DockWidgetArea();
        ++i;
    }

    delete manager;
}

QDockWidget *MainWindow::Duck_Widget(QWidget *widget)
{
    QDockWidget *dock = new QDockWidget(widget->windowTitle());
    dock->setFeatures(QDockWidget::DockWidgetMovable);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setWidget(widget);
    return dock;
}

void MainWindow::actionTriggeredSlots(QAction *action)
{
    if(ui->mdiArea->subWindowList().count() != 0){
        for(int i =0 ; i < ui->mdiArea->subWindowList().count(); i++){
            if(ui->mdiArea->subWindowList().at(i)->windowTitle() == action->text()){
                return;
            }
        }
    }
    ui->mdiArea->addSubWindow(_MapWidget[action->text()],Qt::SubWindow);
    _MapWidget[action->text()]->setWindowIcon(action->icon());
    _MapWidget[action->text()]->setWindowTitle(action->text());
    _MapWidget[action->text()]->show();

    loadPluginWidget();
}
