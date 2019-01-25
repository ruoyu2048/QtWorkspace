#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include "PluginInterface.h"

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = nullptr);
    QMap<QString, PluginInterface*> FindPlugin();

private:
    QMap<QString, PluginInterface*> _MapPlugin;

signals:

public slots:

};

#endif // PLUGINMANAGER_H
