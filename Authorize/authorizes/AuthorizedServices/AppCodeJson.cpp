#include "AppCodeJson.h"
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

#include "DeviceInfo.h"

AppCodeJson::AppCodeJson(QObject *parent) : QObject(parent)
{
    if( m_crypto.generateRsaKeyPair() ){
        m_str_pub_Key="MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCAWzPZcXYilgx7yv2YJZz6nSq79sINBZeIuEkNcAuuEAEfNKOG+rWftVW9u7cSbDFIBBFWUge522+ygvRaFka+QNZui21W/BFb+Tv6QmOB0PYgt3w1t2IqbWgWD7/vfIxBEqWBs0FyTUp2AWzmBvvD+Z1fSmUTh3pAEgtn2DCwHQIDAQAB";
        m_str_pri_key="MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAIBbM9lxdiKWDHvK/ZglnPqdKrv2wg0Fl4i4SQ1wC64QAR80o4b6tZ+1Vb27txJsMUgEEVZSB7nbb7KC9FoWRr5A1m6LbVb8EVv5O/pCY4HQ9iC3fDW3YiptaBYPv+98jEESpYGzQXJNSnYBbOYG+8P5nV9KZROHekASC2fYMLAdAgMBAAECgYArJ/8H5jk1KcQqvzaCuGrObJeYuT/HhxDLHEMbrIeQz7rA1j8BPDxi5WJdt8a2//+V2iBGKLCGUfx9RcCIpaLFBwdaQu9YI6Bzkyri6drou/4hP5vjo+Ii+pjy5Yv+MHRA94uTkxB56aH4RRl+GVNNCiVXlgD7QePjQezYwOOgYQJBAPkUcPMB/Kxc5MrfAEYLA+Qwry9C4v3v11IY3Gh7qPJljPh75GfRBBbawwsR2WsHWVIpRpSWmWj39yz2m7GXXOkCQQCD7CAS7ECW9lA4GvCFKO7dCs+g3STcEgmIonMw5LW9peoXw1FOaatPT8M8+M5f09vsWs65NzKT5+qPURwPxukVAkEA27xEyJ0AI67vyGli/GfLTwwEAFWpmwxzkGGOUD6HlRAxhSUHIu8qBF+JocJlZB5fljQDqCITxJ682FXWlJ1LwQJAJrkXzRYl+2n/sAhQU3Xpb16uBXUETcjJnZ65mz93DVz3MtR1OEQSMeyizXHm+f/xDRa2mW1XXkzz7++KAiOL5QJBALDVERgCFWzk3QnZgerAeBGm7pBRWBUsJhlDOTB0OBQjSYcSpUBszgYroZM2EAsx3Ks33zKeJd5E5LJAGGRXgCk=";

        string str_pub_key = m_crypto.formatRSAKey(m_str_pub_Key,Public);
        string str_pri_key = m_crypto.formatRSAKey(m_str_pri_key,Private);
        m_crypto.setRsaPublicKey( (unsigned char*)str_pub_key.c_str(),str_pub_key.length());
        m_crypto.setRsaPrivateKey((unsigned char*)str_pri_key.c_str(),str_pri_key.length());
    }
}

bool AppCodeJson::rsaEncode()
{
    QJsonObject obj;
    DeviceInfo devInfo;
    obj.insert("systemName", devInfo.sysName());
    obj.insert("mac", devInfo.mac().join("+"));
    obj.insert("motherboardSN", devInfo.baseboardNum());
    obj.insert("hardDiskSN", devInfo.harddiskNum());
    obj.insert("timeNow", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    qDebug()<<QJsonDocument(obj).toJson();
    QByteArray jsVal=QJsonDocument(obj).toJson();

    string std_plaintext(jsVal.constData(),static_cast<size_t>(jsVal.length()));
    string std_ciphertext_base64;
    if( m_crypto.rsaEncrypt(std_plaintext,std_ciphertext_base64) ){
        QByteArray byte_ciphertext_base64(std_ciphertext_base64.c_str(),
                                          static_cast<int>(std_ciphertext_base64.length()));
        QFile file("applicationCode.txt");
        file.open(QIODevice::WriteOnly);
        file.write(byte_ciphertext_base64);
        file.close();
        return true;
    }

    return false;
}

bool AppCodeJson::rsaDecode()
{
    QFile file("applicationCode.txt");
    if( !file.exists() )
        return false;

    if( file.open(QIODevice::ReadOnly) ){
        QByteArray byte_ciphertext_base64 = file.readAll();
        file.close();

        string std_ciphertext_base64(byte_ciphertext_base64.constData(),
                                     static_cast<size_t>(byte_ciphertext_base64.length()));
        string std_plaintext;
        m_crypto.rsaDecrypt(std_ciphertext_base64,std_plaintext);
        QByteArray byte_plaintext(std_plaintext.c_str(),
                                  static_cast<int>(std_plaintext.length()));
        qDebug()<<"Decoded:\n"<<byte_plaintext;
        if( parseAppCode(byte_plaintext) ){

        }
    }

    return true;
}

bool AppCodeJson::parseAppCode(QByteArray &appcode)
{

    return true;
}
