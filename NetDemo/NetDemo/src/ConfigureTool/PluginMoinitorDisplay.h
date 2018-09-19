#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QTabWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "XML.h"
#include "DataProcess.h"
#include "CDataPacket.h"
#include "Communication.h"
#include "CommunicationCfg.h"

class SubMachine;
class PluginMoinitorDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit PluginMoinitorDisplay(QWidget *parent = nullptr);
    void InitMainWindow();
    void setSubMachine(QString strName);

protected:
    virtual void closeEvent(QCloseEvent* ev)override;

private:
    QGridLayout*        m_pGLayout;
    QHBoxLayout*        m_pHLayout_Main;
    QHBoxLayout*        m_pHLayout;
    SubMachine*         m_pSubMachine;
signals:

public slots:
};

class SubMachine : public QTabWidget
{
    Q_OBJECT
public:
    explicit SubMachine(QWidget *parent = nullptr);
    void InitTabMain();
    void switchSubMachine(QString strName);

private:
    uchar hexToByteArray(QString strHex);

private:
    XML*                m_pXML;
    quint8              m_curSubId;                 //当前分机ID
    QTreeWidget*        m_pCurTree;                 //记录当前Tab页
    QList<quint8>       m_subDevIds;                //分机ID列表
    QList<QString>      m_subDevNames;              //分机名称
    QList<QTreeWidget*> m_subDevTabs;               //分机树列表
    QMap<QLineEdit*,QStringList>m_lineEditTips;     //编辑框的正则表达式信息
    QMap<QPushButton*,QTreeWidgetItem*>m_btnMap;    //当前设置按钮与控制属性父节点的映射关系

    DataProcess         m_DataProcess;

signals:

private slots:
    void currentTabChanged(int index);
    void lineTextEdited(QString strText);
    void btnSetClicked();
};

#endif // MAINWINDOW_H
