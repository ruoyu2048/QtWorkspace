#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include "pluginmanager.h"


PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{

}

QMap<QString, PluginInterFace*> PluginManager::FindPlugin()
{
    QDir path( "./" );
    foreach( QString pluginName, path.entryList(QDir::Files) )
    {
        qDebug()<<pluginName;
        QPluginLoader loader( path.absoluteFilePath( pluginName ) );
        QObject *couldBePlugin = loader.instance();
        if( couldBePlugin )
        {
            qDebug() << tr("load plugin Dir:%1").arg(pluginName);
            PluginInterFace *plugin = qobject_cast<PluginInterFace*>( couldBePlugin );
            if( plugin )
            {
                _MapPlugin[ plugin->name() ] = plugin;
            }
        }
    }
    return _MapPlugin;
}
