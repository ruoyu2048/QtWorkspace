#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include "PluginManager.h"


PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{

}

QMap<QString, PluginInterface*> PluginManager::FindPlugin()
{
    QDir pluginsDir;
    QString pluginsPath=pluginsDir.currentPath()+"/PluginDir";
            qDebug()<<pluginsPath;
    pluginsDir.setCurrent(pluginsPath);
    foreach( QString pluginName, pluginsDir.entryList(QDir::Files) )
    {
        qDebug()<<pluginsDir.absolutePath();
        qDebug()<<pluginName;
        QPluginLoader loader( pluginsDir.absoluteFilePath( pluginName ) );
        QObject *couldBePlugin = loader.instance();
        if( couldBePlugin )
        {
            qDebug() << tr("load plugin Dir:%1").arg(pluginName);
            PluginInterface *plugin = qobject_cast<PluginInterface*>( couldBePlugin );
            if( plugin )
            {
                _MapPlugin[ plugin->name() ] = plugin;
            }
        }
    }
    return _MapPlugin;
}
