#ifndef FILENAMEDLG_H
#define FILENAMEDLG_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

class FileNameDlg : public QDialog
{
    Q_OBJECT
public:
    FileNameDlg();

    QString getFileName();

signals:

public slots:

private slots:
    void onBtnYes();
    void onBtnNo();

private:
    QGridLayout* m_pGLMain;
    QLineEdit*  m_pLEFileName;
    QPushButton* m_pBtnYes;
    QPushButton* m_pBtnNo;
};

#endif // FILENAMEDLG_H
