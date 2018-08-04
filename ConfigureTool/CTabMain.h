#ifndef CTABMAIN_H
#define CTABMAIN_H

#include <QTabWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "XML.h"
#include "CtrlDelegate.h"

class CTabMain : public QTabWidget
{
    Q_OBJECT
public:
    explicit CTabMain(QWidget *parent = nullptr);
    void InitTabMain();

private:
    XML*                m_pXML;
    CtrlDelegate*       m_pCtrlDelegate;
    QPushButton*        m_pBtnSaveSet;//保存设置
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
    void changeCurTree(int nIndex);
    void treeCellClicked(QTreeWidgetItem* pTreeItem,int nCol);
};

#endif // CTABMAIN_H
