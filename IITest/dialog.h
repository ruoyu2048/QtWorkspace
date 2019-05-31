#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

#include "CDataOperation.h"
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


public slots:
    void onTest(QByteArray sss);
private:
    Ui::Dialog *ui;

    CDataOperation mm;
};

#endif // DIALOG_H
