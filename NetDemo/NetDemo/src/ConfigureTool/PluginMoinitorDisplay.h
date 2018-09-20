#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QTimer>
#include <QTabWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "XML.h"
#include "CDataPacket.h"
#include "Communication.h"
#include "CommunicationCfg.h"

struct RowItem{
    uchar               subID;          //分机ID
    QString             strId;          //参数ID
    QString             strType;        //Ctrl、Monitor
    QString             dataType;       //数据类型
    qint32              dataLen;        //数据长度
    QString             displayType;    //显示类型，select、text
    QString             paramType;      //select参数列表
    QTreeWidgetItem*    pAttrItem;      //绑定的位置
    QString             displayValue;   //用于存储界面上的数值
};
typedef QMap<QString,RowItem*> RowItemMap;

class DataProcess : public QObject
{
    Q_OBJECT
public:
    explicit DataProcess(QObject *parent = nullptr);
    void startDataProcess(uchar subID);
    void resetSubDevID(uchar subID);

    RowItem* getRowItem(QString strId);
    void updateRowItem(QString strId,QString itemVal);
    /**
     * @brief updateRowItemMap
     * @param subID--当前分机ID
     * @param attr--参数属性对象
     * @param strType--参数类型(控制参数，监控参数）
     * @param pAttrItem--与当前参数绑定的显示控件
     */
    void updateRowItemMap(uchar subID,Attr &attr,QString strType,QTreeWidgetItem* pAttrItem=NULL);
    void mappingToCmd(uchar cmd,QList<RowItem*> attrList);
private:
    void startCommu();
    void startResetTimer();
    void packMessage(uchar cmd,QList<RowItem*> attrList);
    void updateDSP1State(DSP1 &dsp1);
signals:

public slots:

private slots:
    void resetStateDisplay();
    void updateTabView(CDataPacket* dataPkt);

private:
    quint8                  m_SubID;        //分机ID
    quint32                 m_ResetFlag;    //用于标记报文是否已更新
    QTimer*                 m_pResetTimer;  //如果长时间无法收到当前分机的报文，清空状态信息
    QMap<quint8,RowItemMap> mSubRowItemMap;//分机参数字典

    CommunicationCfg    mCommCfg;
    Communication       mComm;
    QTreeWidgetItem*    mpEnt;
};

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
