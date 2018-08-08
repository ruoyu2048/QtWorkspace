#ifndef LOGMANAGE_H
#define LOGMANAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QSpacerItem>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>
#include "DB.h"
#include "BtnDelegate.h"
#include "LogTableModel.h"

namespace Ui {
class LogManage;
}

class BtnDelegate;
class LogTableModel;

class LogManage : public QWidget
{
    Q_OBJECT

public:
    explicit LogManage(QWidget *parent = 0);
    ~LogManage();

private:
    void InitMainWindow();
    void InitGroupBox();
    void InitButtons();
    void InitLogView();

    void Connections();

private:
    void UpdateAllPageCountInfo();
    void UpdateTableDisplay();
    void UpdateLablePageInfo();

private:
    Ui::LogManage *ui;

    QVBoxLayout*    m_pMainLayout;
    //GroupBox
    QGroupBox*      m_pGBSearch;
    QHBoxLayout*    m_pGBHALayout;
    QHBoxLayout*    m_pGBHBLayout;

    QLabel*         m_pLBDevName;
    QLineEdit*      m_pLEDevName;
    QLabel*         m_pLBTime;
    QDateEdit*      m_pDEStart;
    QLabel*         m_pLBTo;
    QDateEdit*      m_pDEEnd;
    QSpacerItem*    m_pGBSpacer;

    QPushButton*    m_pBtnSearch;
    QPushButton*    m_pBtnReset;

    //Buttons
    QHBoxLayout*    m_pBtnHLayout;
    QPushButton*    m_pBtnFirst;
    QPushButton*    m_pBtnNext;
    QPushButton*    m_pBtnPrev;
    QPushButton*    m_pBtnLast;
    QComboBox*      m_pCoBRowNum;       //日志显示最大条数
    QLabel*         m_pLBTotal;         //显示总项数
    QLabel*         m_pLBIndex;         //显示当前页信息
    QSpacerItem*    m_pBtnSpacer;


    //table
    int             m_nAllPageCnt;      //总页数
    int             m_nCurPageIdx;      //当前页
    int             m_nMaxRowNum;       //最大显示行数
    int             m_nCurAllRow;       //当前的所有数据条数
    QTableView*     m_pLogTableView;    //表格视图
    QPushButton*    m_pBtnDelete;
    LogTableModel*  m_pLogTableModel;   //表格模型
    BtnDelegate*    m_pBtnDelegate;     //按钮代理，相应删除日志操作

    DB              m_DB;
private slots:
    void BtnSearchClicked();
    void BtnResetClicked();
    void BtnFirstClicked();
    void BtnNextClicked();
    void BtnPrevClicked();
    void BtnLastClicked();
    void ComBRowNumChanged(int nIndex);
    void DeleteRow(QModelIndex modelIndex);
};

#endif // LOGMANAGE_H
