#include "XbelTree.h"

#include <QFile>
#include <QtDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>

XbelTree::XbelTree(QWidget *parent):IXbelTree(parent)
{

}

XbelTree::~XbelTree(){

}

void XbelTree::setRootElement( const QDomElement& rootElement ){
    if( "root" == rootElement.tagName() ){
        //解析根节点属性参数
        m_strName = rootElement.attribute("name");
        m_strId = rootElement.attribute("id");

        QTreeWidgetItem* pItemRoot = createItem(rootElement);
        setItemExpanded(pItemRoot,true);
        pItemRoot->setFlags(pItemRoot->flags() | Qt::ItemIsEditable);
        pItemRoot->setText(0,"name");
        pItemRoot->setText(1,m_strName);
        pItemRoot->setText(2,"id");
        pItemRoot->setText(3,m_strId);

        //命令类型
        QDomElement domClass = rootElement.firstChildElement("class");
        while (!domClass.isNull()) {
            parseUnitElement(domClass,pItemRoot);
            domClass = domClass.nextSiblingElement("class");
        }
    }
}

void XbelTree::parseUnitElement(const QDomElement &element, QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem* pClassItem = createItem(element, parentItem);
    setItemExpanded(pClassItem,true);
    pClassItem->setText(0,"name");
    pClassItem->setText(1,element.attribute("name"));
    pClassItem->setText(2,"type");
    pClassItem->setText(3,element.attribute("type"));

    QDomElement domUnit = element.firstChildElement();
    while (!domUnit.isNull()) {
        QTreeWidgetItem* pUnitItem = createItem(domUnit, pClassItem);
        setItemExpanded(pUnitItem,true);
        pUnitItem->setFlags(pUnitItem->flags() | Qt::ItemIsEditable);
        pUnitItem->setText(0,"name");
        pUnitItem->setText(1,domUnit.attribute("name"));
        pUnitItem->setText(2,"cmd");
        pUnitItem->setText(3,domUnit.attribute("cmd"));

        QDomElement domId = domUnit.firstChildElement("id");
        QTreeWidgetItem* pItemId = createItem(domId,pUnitItem);
        pItemId->setFlags(pItemId->flags() | Qt::ItemIsEditable);
        pItemId->setText(0,domId.tagName());
        pItemId->setText(1,domId.text());

        QDomElement domName = domUnit.firstChildElement("name");
        QTreeWidgetItem* pItemName = createItem(domName,pUnitItem);
        pItemName->setFlags(pItemName->flags() | Qt::ItemIsEditable);
        pItemName->setText(0,domName.tagName());
        pItemName->setText(1,domName.text());

        QDomElement domShow = domUnit.firstChildElement("show");
        QTreeWidgetItem* pItemShow = createItem(domShow,pUnitItem);
        pItemShow->setFlags(pItemShow->flags() | Qt::ItemIsEditable);
        pItemShow->setText(0,domShow.tagName());
        pItemShow->setText(1,domShow.text());

        QDomElement domSet = domUnit.firstChildElement("set");
        QTreeWidgetItem* pItemSet = createItem(domSet,pUnitItem);
        pItemSet->setFlags(pItemSet->flags() | Qt::ItemIsEditable);
        pItemSet->setText(0,domSet.tagName());
        pItemSet->setText(1,domSet.text());

        QDomElement domDataType = domUnit.firstChildElement("dataType");
        QTreeWidgetItem* pItemDataType = createItem(domDataType,pUnitItem);
        pItemDataType->setFlags(pItemDataType->flags() | Qt::ItemIsEditable);
        pItemDataType->setText(0,domDataType.tagName());
        pItemDataType->setText(1,domDataType.text());

        QDomElement domDataLen = domUnit.firstChildElement("dataLen");
        QTreeWidgetItem* pItemDataLen = createItem(domDataLen,pUnitItem);
        pItemDataLen->setFlags(pItemDataLen->flags() | Qt::ItemIsEditable);
        pItemDataLen->setText(0,domDataLen.tagName());
        pItemDataLen->setText(1,domDataLen.text());

        QDomElement domDisplayType = domUnit.firstChildElement("displayType");
        QTreeWidgetItem* pItemDisplayType = createItem(domDisplayType,pUnitItem);
        pItemDisplayType->setFlags(pItemDisplayType->flags() | Qt::ItemIsEditable);
        pItemDisplayType->setText(0,domDisplayType.tagName());
        pItemDisplayType->setText(1,domDisplayType.text());

        QDomElement domParamType = domUnit.firstChildElement("select");
        QTreeWidgetItem* pItemParamType = createItem(domParamType,pUnitItem);
        pItemParamType->setFlags(pItemParamType->flags() | Qt::ItemIsEditable);
        pItemParamType->setText(0,domParamType.tagName());
        pItemParamType->setText(1,domParamType.text());

        QDomElement domDefault = domUnit.firstChildElement("default");
        QTreeWidgetItem* pItemDefault = createItem(domDefault,pUnitItem);
        pItemDefault->setFlags(pItemDefault->flags() | Qt::ItemIsEditable);
        pItemDefault->setText(0,domDefault.tagName());
        pItemDefault->setText(1,domDefault.text());

        QDomElement domRegExp = domUnit.firstChildElement("regExp");
        QTreeWidgetItem* pItemRegExp = createItem(domRegExp,pUnitItem);
        pItemRegExp->setFlags(pItemRegExp->flags() | Qt::ItemIsEditable);
        pItemRegExp->setText(0,domRegExp.tagName());
        pItemRegExp->setText(1,domRegExp.text());

        QDomElement domRegTips = domUnit.firstChildElement("regTips");
        QTreeWidgetItem* pItemRegTips = createItem(domRegTips,pUnitItem);
        pItemRegTips->setFlags(pItemRegTips->flags() | Qt::ItemIsEditable);
        pItemRegTips->setText(0,domRegTips.tagName());
        pItemRegTips->setText(1,domRegTips.text());

        domUnit = domUnit.nextSiblingElement();
    }
}

