#ifndef CDATADEAL_H
#define CDATADEAL_H

#include <QObject>
#include "Communication.h"
#include "CommunicationCfg.h"

class CDataDeal : public QObject
{
    Q_OBJECT
public:
    explicit CDataDeal(QObject *parent = nullptr);
    void dataTest(uchar cCmdId, QStringList attrList);
signals:

public slots:
private:
    CommunicationCfg udpCfg;
    Communication comm;
};

#endif // CDATADEAL_H
