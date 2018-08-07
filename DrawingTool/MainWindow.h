#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTimer>
#include <QGraphicsView>
#include "DrawScene.h"

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
    QAction*        m_pActSave;
    QAction*        m_pActSelected;
    QAction*        m_pActLine;
    QAction*        m_pActEllipse;
    QAction*        m_pActRectangle;
    QAction*        m_pActRotation;
    QActionGroup*   m_pActGroup;

    QAction*        m_pActToFront;
    QAction*        m_pActToBack;

    QGraphicsView*  m_pDrawView;
    DrawScene*      m_pDrawScene;

    QTimer*         m_pTimer;
private slots:
    void ActSave();

    void ActSelected();
    void ActLine();
    void ActEllipse();
    void ActRectangle();
    void ActRotation();

    void ActBringToFront();
    void ActSendToBack();

    void ActAddShape();

    void UpdateActions();
};

#endif // MAINWINDOW_H
