#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = nullptr);

    const QString sysName();

    const QString machineName();

    const QStringList ip();

    const QStringList mac();

    const QString baseboardNum();

    const QString harddiskNum();


private:
    QString getWMIC(const QString &cmd);


signals:

public slots:
};

#endif // DEVICEINFO_H
