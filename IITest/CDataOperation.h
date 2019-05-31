#ifndef CDATAOPERATION_H
#define CDATAOPERATION_H

#include <QObject>
#include <Communication.h>
class CDataOperation : public QObject
{
    Q_OBJECT
public:
    explicit CDataOperation(QObject *parent = nullptr);

    bool startDataOperation( CommunicationCfg commCfg );
    void stopDataOperation();
    bool sendData( QByteArray sendAry );

signals:
    void receivedEncodeAry( QByteArray encodeAry );

private slots:
    void onReadData(QByteArray recvAry);

private:
    Communication* m_pCommu;
    QByteArray m_cacheAry;
};

#endif // CDATAOPERATION_H
