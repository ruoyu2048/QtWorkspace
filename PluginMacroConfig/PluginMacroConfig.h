#ifndef PLUGINMACROCONFIG_H
#define PLUGINMACROCONFIG_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTabWidget>
#include <QComboBox>
#include <QLabel>

class PluginMacroConfig : public QWidget
{
    Q_OBJECT

public:
    PluginMacroConfig(QWidget *parent = 0);
    ~PluginMacroConfig();
private:
    void initMainWindow();
    void initContrals();
    void initConfigurationManage();
    void initConfigurationDisplayTab();
    void initConfigurationDisplay();


private slots:
    void onCBCfgFileName(int nIndex);
    void onBtnStartCfg();
    void onBtnSaveCfg();
    void onBtnGetCfg();
    void onBtnAddCfg();
    void onBtnDeleteCfg();
    void onBtnJobRelationCfg();

    void onItemChanged(QTreeWidgetItem* pItem,int column);
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
    QTreeWidget* m_pCfgDisplayTree;
};

#endif // PLUGINMACROCONFIG_H
