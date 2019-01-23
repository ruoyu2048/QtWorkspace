#include <QCoreApplication>
#include <QByteArray>
#include <QDebug>
#include <QtEndian>

int isLittleEndian()
{
    union UCHECK
    {
        int n;
        char ch;
    }uc;
    uc.n=1;
    return (uc.ch==1);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if( isLittleEndian() )
        qDebug()<<"Little";

    if( !isLittleEndian() ){

    }


    char *aa="AB";
    char bb[2];
    qToBigEndian(aa,bb);
    qDebug()<<bb;


    return a.exec();
}
