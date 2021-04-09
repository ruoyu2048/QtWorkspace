#include "AuthCodeJson.h"
#include <QDebug>
#include <QDateTime>

#include <QJsonObject>
#include <QJsonDocument>

#include <qrsaencryption.h>

AuthCodeJson::AuthCodeJson(QObject *parent) : QObject(parent)
{
    if( QRSAEncryption::generatePairKey(pubKey,privKey,QRSAEncryption::RSA_1024) ){
        qDebug()<<"pubKey:"<<pubKey.toBase64();
        qDebug()<<"privKey:"<<privKey.toBase64()<<"\n";
    }
}

bool AuthCodeJson::rsaEncode()
{
    QString strPrivKey;
    QString strDevCode;
    QFile file("applicationCode.txt");
    if( file.open( QIODevice::ReadOnly|QIODevice::Text ) ){
        while (!file.atEnd()) {
            QByteArray lineAry=file.readLine();
            qDebug()<<lineAry<<"\n";
            QString strLine( lineAry );
            QString strLabelPrivKey = "privKey:";
            if( strLine.startsWith(strLabelPrivKey) ){
                int nPos = strLine.indexOf(strLabelPrivKey)+strLabelPrivKey.length();
                strPrivKey=strLine.mid(nPos).trimmed();
            }

            QString strLabelAppCode = "appCode:";
            if( strLine.startsWith(strLabelAppCode) ){
                int nPos = strLine.lastIndexOf(strLabelAppCode)+strLabelAppCode.length();
                strDevCode=strLine.mid(nPos).trimmed();
            }
        }

        file.close();

        auto decryptMsg = QRSAEncryption::decode(QByteArray::fromBase64(strDevCode.toLocal8Bit())
                                                 ,QByteArray::fromBase64(strPrivKey.toLocal8Bit()),
                                                 QRSAEncryption::Rsa::RSA_1024);

        qDebug()<<decryptMsg;


        QJsonObject appCode = QJsonDocument::fromJson(decryptMsg).object();
        if( !appCode.isEmpty() ){
            appCode.insert("expirationDate",
                           QDateTime::currentDateTime().addYears(1).toString("yyyy-MM-dd hh:mm:ss"));
            QByteArray clearTxt=QJsonDocument(appCode).toJson();
            if( generateAuthCode(clearTxt) ){

            }
        }

        return true;
    }

    return false;
}

bool AuthCodeJson::rsaDecode()
{
    QString strPrivKey;
    QString strDevCode;
    QFile file("applicationCode.txt");
    if( file.open( QIODevice::ReadOnly|QIODevice::Text ) ){
        while (!file.atEnd()) {
            QByteArray lineAry=file.readLine();
            //qDebug()<<lineAry<<"\n";
            QString strLine( lineAry );
            QString strLabelPrivKey = "privKey:";
            if( strLine.startsWith(strLabelPrivKey) ){
                int nPos = strLine.indexOf(strLabelPrivKey)+strLabelPrivKey.length();
                strPrivKey=strLine.mid(nPos).trimmed();
            }

            QString strLabelDevCode = "DevCode:";
            if( strLine.startsWith(strLabelDevCode) ){
                int nPos = strLine.lastIndexOf(strLabelDevCode)+strLabelDevCode.length();
                strDevCode=strLine.mid(nPos).trimmed();
            }
        }

        file.close();

        auto decryptMsg = QRSAEncryption::decode(QByteArray::fromBase64(strDevCode.toLocal8Bit())
                                                 ,QByteArray::fromBase64(strPrivKey.toLocal8Bit()),
                                                 QRSAEncryption::Rsa::RSA_1024);

        qDebug()<<decryptMsg;

        return true;
    }

    return false;
}

bool AuthCodeJson::generateAuthCode(QByteArray &clearTxt){
    qDebug()<<clearTxt;
    QFile file("authorizationCode.txt");
    if( file.open(QIODevice::WriteOnly|QIODevice::Text) ){
        auto encryptMsg = QRSAEncryption::encode(clearTxt,pubKey,QRSAEncryption::Rsa::RSA_1024);

        if( encryptMsg == clearTxt )
            return false;

        file.write("pubKey:");
        file.write(pubKey.toBase64());
        file.write("\n");

        file.write("privKey:");
        file.write(privKey.toBase64());
        file.write("\n");

        file.write("authCode:");
        file.write(encryptMsg.toBase64());
        file.close();

        auto decryptMsg = QRSAEncryption::decode(encryptMsg,privKey,QRSAEncryption::Rsa::RSA_1024);

        qDebug()<<decryptMsg;

        return true;
    }

    return false;
}
