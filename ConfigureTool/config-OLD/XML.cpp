#include "XML.h"
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QDebug>

XML::XML(QObject *parent) : QObject(parent)
{

}

bool XML::getAllRaderConfigDirPath(QString strConfigPath)
{
    QDir dir(strConfigPath);
    if( false == dir.exists() )
    {
        qDebug()<<"The path of Radar configuration list is not exit !";
        return false;
    }
    //只读取当前路径下的文件目录
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);

    QFileInfoList  fileInfoList = dir.entryInfoList();
    int nEntryCnt = fileInfoList.count();
    for( int i=0; i<nEntryCnt; ++i )
    {
        QFileInfo fileInfo = fileInfoList.at(i);
        if( fileInfo.isDir() ){
            m_raderList.append(fileInfo.absoluteFilePath());
            qDebug()<<"RootDirs:"<<fileInfo.baseName();
        }
    }
    return true;
}

bool XML::parseConfigurationFiles()
{
    int nRaderSz = m_raderList.count();
    for( int i=0; i<nRaderSz; ++i )
    {
        QString strEquipListFile("");//设备汇总文件名称
        QString strCfgDirPath = m_raderList.at(i);
        if( false == getEquipmentListFile(strCfgDirPath,strEquipListFile) ){
            qDebug()<<"get Equipment List File failed !";
            continue;
        }

        QSet<QString> readyLoadFiles;//预加载的配置文件名称集合
        if( 0 != readEquipmentList(strEquipListFile,readyLoadFiles) ){
            qDebug()<<"read Equipment List failed !";
            continue;
        }

        if( 0 != readAllEquipmentInfo(strCfgDirPath,readyLoadFiles) )
            return false;
    }
    return true;
}

bool XML::getEquipmentListFile(QString strPath,QString &strEquipListFile)
{
    QDir dir(strPath);
    if( false == dir.exists() )
    {
        qDebug()<<"The path of Radar configuration list is not exit !";
        return false;
    }
    //读取分机设备列表
    dir.setFilter(QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    QFileInfoList  fileInfoList = dir.entryInfoList();
    int nEntrySz = fileInfoList.count();
    if( 1 == nEntrySz ){
        strEquipListFile = fileInfoList.at(0).absoluteFilePath();
    }
    else{
        qDebug()<<"There exist invalade file !";
        return false;
    }
    return true;
}

int XML::readEquipmentList(QString strPath,QSet<QString> &readyLoadFiles)
{
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open XML Failed,FilePath:"<<strPath;
        return -2;
    }

    QDomDocument doc;
    QString error = "";
    int row = 0, column = 0;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return -1;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if( "RaderEquipment"==root.tagName() && "true"==root.attribute("enable") )
    {
        qDebug()<<"Device:"<<root.attribute("name");
        emit createRaderItem(root.attribute("name"));
        QDomNode node = root.firstChild();
        while(!node.isNull()) {
           QDomElement element = node.toElement(); // try to convert the node to an element.
           if(!element.isNull()) {
               if( "SubEquipment" == element.tagName() && "true" == element.attribute("enable")){
                  QString strFileName = element.attribute("name");
                  readyLoadFiles.insert(strFileName);
              }
           }
           node = node.nextSibling();
        }
    }
    return 0;
}

int XML::readAllEquipmentInfo(QString strPath,QSet<QString> readyLoadFiles)
{
    QDir dir(strPath);
    if( false == dir.exists() )
    {
        qDebug()<<"The path of Radar configuration list is not exit !";
        return -2;
    }
    //读取分机设备列表
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    QFileInfoList  fileInfoList = dir.entryInfoList();
    int nEntrySz = fileInfoList.count();
    if( 1 == nEntrySz ){
        QFileInfo fileInfo = fileInfoList.at(0);
        if( fileInfo.isDir() )
        {
            QString strSubDir(fileInfo.absoluteFilePath());
            QDir subDir(strSubDir);
            if( false == subDir.exists() )
            {
                qDebug()<<"The path of Radar configuration subdir is not exit !";
                return -1;
            }
            //读取分机设备列表
            subDir.setFilter(QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
            QFileInfoList  subFileInfoList = subDir.entryInfoList();
            int nSubSz = subFileInfoList.size();
            for( int j=0;j<nSubSz; ++j )
            {
                QFileInfo subFileInfo = subFileInfoList.at(j);
                if(readyLoadFiles.find(subFileInfo.baseName()) != readyLoadFiles.end()){
                    readEquipmentInfo(subFileInfo.absoluteFilePath());
                    qDebug()<<"BaseName:"<<subFileInfo.baseName();
                }
            }
        }
    }
    return 0;
}

int XML::readEquipmentInfo(QString strPath)
{
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open XML Failed,FilePath:"<<strPath;
        return -2;
    }

    QDomDocument doc;
    QString error = "";
    int row = 0, column = 0;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return -1;
    }
    file.close();

    //Root
    QDomElement root = doc.documentElement();
    if( !root.isNull() )
    {
        qDebug()<<root.attribute("name");
        emit addRaderSubItem(root.attribute("name"));
        //Second Level
        QDomNode secNode = root.firstChild();
        while(!secNode.isNull()) {
           QDomElement eleSec = secNode.toElement(); // try to convert the node to an element.
           if(!eleSec.isNull()) {
               if( "Class" == eleSec.tagName() && "true" == eleSec.attribute("enable")){
                  QString strClassName = eleSec.attribute("name");
                  qDebug()<<"\t"<<strClassName;
                  emit addClassItem(strClassName);

                  //Third Level
                  QDomNode thirNode = eleSec.firstChild();
                  while (!thirNode.isNull()) {
                     QDomElement eleThir = thirNode.toElement(); // try to convert the node to an element.
                     if(!eleThir.isNull()) {
                         if( "Item" == eleThir.tagName() && "true" == eleThir.attribute("enable")){
                            QString strItemName = eleThir.attribute("name");
                            qDebug()<<"\t\t"<<strItemName;
                            emit addItem(strItemName);
                            //Forth Level
                            QDomNode forthNode = eleThir.firstChild();
                            while (!forthNode.isNull()) {
                               QDomElement eleForth = forthNode.toElement(); // try to convert the node to an element.
                               if(!eleForth.isNull()) {
                                   if( "Attribute" == eleForth.tagName() && "true" == eleForth.attribute("enable")){
                                      QString strAttrName = eleForth.attribute("name");
                                      QString strDataType = eleForth.attribute("dataType");
                                      QString strDataValue = eleForth.attribute("value");
                                      qDebug()<<"\t\t\t"<<strAttrName<<strDataType<<strDataValue;
                                      emit addData(strAttrName,strDataType,strDataValue);
                                   }
                               }
                               forthNode = forthNode.nextSibling();
                            }
                         }
                     }
                     thirNode = thirNode.nextSibling();
                  }
              }
           }
           secNode = secNode.nextSibling();
        }
    }
    return 0;
}
