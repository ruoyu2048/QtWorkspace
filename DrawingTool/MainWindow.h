#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QGraphicsView>

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
    void InitMainWindow();
    void InitToolBar();
    void InitMainView();
private:
    Ui::MainWindow *ui;
    QAction* m_pActSave;
    QAction* m_pActSelected;
    QAction* m_pActLine;
    QAction* m_pActEllipse;
    QAction* m_pActRectangle;
    QAction* m_pActRotation;
    QGraphicsView* m_pDrawView;
private slots:
    void ActSave();
    void ActSelected();
    void ActLine();
    void ActEllipse();
    void ActRectangle();
    void ActRotation();
};

#endif // MAINWINDOW_H
