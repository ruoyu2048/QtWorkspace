#ifndef CTABMAIN_H
#define CTABMAIN_H

#include <QTabWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "XML.h"
#include "CDataDeal.h"

class CTabMain : public QTabWidget
{
    Q_OBJECT
public:
    explicit CTabMain(QWidget *parent = nullptr);
    void InitTabMain();

private:
    XML*                m_pXML;
    QPushButton*        m_pBtnSaveSet;//保存设置
    QTabWidget*         m_pTabMain;
    QTreeWidget*        m_pCurTree;
    QList<QTreeWidget*> m_treeWidgetList;
    QMap<QLineEdit*,QStringList>m_lineEditTips;
    //当前设置按钮与控制属性父节点的映射关系
    QMap<QPushButton*,QTreeWidgetItem*>m_btnMap;

    CDataDeal           mDataDeal;
signals:

private slots:
    void currentTabChanged(int index);
    void lineTextEdited(QString strText);
    void btnSetClicked();
};

#endif // CTABMAIN_H
