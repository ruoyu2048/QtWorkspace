#include "TestDisplay.h"
#include <QFile>
#include <QDebug>

TestDisplay::TestDisplay()
{
    initMainWindow();
}

void TestDisplay::displayItems(QString strFile)
{
    qDebug()<<strFile;
    QFile file(strFile);
    if(file.exists()){
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString strLine = QString::fromLocal8Bit(line);
                QStringList strSplitValues = strLine.split(" ");
                QStringList strRowValuse;
                qDebug()<<strLine;
                foreach(QString strValue,strSplitValues){
                    strValue=strValue.trimmed();
                    if( !strValue.isEmpty() ){
                        strValue.remove("\n");
                        strValue.remove("\t");
                        strValue=strValue.trimmed();
                        if( !strValue.isEmpty() ){
                            qDebug()<<strValue;
                        }
                    }
                }
            }
            file.close();
        }
    }
}

void TestDisplay::initMainWindow()
{
    QStringList headers;
    headers<< QString::fromLocal8Bit("测试项")
            <<QString::fromLocal8Bit("测试阻值")
            <<QString::fromLocal8Bit("测试点")
            <<QString::fromLocal8Bit("测试结论");
    this->setColumnCount(headers.size());
    this->setHorizontalHeaderLabels(headers);
}


