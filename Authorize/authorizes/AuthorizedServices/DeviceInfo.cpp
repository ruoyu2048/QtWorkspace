#include "DeviceInfo.h"

#include <QProcess>
#include <QHostInfo>
#include <QNetworkInterface>

DeviceInfo::DeviceInfo(QObject *parent) : QObject(parent)
{

}

const QString DeviceInfo::sysName()
{
    return QSysInfo::prettyProductName();
}

const QString DeviceInfo::machineName()
{
    return QSysInfo::machineHostName();
}

const QStringList DeviceInfo::ip()
{
    QStringList strIpList;

    QList<QNetworkInterface> netList = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface item, netList){
        if((QNetworkInterface::IsUp & item.flags()) && (QNetworkInterface::IsRunning & item.flags()) && item.type()!=QNetworkInterface::Virtual)
        {
            //过滤掉虚拟网卡
            if( !item.hardwareAddress().startsWith("00:50:56:C0") && !item.hardwareAddress().isEmpty() ){
                QList<QNetworkAddressEntry> entryList = item.addressEntries();
                foreach(QNetworkAddressEntry entry, entryList) {
                    if( QAbstractSocket::IPv4Protocol == entry.ip().protocol() && QHostAddress::LocalHost != entry.ip() ) {
                        strIpList.push_back(entry.ip().toString());
                    }
                }
            }
        }
    }
    return strIpList;
}

const QStringList DeviceInfo::mac()
{
    QStringList strMacList;
    QList<QNetworkInterface> netList = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface item, netList){
        if((QNetworkInterface::IsUp & item.flags()) && (QNetworkInterface::IsRunning & item.flags()) && item.type()!=QNetworkInterface::Virtual)
        {
            //过滤掉虚拟网卡
            if( !item.hardwareAddress().startsWith("00:50:56:C0") && !item.hardwareAddress().isEmpty() ){
                strMacList.push_back(item.hardwareAddress());
            }
        }
    }
    return strMacList;
}

const QString DeviceInfo::baseboardNum()
{
#ifdef Q_OS_WIN
    return getWMIC("wmic baseboard get serialnumber");
#elif Q_OS_LINUX

#endif
}

const QString DeviceInfo::harddiskNum()
{
#ifdef Q_OS_WIN
    return getWMIC("wmic diskdrive where index=0 get serialnumber");
#elif Q_OS_LINUX

#endif
}

QString DeviceInfo::getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result;
}
