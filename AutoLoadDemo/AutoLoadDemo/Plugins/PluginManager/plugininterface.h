#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#define BrushInterface_iid "org.XEQD.PlugAndPaint.BrushInterface"

#include <QObject>
#include <QString>
#include <QToolButton>

enum DUCK_WIDGETTYPE{
    MDIAREA,
    DOCKWIDGET
};


class PluginInterFace
{

public:
    virtual QString name() const=0;
    virtual QString iconPath() const=0;
    virtual QWidget *widget() const=0;
    virtual DUCK_WIDGETTYPE duck_Widget() const=0;
    virtual Qt::DockWidgetArea duck_DockWidgetArea() const=0;

};

QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(PluginInterFace, BrushInterface_iid)
    QT_END_NAMESPACE

#endif
