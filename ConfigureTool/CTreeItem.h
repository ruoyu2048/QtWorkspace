#ifndef CTREEITEM_H
#define CTREEITEM_H

#include <QList>

struct Attr{
    QString name;
    QString displayName;
    QString displayType;
    QString dataType;
    QString value;
    QString paramType;
    QString validator;
    QString tips;
};

struct Entity{
        QString name;
        QString displayName;
        QList<Attr> attrs;
        };

struct SubjectInfo{
        QString name;
        QString displayName;
        QString type;
        QList<Entity>ents;
        };

struct Subject{
        QString name;           //分机标识
        QString displayName;    //分机名称
        QList<SubjectInfo>subjectInfos;
        };

struct Rader_Total{
        QString         name;       //总机标识
        QString         displayName;//总机名称
        QString         cfgFile;    //分机配置
        QList<Subject>  subjects;   //分机列表
        };


#endif // CTREEITEM_H
