#include "helloworld.h" 
#include "helloworldwidget.h"



QString HelloWorld::name() const
{
    return tr("HelloWorld");
}

QString HelloWorld::iconPath() const
{
    return "://HelloWorld.png";
}

QWidget *HelloWorld::widget() const
{
    return new HelloWorldWidget();
}

DUCK_WIDGETTYPE HelloWorld::duck_Widget() const
{
    return DUCK_WIDGETTYPE::DOCKWIDGET;
}

Qt::DockWidgetArea HelloWorld::duck_DockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}
