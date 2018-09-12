#include "XML.h"
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QDebug>

XML::XML(QObject *parent) : QObject(parent)
{

}

bool XML::getConfigureInfo(QString strConfigPath)
{
    if( true == getAllRaderConfigDirPath(strConfigPath) )
        return parseConfigurationFiles();

    return false;
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
    for( int i=0; i<nEntryCnt; ++i ){
        QFileInfo fileInfo = fileInfoList.at(i);
        if( fileInfo.isDir() ){
            //config/Rander_01、config/Rander_02、...
            mPathDirs.append(fileInfo.absoluteFilePath());
        }
    }
    return true;
}

bool XML::parseConfigurationFiles()
{
    //Rander_01 Rander_02 ...
    int nRaderSz = mPathDirs.count();
    for( int i=0; i<nRaderSz; ++i )
    {

        QString strEquipListFile("");//设备汇总文件名称
        QString strCfgDirPath = mPathDirs.at(i);
        if( false == getEquipmentListFile(strCfgDirPath,strEquipListFile) ){
            qDebug()<<"get Equipment List File failed !";
            continue;
        }

        Rader_Total raderTotal;
        //Subobject_01  Subobject_02 ...
        QSet<QString> readyLoadFiles;//预加载的配置文件名称集合
        if( 0 != readEquipmentList(strEquipListFile,raderTotal,readyLoadFiles) ){
            qDebug()<<"read Equipment List failed !";
            continue;
        }

        if( 0 != readAllEquipmentInfo(strCfgDirPath,raderTotal,readyLoadFiles) )
            return false;
        mRaderTotals.push_back(raderTotal);
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

int XML::readEquipmentList(QString strPath,Rader_Total &raderTotal,QSet<QString> &readyLoadFiles)
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
    if( "Radar"==root.tagName() && "true"==root.attribute("enable") )
    {
        raderTotal.name = root.attribute("name");
        raderTotal.displayName = root.attribute("displayName");

        QDomNode node = root.firstChild();
        while(!node.isNull()) {
           QDomElement element = node.toElement(); // try to convert the node to an element.
           if(!element.isNull()) {
               if( "Subject" == element.tagName() && "true" == element.attribute("enable")){
                  //raderTotal.name = element.attribute("name");
                  //raderTotal.displayName = element.attribute("displayName");
                  raderTotal.cfgFile = element.attribute("cfg");
                  readyLoadFiles.insert(raderTotal.cfgFile);
              }
           }
           node = node.nextSibling();
        }
    }
    return 0;
}

int XML::readAllEquipmentInfo(QString strPath,Rader_Total &raderTotal,QSet<QString> readyLoadFiles)
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
                    Subject subject;
                    if( 0==readEquipmentInfo(subFileInfo.absoluteFilePath(),subject) )
                        raderTotal.subjects.push_back(subject);
                }
            }
        }
    }
    return 0;
}

int XML::readEquipmentInfo(QString strPath,Subject &subject)
{
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly)){
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

    //Subject
    QDomElement rootElem = doc.documentElement();
    if( !rootElem.isNull() ){
        if( "Subject" == rootElem.tagName() ){
            subject.name = rootElem.attribute("name");
            subject.displayName = rootElem.attribute("displayName");
            //qDebug()<<subject.name <<subject.displayName;

            //subjectInfo
            QDomNode secNode = rootElem.firstChildElement();
            while( !secNode.isNull() ){
                QDomElement subInfoElement = secNode.toElement();
                if(!subInfoElement.isNull() && "subjectInfo" == subInfoElement.tagName()) {
                    SubjectInfo subjectInfo;
                    subjectInfo.name = subInfoElement.attribute("name");
                    subjectInfo.displayName = subInfoElement.attribute("displayName");
                    subjectInfo.type = subInfoElement.attribute("type");
                    //qDebug()<<subInfoElement.attribute("name")<<subInfoElement.attribute("displayName")<<subjectInfo.type;
                    //subject.subjectInfos.push_back(subjectInfo);

                    //entity
                    QDomNode entNode = secNode.firstChildElement();
                    while( !entNode.isNull() ){
                        QDomElement entElem = entNode.toElement();
                        if( !entElem.isNull() && "entity" == entElem.tagName()){
                            Entity entity;
                            entity.name = entElem.attribute("name");
                            entity.displayName = entElem.attribute("displayName");
                            //qDebug()<<entElem.attribute("name")<<entElem.attribute("displayName");
                            //subjectInfo.ents.push_back(entity);

                            //Attr
                            //subjectInfo
                            QDomNode attrNode = entElem.firstChildElement();
                            while( !attrNode.isNull() ){
                                QDomElement attrElem = attrNode.toElement();
                                if(!attrElem.isNull()  && "attribute" == attrElem.tagName()) {
                                    Attr attr;
                                    if( "true" == attrElem.attribute("show"))
                                        attr.show = true;
                                    if( "false" == attrElem.attribute("show"))
                                        attr.show = false;

                                    if( "true" == attrElem.attribute("addBtn"))
                                        attr.addBtn = true;
                                    if( "false" == attrElem.attribute("addBtn"))
                                        attr.addBtn = false;

                                    attr.dataType = attrElem.attribute("dataType");
                                    attr.dataLen = attrElem.attribute("dataLen").toInt();
                                    attr.displayType = attrElem.attribute("displayType");
                                    attr.name = attrElem.attribute("name");
                                    attr.displayName = attrElem.attribute("displayName");
                                    attr.paramType = attrElem.attribute("paramType");
                                    attr.defValue = attrElem.attribute("default");
                                    attr.validator = attrElem.attribute("validator");
                                    attr.tips = attrElem.attribute("tips");
                                    qDebug()<<attr.displayName;
                                    entity.attrs.push_back(attr);
                                }
                                attrNode = attrNode.nextSibling();
                            }
                            subjectInfo.ents.push_back(entity);
                        }
                        entNode = entNode.nextSibling();
                    }
                    subject.subjectInfos.push_back(subjectInfo);
                }
                secNode = secNode.nextSibling();
            }
        }
    }

    return 0;
}