//protect slots;
void XbelTree::updateDomElement(QTreeWidgetItem* pItem, int column)
{
    QDomElement element = getDomElementForItem(pItem);
    if (!element.isNull()) {
        //屏蔽所有标签列修改
        if (column%2 == 0) {
            QDomElement oldTitleElement = element.firstChildElement("title");
            QDomElement newTitleElement = getDocument().createElement("title");

            QDomText newTitleText = getDocument().createTextNode(pItem->text(0));
            newTitleElement.appendChild(newTitleText);

            element.replaceChild(newTitleElement, oldTitleElement);
        }
        else {
            if (element.tagName() == "class"){
                element.setAttribute("name", pItem->text(1));
                element.setAttribute("type", pItem->text(3));
            }
            else if (element.tagName() == "unit"){
                element.setAttribute("name", pItem->text(1));
                element.setAttribute("cmd", pItem->text(3));
            }
            else if (element.tagName() == "id"
                    ||element.tagName() == "name"
                    ||element.tagName() == "show"
                    ||element.tagName() == "set"
                    ||element.tagName() == "dataType"
                    ||element.tagName() == "dataLen"
                    ||element.tagName() == "displayType"
                    ||element.tagName() == "select"
                    ||element.tagName() == "default"
                    ||element.tagName() == "regExp"
                    ||element.tagName() == "regTips")
            {
                QDomNode oldNode = element.firstChild();            //标签之间的内容作为节点的子节点出现，得到原来的子节点
                if( oldNode.isNull() ){
                    qDebug()<<element.tagName()<<pItem->text(1)<<element.isNull();
                    QDomText domText = getDocument().createTextNode(pItem->text(1));
                    element.appendChild(domText);
                }
                else{
                    element.firstChild().setNodeValue(pItem->text(1));   //用提供的value值来设置子节点的内容
                    QDomNode newNode = element.firstChild();            //值修改过后的子节点
                    element.replaceChild(newNode,oldNode);              //调用节点的replaceChild方法实现修改功能
                }
            }
        }
    }
}
