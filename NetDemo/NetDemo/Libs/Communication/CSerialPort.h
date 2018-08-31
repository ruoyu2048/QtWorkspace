#ifndef CSERIALPORT_H
#define CSERIALPORT_H

#include <QObject>

class CSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit CSerialPort(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CSERIALPORT_H