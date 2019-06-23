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

struct SubCfgInfo{
    QString strName;//分机名称
    QString strSubId;//分机ID
    QString strBaseName;//文件名(无后缀)
    QString strAbsFilePath;//分机配置文件绝对路径
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

    void showWindow(int nRadarId);


    void resetSubCfgDisplayTabItem(int nRadarId);

    bool getSubCfgInfo(int nRadarId,QList<SubCfgInfo> &cfgInfoList);

    bool questionMsgBox(QString strInfo);

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
};

#endif // PLUGINMACROCONFIG_H
