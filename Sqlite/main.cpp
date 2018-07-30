#include <QCoreApplication>
#include "DB.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DB db;
    db.OpenDB("Test");
    //db.DeleteTable("student");
    QString strSql = "create table student (id int primary key, name varchar(30), AA varchar(30), BB varchar(30), age int)";
    db.CreateTable("student",strSql);
    for(int i=0;i<10000;i++)
    {
        QString strInsert = QString("INSERT INTO student(id, name, AA, BB, age) VALUES(%1,'AAA','BBB','CCC',100)").arg(i+1);
        qDebug()<<strInsert;
        db.Insert(strInsert);
    }
    db.CloseDB();

    return a.exec();
}
