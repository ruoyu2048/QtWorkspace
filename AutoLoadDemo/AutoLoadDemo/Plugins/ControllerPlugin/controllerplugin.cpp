#include "controllerplugin.h"
#include "widget.h"



QString ControllerPlugin::name() const
{
    return tr("控制器");
}

QString ControllerPlugin::iconPath() const
{
    return ":/Controller.ico";
}

QWidget *ControllerPlugin::widget() const
{
    return new Widget();
}

DUCK_WIDGETTYPE ControllerPlugin::duck_Widget() const
{
    return DUCK_WIDGETTYPE::MDIAREA;
}

Qt::DockWidgetArea ControllerPlugin::duck_DockWidgetArea() const
{
    return Qt::LeftDockWidgetArea;
}
