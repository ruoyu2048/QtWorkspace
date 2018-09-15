#ifndef TST_H
#define TST_H

#include <QObject>
#include "Communication.h"
#include "CommunicationCfg.h"
#include "CDataPacket.h"

class Tst : public QObject
{
    Q_OBJECT
public:
    explicit Tst(QObject *parent = nullptr);

signals:

public slots:
    void test(CDataPacket* dataPkt);
private:
    CommunicationCfg udpCfg;
    Communication cumm;
};

#endif // TST_H
