#include "SubConfigDispalyTree.h"
#include <QHeaderView>

SubConfigDispalyTree::SubConfigDispalyTree()
{
    initSubConfigDispalyTree();
}

void SubConfigDispalyTree::initSubConfigDispalyTree()
{
    this->setColumnCount(3);
    //this->setColumnHidden(2,true);
    QStringList headers;
    headers<<tr("Paramater Name")<<tr("Paramater Value")<<tr("Paramater ID");
    this->setHeaderLabels(headers);
    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QTreeWidgetItem* pp=new QTreeWidgetItem(this,QStringList("Power On"));
    pp->setExpanded(true);
    QTreeWidgetItem* ppp=new QTreeWidgetItem(pp);
    ppp->setText(0,"Paramater_1");
    ppp->setText(1,"111");

    ppp=new QTreeWidgetItem(pp);
    ppp->setText(0,"Paramater_2");
    ppp->setText(1,"222");
}
