#include <QCoreApplication>
#include <QDebug>
#include <QFile>

/*
 * @breif 将16进制字符串转换为对应的字节序列
 */
QByteArray HexToByteArray(QString strHex)
{
    QByteArray hexAry;
    strHex = strHex.trimmed();
    strHex = strHex.simplified();
    QStringList strHexList = strHex.split(" ");

    foreach (QString str, strHexList) {
        if( !str.isEmpty() ){
            bool ok = true;
            char cHex = str.toInt(&ok,16)&0xFF;
            if(ok){
                hexAry.append(cHex);
            }else{
                qDebug()<<"非法的16进制字符："<<str;
            }
        }
    }
    return hexAry;
}

QString ByteArrayToHex(QByteArray byteAry){
    QString strHex(byteAry.toHex().toUpper());
    int aryLen = strHex.length()/2;
    for( int i=1; i<aryLen; i++ ){
        strHex.insert(2*i+i-1," ");
    }
    return strHex;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray src;
    src.append(0x01).append(0x02).append(0xf3);
    src.append(0x01).append(0x02).append(0xf3);
    src.append(0x01).append(0x02).append(0xf3);
    src.append(0x01).append(0xf3);
    qDebug()<<"To File Ary:"<<src;
    QString strHex = ByteArrayToHex(src);
    qDebug()<<"To File:"<<strHex;

    QFile file("Test.dat");
    file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    file.write(strHex.toLatin1());
    file.flush();
    file.close();

    file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    file.write(strHex.toLatin1());
    file.flush();
    file.close();

    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray ll = file.readAll();
    QString strHexFile(ll);
    qDebug()<<"Fr File:"<<strHexFile;
    QByteArray bateAry = HexToByteArray(strHexFile);
    qDebug()<<"Fr File Ary:"<<bateAry;
    return a.exec();
}
