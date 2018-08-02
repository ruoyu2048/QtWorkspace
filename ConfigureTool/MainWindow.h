#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QGridLayout>
#include <QList>
#include "XML.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void InitMainWindow();
    void InitMainTab();

private:
    XML*                m_pXML;
    QGridLayout*        m_pGLayout;
    QTabWidget*         m_pTabMain;
    QTreeWidget*        m_pCurTree;
    QTreeWidgetItem*    m_pCurSubItem;
    QTreeWidgetItem*    m_pCurClass;
    QTreeWidgetItem*    m_pCurItem;
    QList<QTreeWidget*> m_treeWidgetList;

signals:

public slots:

private slots:
    void createRaderItem(QString strRaderName);
    void addRaderSubItem(QString strSubName);
    void addClassItem(QString strClassName);
    void addItem(QString strName);
    void addData(QString strName,QString dataType,QString dataValue);
};

#endif // MAINWINDOW_H
