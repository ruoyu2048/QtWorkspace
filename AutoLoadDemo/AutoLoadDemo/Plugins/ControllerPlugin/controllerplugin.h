#ifndef CONTROLLERPLUGIN_H 
#define CONTROLLERPLUGIN_H 


#define BrushInterface_iid "org.XEQD.PlugAndPaint.BrushInterface"


#include "controllerplugin_global.h"

#include "PluginInterface.h"
#include <QObject>
#include <QWidget>


class CONTROLLERPLUGINSHARED_EXPORT ControllerPlugin:public QObject,PluginInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID BrushInterface_iid FILE "controllerplugin.json")
   Q_INTERFACES(PluginInterface)


public:
     QString name() const;
     QString iconPath() const;
     QWidget *widget() const;
     DUCK_WIDGETTYPE duck_Widget() const;
     Qt::DockWidgetArea duck_DockWidgetArea() const;
};
#endif // TESTPLUGIN_H
