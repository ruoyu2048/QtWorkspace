#ifndef XML_H
#define XML_H

#include <QObject>
#include <QList>
#include <QTreeWidgetItem>

struct Attr{
    bool    show;           //是否显示
    bool    addBtn;         //是否需要添加设置按钮
    QString strID;          //属性ID
    QString dataType;       //数据类型
    quint32 dataLen;        //数据长度
    QString displayType;    //显示类型，select、text
    QString name;           //名称
    QString displayName;    //显示名称
    QString paramType;      //参数类型
    QString defValue;       //默认值
    QString iniFile;        //当前属性参数的配置数据ini文件，Key=displayName
    QString validator;      //编辑框的正则表达式
    QString tips;           //编辑框的错误提示
};

struct Entity{
        QString name;
        QString displayName;
        QString strCmd;
        uchar   cmd;
        QList<Attr> attrs;
        };

struct SubjectInfo{
        QString name;
        QString displayName;
        QString type;
        QString strCmdType;
        uchar   cmdType;
        QList<Entity>ents;
        };

struct Subject{
        QString name;           //分机标识
        QString displayName;    //分机名称
        QString strSubId;       //分机编码
        uchar   cSubId;         //分机编号
        QList<SubjectInfo>subjectInfos;
        };

struct Rader_Total{
        QString         name;       //总机标识
        QString         displayName;//总机名称
        QString         cfgFile;    //分机配置
        QList<Subject>  subjects;   //分机列表
        };

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
