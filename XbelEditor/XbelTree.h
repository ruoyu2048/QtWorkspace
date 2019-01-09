#ifndef XBELTREE_H
#define XBELTREE_H

#include "IXbelTree.h"

class XbelTree : public IXbelTree
{
    Q_OBJECT

public:
    XbelTree( QWidget *parent = nullptr );

    ~XbelTree();

protected:
    virtual void setRootElement( const QDomElement& rootElement );

    virtual void parseUnitElement(const QDomElement &element,QTreeWidgetItem *parentItem = nullptr );

protected slots:
    void updateDomElement(QTreeWidgetItem* pItem, int column);
};

#endif
