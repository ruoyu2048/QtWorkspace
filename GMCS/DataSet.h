#ifndef DATASET_H
#define DATASET_H

#include <QMap>
#include <QVector>
#include <QString>
#include <QObject>
#include <QMutex>
#include <QVariant>
#include <QDomComment>
#include "TreeItem.h"
#include "DataStruct.h"

class DataSet : public QObject
{
    Q_OBJECT
public:
    explicit DataSet(QObject *parent = 0);
    ~DataSet();

    //XML File Operating
    bool OpenXMLFile(QString strFileName);
    void SetLayoutCfgFilePath(QString strFilePath);
    void SetAttributeValue(QString strXMLFile,QString strItemId,QString strAttr,QString strVal);
    QString GetAttributeValue(QString strXMLFile,QString strItemId,QString strAttr);
private:
    QDomDocument m_DomConnment;

public:
    TreeItem* GetRoot();
    TreeItem* FindInAllItems(QString itemId);
    TreeItem* FindHostItem(QString itemId);
    QString FindHostID(QString itemIP,QString transferHostIP,bool bInface = false);
    void SetupModelData(QString strXMLFile);
    void MessageBoxEx(QString errorInfo, bool bQuit = false);

public:
   QString m_strLayoutCfgFilePath;
   //It's invisible root
   TreeItem* m_root;
   QMap<QString,TreeItem*>m_AllItems;
   QMap<QString,TreeItem*>m_AllModels;
   QMap<QString,TreeItem*>m_AllHosts;
   QVector<HostOnlineInfo*>m_vHostOnlineInfo;
   QMutex m_AllItemsMutex;
   QMutex m_AllModelsMutex;
   QMutex m_AllHostsMutes;
   QMutex m_XMLMutex;



signals:
   void quitApplication();

public slots:

};

#endif // DATASET_H
