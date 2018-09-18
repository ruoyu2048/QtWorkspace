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
    void startCommu();
    uchar hexToByteArray(QString strHex);
    void packMessage(QString cCmdId, QStringList attrList);
    void mappingToCmd(QTreeWidgetItem* pEntItem,QStringList strInfos);
private:
    XML*                m_pXML;
    QTreeWidget*        m_pCurTree;
    QList<QString>      m_subMachines;//分机名称
    QList<QTreeWidget*> m_treeWidgetList;//分机树列表
    QMap<QLineEdit*,QStringList>m_lineEditTips;//编辑框的正则表达式信息
    QMap<QPushButton*,QTreeWidgetItem*>m_btnMap;//当前设置按钮与控制属性父节点的映射关系
    QMap<QTreeWidgetItem*,QString>mCtrlMap;//Entity节点与Ids的映射关系，发送命令时使用
    QMap<QString,QTreeWidgetItem*>mMonitorMap;//Ids与Entity的映射关系，接受命令更新界面使用

    CommunicationCfg    mCommCfg;
    Communication       mComm;
    QTreeWidgetItem*    mpEnt;
signals:

private slots:
    void currentTabChanged(int index);
    void lineTextEdited(QString strText);
    void btnSetClicked();
    void updateTabView(CDataPacket* dataPkt);
};

#endif // MAINWINDOW_H
