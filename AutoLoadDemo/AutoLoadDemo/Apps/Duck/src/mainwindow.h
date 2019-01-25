#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void iniForm();
    void loadPlugin();
    void loadPluginWidget();
    QDockWidget *Duck_Widget(QWidget *widget);

private:
    Ui::MainWindow *ui;
    QMap<QString, QWidget *> _MapWidget;
    QMap<QString, Qt::DockWidgetArea> _MapDockWidgetArea;

private slots:
    void actionTriggeredSlots(QAction*);
};

#endif // MAINWINDOW_H
