#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QGridLayout>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void InitMainWindow();
    void InitMainTab();

private:
    QGridLayout* m_pGLayout;
    QTabWidget* m_pTabMain;

signals:

public slots:
};

#endif // MAINWINDOW_H
