#ifndef XML_H
#define XML_H

#include <QObject>
#include "CTreeItem.h"

class XML : public QObject
{
    Q_OBJECT
public:
    explicit XML(QObject *parent = nullptr);
    bool getConfigureInfo(QString strConfigPath);

private:
    /************************************************************************
    *函数名:	getAllRaderConfigDirPath
    *概述:获取根目录下所有雷达配置信息存储文件夹
    *参数：strConfigPath--雷达信息配置文件存储文件夹路径
    *返回值：如果获取成功，则返回true，否则返回false
    ************************************************************************/
    bool getAllRaderConfigDirPath(QString strConfigPath);

    /************************************************************************
    *函数名:	parseConfigurationFiles
    *概述:解析雷达配置文件
    *参数：无
    *返回值：解析成功返回true，否则返回false
    ************************************************************************/
    bool parseConfigurationFiles();

private:
    /************************************************************************
    *函数名:	getEquipmentListFile
    *概述:获取当前雷达配置信息汇总XML文件路径
    *参数：strPath--当前雷达配置信息存储文件夹路径
    *     strEquipListFile--当前雷达配置信息汇总XML文件路径
    *返回值：如果获取成功，则返回true，否则返回false
    ************************************************************************/
    bool getEquipmentListFile(QString strPath,QString &strEquipListFile);

    /************************************************************************
    *函数名:	readEquipmentList
    *概述:解析当前雷达配置信息汇总XML文件
    *参数：strPath--当前雷达配置信息存储文件夹路径
    *     readyLoadFiles--预加载的子项配置文件名称集
    *返回值：如果解析成功，则返回true，否则返回false
    ************************************************************************/
    int readEquipmentList(QString strPath,Rader_Total &raderTotal,QSet<QString> &readyLoadFiles);

    /************************************************************************
    *函数名:	readAllEquipmentInfo
    *概述:解析当前雷达所有子项配置文件信息
    *参数：strPath--当前雷达子项配置文件路径
    *     readyLoadFiles--预加载的子项配置文件名称集
    *返回值：如果解析成功，则返回true，否则返回false
    ************************************************************************/
    int readAllEquipmentInfo(QString strPath,Rader_Total &raderTotal,QSet<QString> readyLoadFiles);

    /************************************************************************
    *函数名:	readEquipmentInfo
    *概述:解析当前雷达子项配置文件信息
    *参数：strPath--当前雷达子项配置文件路径
    *返回值：如果解析成功，则返回true，否则返回false
    ************************************************************************/
    int readEquipmentInfo(QString strPath,Subject &subject);

private:
    uchar hexToByteArray(QString strHex);

public:
    QList<Rader_Total> mRaderTotals;//总机列表
private:
    QStringList mPathDirs;//配置文件的文件夹目录列表
signals:

public slots:
};

#endif // XML_H
