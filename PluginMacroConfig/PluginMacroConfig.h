#ifndef PLUGINMACROCONFIG_H
#define PLUGINMACROCONFIG_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QFileInfo>
#include <QLabel>

#include "SubConfigDispalyTree.h"
#include "DispatchThread.h"

enum MsgBoxType{
    Quesstion,
    Information
};

class PluginMacroConfig : public QWidget
{
    Q_OBJECT

public:
    PluginMacroConfig(QWidget *parent = 0);
    ~PluginMacroConfig();
private:
    void initMainWindow();
    /**
     * @brief initContrals 按钮初始化
     */
    void initContrals();

    /**
     * @brief initConfigFileNameCommbox 文件列表初始化
     */
    void initConfigFileNameCommbox();

    /**
     * @brief initDeviceCfgManageTree 初始化设备配置管理树控件
     */
    void initDeviceCfgManageTree();

    /**
     * @brief initSubCfgDisplayTab 分机配置信息Tab控件初始化
     */
    void initSubCfgDisplayTab();

    /**
     * @brief showWindow 显示宏配置插件
     * @param nRadarId 雷达Id
     */
    void showWindow(int nRadarId);

    /**
     * @brief clearDeviceCfgManageTreeItems 清除设备配置管理树元素
     */
    void clearDeviceCfgManageTreeItems();

    /**
     * @brief clearSubCfgDisplayTabItems 清除Tab控件中所有分机参数控件
     */
    void clearSubCfgDisplayTabItems();

    /**
     * @brief resetSubCfgDisplayItems 重置Tab项目
     * @param nRadarId
     */
    void resetSubCfgDisplayItems(int nRadarId);

    /**
     * @brief getSubCfgInfo 获取当前雷达分机配置信息
     * @param nRadarId 雷达Id
     * @param cfgInfoList 获取到的分机配置文件信息
     * @return
     */
    bool getSubCfgInfo(int nRadarId,QList<SubCfgInfo> &cfgInfoList);

    bool msgBox(MsgBoxType msgBoxType,QString strInfo);

    /**
     * @brief updateDeviceCfgManageTree 更新设备配置管理树元素勾选状态
     * @param devManageItemCheckedInfo  树节点勾选状态字典<subDevId,checked>
     */
    void updateDeviceCfgManageTree(QMap<QString,bool> devManageItemCheckedInfo);

    /**
     * @brief updateSubCfgDisplayTabItemValue 更新Tab控件中分机参数
     * @param subDevParaMap<分机Id，<参数Id，参数值>>
     */
    void updateSubCfgDisplayTabItemValue(QMap<QString,QMap<QString,QString>> subDevParaMap);

private slots:
    void onCBCfgFileName(int nIndex);
    void onBtnStartCfg();
    void onBtnSaveCfg();
    void onBtnGetCfg();
    void onBtnAddCfg();
    void onBtnDeleteCfg();
    void onBtnJobRelationCfg();

    void onItemChanged(QTreeWidgetItem* pItem,int column);
    void onItemClicked(QTreeWidgetItem* pItem,int column);

    void onCurrentTabChanged(int nIndex);

    void setChildCheckState(QTreeWidgetItem* pItem, Qt::CheckState cs);
    void setParentCheckState(QTreeWidgetItem* pItem);

private:
    int          m_nRadarId;//记录当前雷达Id
    QGridLayout* m_pGLMain;
    QLabel*      m_pLBCfgFileName;
    QComboBox*   m_pCBCfgFileName;

    QPushButton* m_pBtnStartCfg;
    QPushButton* m_pBtnSaveCfg;
    QPushButton* m_pBtnGetCfg;
    QPushButton* m_pBtnAddCfg;
    QPushButton* m_pBtnDeleteCfg;
    QPushButton* m_pBtnJobRelationCfg;

    QTreeWidget* m_pCfgManageTree;
    QTreeWidgetItem* m_pCfgManageRootItem;

    QTabWidget*  m_pCfgDispalyTab;
    SubConfigDispalyTree* m_pCfgSubTree;

    DispatchThread m_dispatchThread;
};

#endif // PLUGINMACROCONFIG_H
