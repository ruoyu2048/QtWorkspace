#ifndef MTFILETRANSLATE_H
#define MTFILETRANSLATE_H

#include <QWidget>
#include "CHotUpdateServer.h"
#include "CHotUpdateClient.h"
#include "CHotUpdateDataStruct.h"

namespace Ui {
class MTFileTranslate;
}

class MTFileTranslate : public QWidget
{
    Q_OBJECT

public:
    explicit MTFileTranslate(QWidget *parent = nullptr);
    ~MTFileTranslate();


public slots:
    void onClientConnected();
    void onClientDisconnected();
//    void onUpdateSendProcess(FileUpdateInfo fuInfo);
//    void onUpdateReceiveProcess(FileUpdateInfo fuInfo);

    void onUpdateSendProcess(QVariant varSP);
    void onUpdateReceiveProcess(QVariant varRP);

private slots:
    void on_btnStartServer_clicked();

    void on_btnStartClient_clicked();

    void on_btnSelectC_clicked();

    void on_btnSelectS_clicked();

private:
    Ui::MTFileTranslate *ui;

    CHotUpdateServer* pS;
    CHotUpdateClient* pC;
};

#endif // MTFILETRANSLATE_H
