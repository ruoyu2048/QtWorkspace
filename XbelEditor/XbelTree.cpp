#include "XbelTree.h"

#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>

XbelTree::XbelTree(QWidget *parent):IXbelTree(parent)
{

}

XbelTree::~XbelTree(){

}

void XbelTree::setRootElement( const QDomElement& rootElement ){
    QDomElement child = rootElement.firstChildElement("folder");
    while (!child.isNull()) {
        parseUnitElement(child);
        child = child.nextSiblingElement("folder");
    }
}

void XbelTree::parseUnitElement(const QDomElement &element, QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = createItem(element, parentItem);

    QString title = element.firstChildElement("title").text();
    if (title.isEmpty())
        title = QObject::tr("Folder");

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setIcon(0, m_folderIcon);
    item->setText(0, title);

    bool folded = (element.attribute("folded") != "no");
    setItemExpanded(item, !folded);

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "folder") {
            parseUnitElement(child, item);
        }
        else if (child.tagName() == "bookmark") {
            QTreeWidgetItem *childItem = createItem(child, item);

            QString title = child.firstChildElement("title").text();
            if (title.isEmpty())
                title = QObject::tr("Folder");

            childItem->setFlags(item->flags() | Qt::ItemIsEditable);
            childItem->setIcon(0, m_folderIcon);
            childItem->setText(0, title);
            childItem->setText(1, child.attribute("href"));
        }
        else if (child.tagName() == "separator") {
            QTreeWidgetItem *childItem = createItem(child, item);
            childItem->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));
            childItem->setText(0, QString(30, 0xB7));
        }
        child = child.nextSiblingElement();
    }
}

//protect slots;
void XbelTree::updateDomElement(QTreeWidgetItem *item, int column)
{
    QDomElement element = m_domElementForItem.value(item);
    if (!element.isNull()) {
        if (column == 0) {
            QDomElement oldTitleElement = element.firstChildElement("title");
            QDomElement newTitleElement = m_domDocument.createElement("title");

            QDomText newTitleText = m_domDocument.createTextNode(item->text(0));
            newTitleElement.appendChild(newTitleText);

            element.replaceChild(newTitleElement, oldTitleElement);
        }
        else {
            if (element.tagName() == "bookmark")
                element.setAttribute("href", item->text(1));
        }
    }
}
