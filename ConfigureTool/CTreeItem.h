#ifndef CTREEITEM_H
#define CTREEITEM_H

#include <QObject>
#include <QList>

struct Forth_Level{
    QString source;
    QString name;
    QString displayName;
    QString dataType;
    QString readWriteFlag;
    QString validator;
    QString displayType;
    QString unit;
    QString paramType;
    QString hidden;
    QString macroConfig;
};

//entity
struct Third_Level{
    QString source;
    QString readOnly;
    QString name;
    QString displayName;
    QString type;
    QList<Forth_Level>attrs;//实体的属性列表
};

struct Second_Level{
    //description
    QString description;
    //subjectInfoMD
    QString name;
    QString displayName;
    QString type;
    QList<Third_Level>ents;//实体列表
};

//subjectType
//struct First_Level{
struct RaderInfo{
    QString filePath;
    QString isComposite;
    QString name;
    QString displayName;
    QString category;
    QString version;
    QString icon;
    Second_Level secLevel;
};


//class CTreeItem : public QObject
//{
//    Q_OBJECT
//public:
//    explicit CTreeItem(QObject *parent = nullptr);

//signals:

//public slots:
//};

#endif // CTREEITEM_H
