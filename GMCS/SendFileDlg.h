#ifndef SENDFILEDLG_H
#define SENDFILEDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

class SendFileDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SendFileDlg(QWidget *parent = 0);
    void InitCentralWidget();
    void Connections();

private:
    QLabel* m_pLabelFile;
    QLabel* m_pLabelDesPath;
    QLineEdit* m_pSrcFile;
    QLineEdit* m_pDesPath;
    QPushButton* m_pBtnBrowse;
    QSpacerItem* m_pCentralSpcerItem;
    QSpacerItem* m_pEndSpcerItem;
    QPushButton* m_pBtnSend;
    QPushButton* m_pBtnCancel;
    QHBoxLayout* m_pHSrcFileLayout;
    QHBoxLayout* m_pHDesPathLayout;
    QHBoxLayout* m_pHBtnLayout;
    QVBoxLayout* m_pMainLayout;

signals:

public slots:

private slots:
    void Browse();
    void Send();
    void Cancel();
};

#endif // SENDFILEDLG_H
