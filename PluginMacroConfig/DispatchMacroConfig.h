#ifndef DISPATCHMACROCONFIG_H
#define DISPATCHMACROCONFIG_H

#include <QObject>
#include <QTimer>

struct ParaID_Value{
    QString strParaId;//参数Id
    QString strParaVal;//参数值
};

struct CMD_PV{
    QString strCMDTag;  //命令标识
    QList<ParaID_Value> pvList;//参数ID_Value列表
};

struct SubMacroConfig{//分机设备宏配置参数
    QString strSubDevId;//分机Id
    QList<CMD_PV> cpvList;//命令参数列表
};

class DispatchMacroConfig : public QObject
{
    Q_OBJECT
public:
    explicit DispatchMacroConfig(QObject *parent = nullptr);

    bool startDispatch(QString strFilePath);

private:
    bool initSubMacroConfigList( QString strFilePath);
    void dispatchFirst();
signals:
    void dispatchFinished();

    /**
     * @brief publishMacroCfg 发布宏配置报文
     * @param strSubDevId 分机设备Id
     * @param strCmd 当前命令Id
     * @param strKeyValues 命令参数信息，格式：参数ID|参数值,参数ID|参数值,...
     */
    void publishMacroCfg(QString strSubDev,QString strCmd,QString strKeyValues);

public slots:
    void onTimer();
    void onDispatchNext();

private:
    int m_nRadarId;
    QList<SubMacroConfig>m_smcList;
};

#endif // DISPATCHMACROCONFIG_H
