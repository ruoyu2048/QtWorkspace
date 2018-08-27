#ifndef CTABMAIN_H
#define CTABMAIN_H

#include <QTabWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "XML.h"

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
    QMap<QLineEdit*,QString> m_lineEditMap;
signals:

public slots:
    void saveConfigureInfo();
private slots:
    void currentTabChanged(int index);
    void itemClicked(QModelIndex modelIndex);
    void lineTextEdited(QString strText);
};

#endif // CTABMAIN_H
