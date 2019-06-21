#ifndef SUBCONFIGDISPALYTREE_H
#define SUBCONFIGDISPALYTREE_H

#include <QTreeWidget>

class SubConfigDispalyTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SubConfigDispalyTree();

private:
    void initSubConfigDispalyTree();

signals:

public slots:
};

#endif // SUBCONFIGDISPALYTREE_H
