#ifndef CTREEITEM_H
#define CTREEITEM_H

#include <QList>

struct Attr{
    bool    show;           //是否显示
    bool    addBtn;         //是否需要添加设置按钮
    QString dataType;       //数据类型
    quint32 dataLen;        //数据长度
    QString displayType;    //显示类型，hide、select、text
    QString name;           //名称
    QString displayName;    //显示名称
    QString paramType;      //参数类型
    QString defValue;       //默认值
    QString validator;      //编辑框的正则表达式
    QString tips;           //编辑框的错误提示
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
