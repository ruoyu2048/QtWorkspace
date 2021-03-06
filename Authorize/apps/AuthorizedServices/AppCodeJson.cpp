﻿#include "AppCodeJson.h"
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

#include "DeviceInfo.h"

#include <qrsaencryption.h>

AppCodeJson::AppCodeJson(QObject *parent) : QObject(parent)
{

}

bool AppCodeJson::rsaEncode()
{
    QFile file("applicationCode.txt");
    file.open(QIODevice::WriteOnly);
    QJsonObject obj;
    DeviceInfo devInfo;
    obj.insert("systemName", devInfo.sysName());
    obj.insert("mac", devInfo.mac().join("+"));
    obj.insert("motherboardSN", devInfo.baseboardNum());
    obj.insert("hardDiskSN", devInfo.harddiskNum());
    obj.insert("timeNow", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    qDebug()<<QJsonDocument(obj).toJson();
    QByteArray jsVal=QJsonDocument(obj).toJson();

    QByteArray pub,priv;
    QRSAEncryption rsa(QRSAEncryption::Rsa::RSA_1024);
    rsa.generatePairKey(pub,priv);
//    qDebug()<<"\nPubKey:"<<pub.toBase64()<<" \n\nPrivKey:"<<priv.toBase64();
//    file.write("PubKey:\n");
//    file.write(pub.toBase64());
//    file.write("\nPrivKey:\n");
//    file.write(priv.toBase64());

    pub = QByteArray::fromBase64("AENxdz+xk1QB+kh3mCiRO5dShA8OhSOD2HnWljObPUsQPgPfWMBDQetXhKeUMURyvAiBR9MpkYwwCTPW6/tHM3DeriAfGq0Frg5rgQh1e9B3EgQLnC7MqdQitVdBKmd1lomZ+RV59/E+HslbLRosG1wlY7j3w098SX4b0QAGTqsAoIDr0ph85XXSBtiOK8Adv3Yr6CY9lQClpmY+T+mYGBlPyIrp2PXjY5Toz4dZouzhsOI92raf4kaF39uFwESf1knZ5jVWGfyhDAsWMiNls5lo5daGnBJVMo1l5bvJTJuofru0Hsll41rY/AyoxL7pAa5FFU9ZdYQ0yBQYutTMBw==");
    priv=QByteArray::fromBase64("ADLe8tRCOROLY54Vq5MqEjVDYpmFH7aNAUCbiHu2yN7B0cSzJdpHVqw2KoSb2hHVRPzJw+ZNmlgqAyDazvgTah5IL/zWlhZ9YB4vJNU1Wm8IFRtOELzKcYRAmYJFGPspxss39eHHasLQYq/3qKx43M6ukf6QH+b8IUQsIWp316MAoIDr0ph85XXSBtiOK8Adv3Yr6CY9lQClpmY+T+mYGBlPyIrp2PXjY5Toz4dZouzhsOI92raf4kaF39uFwESf1knZ5jVWGfyhDAsWMiNls5lo5daGnBJVMo1l5bvJTJuofru0Hsll41rY/AyoxL7pAa5FFU9ZdYQ0yBQYutTMBw==");
    qDebug()<<"\nPubKey:"<<pub.toBase64()<<" \n\nPrivKey:"<<priv.toBase64();
    auto encryptMsg  = rsa.encode(jsVal,pub);

    if( encryptMsg == jsVal )
        return false;


    file.write("\nencryptMsg:\n");
    file.write(encryptMsg.toBase64());
    file.close();

    auto decryptMsg=rsa.decode(encryptMsg,priv);

    qDebug()<<decryptMsg;

    return true;
}
