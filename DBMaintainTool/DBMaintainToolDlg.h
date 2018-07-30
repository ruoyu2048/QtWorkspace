#ifndef DBMAINTAINTOOLDLG_H
#define DBMAINTAINTOOLDLG_H

#include <QDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QVector>
#include "DataStruct.h"



namespace Ui {
class DBMaintainToolDlg;
}

class DBMaintainToolDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DBMaintainToolDlg(QWidget *parent = 0);
    ~DBMaintainToolDlg();

    void InitLayout();
    void ConnectToDB();
    void ReadConfigFromTxt();
    void ReadConfigFromDB();
    void GetSimulatorInfoFromDB();
    void GetSimNodePCInfoFromDB();
    void WriteToSimulatorCodeTable();
    void WriteToSimNodePCInfoTable();
    void WriteToSimulatorCodeTableTxt();
    void WriteToSimNodePCInfoTableTxt();
    SimulatorInfo* FindSimulatorInfo(int iSimCode);

public:
    QGroupBox* m_pGroupBox;
    QCheckBox* m_pCBSimCode;
    QCheckBox* m_pCBSimNode;
    QPushButton* m_pBtnOutput;
    QPushButton* m_pBtnInput;
    QVBoxLayout* m_pVBoxlayout;
    QGridLayout* m_pGridLayout;

    bool m_bIsConnect;
    bool m_bSimCode;
    bool m_bSimNode;

//    int m_iAllParentNum;
//    struct SimulatorInfo m_SimulatorInfo[252];
    QVector<SimulatorInfo*> m_vSimulatorInfo;

public slots:
    void CBSimCode();
    void CBSimNode();
    void BtnOutput();
    void BtnInput();

private:
    Ui::DBMaintainToolDlg *ui;
};

#endif // DBMAINTAINTOOLDLG_H
