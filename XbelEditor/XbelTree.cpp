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
        m_strName = rootElement.attribute("en");
        m_strId = rootElement.attribute("id");

        QTreeWidgetItem* pItemRoot = createItem(rootElement);
        setItemExpanded(pItemRoot,true);
        pItemRoot->setFlags(pItemRoot->flags() | Qt::ItemIsEditable);
        pItemRoot->setText(0,"zh");
        pItemRoot->setText(1,rootElement.attribute("zh"));
        pItemRoot->setText(2,"en");
        pItemRoot->setText(3,rootElement.attribute("en"));
        pItemRoot->setText(4,"id");
        pItemRoot->setText(5,m_strId);

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
    pClassItem->setFlags(pClassItem->flags() | Qt::ItemIsEditable);
    pClassItem->setText(0,"zh");
    pClassItem->setText(1,element.attribute("zh"));
    pClassItem->setText(2,"en");
    pClassItem->setText(3,element.attribute("en"));
    pClassItem->setText(4,"type");
    pClassItem->setText(5,element.attribute("type"));

    QDomElement domUnit = element.firstChildElement();
    while (!domUnit.isNull()) {
        QTreeWidgetItem* pUnitItem = createItem(domUnit, pClassItem);
        setItemExpanded(pUnitItem,true);
        pUnitItem->setFlags(pUnitItem->flags() | Qt::ItemIsEditable);
        pUnitItem->setText(0,"zh");
        pUnitItem->setText(1,domUnit.attribute("zh"));
        pUnitItem->setText(2,"en");
        pUnitItem->setText(3,domUnit.attribute("en"));
        pUnitItem->setText(4,"cmd");
        pUnitItem->setText(5,domUnit.attribute("cmd"));

        if(domUnit.hasChildNodes()){
            QDomElement domAttr=domUnit.firstChildElement();
            while (!domAttr.isNull()) {
                QTreeWidgetItem* pAttrItem=createItem(domAttr,pUnitItem);
                setItemExpanded(pAttrItem,true);
                pAttrItem->setFlags(pAttrItem->flags() | Qt::ItemIsEditable);
                pAttrItem->setText(0,domAttr.tagName());

                QDomElement domNameZH = domAttr.firstChildElement("zh");
                QTreeWidgetItem* pItemNameZH = createItem(domNameZH,pAttrItem);
                pItemNameZH->setFlags(pItemNameZH->flags() | Qt::ItemIsEditable);
                pItemNameZH->setText(0,domNameZH.tagName());
                pItemNameZH->setText(1,domNameZH.text());

                QDomElement domNameEN = domAttr.firstChildElement("en");
                QTreeWidgetItem* pItemNameEN = createItem(domNameEN,pAttrItem);
                pItemNameEN->setFlags(pItemNameEN->flags() | Qt::ItemIsEditable);
                pItemNameEN->setText(0,domNameEN.tagName());
                pItemNameEN->setText(1,domNameEN.text());

                QDomElement domId = domAttr.firstChildElement("id");
                QTreeWidgetItem* pItemId = createItem(domId,pAttrItem);
                pItemId->setFlags(pItemId->flags() | Qt::ItemIsEditable);
                pItemId->setText(0,domId.tagName());
                pItemId->setText(1,domId.text());

                QDomElement domShow = domAttr.firstChildElement("show");
                QTreeWidgetItem* pItemShow = createItem(domShow,pAttrItem);
                pItemShow->setFlags(pItemShow->flags() | Qt::ItemIsEditable);
                pItemShow->setText(0,domShow.tagName());
                pItemShow->setText(1,domShow.text());

                QDomElement domSet = domAttr.firstChildElement("set");
                QTreeWidgetItem* pItemSet = createItem(domSet,pAttrItem);
                pItemSet->setFlags(pItemSet->flags() | Qt::ItemIsEditable);
                pItemSet->setText(0,domSet.tagName());
                pItemSet->setText(1,domSet.text());

                QDomElement domDataType = domAttr.firstChildElement("dataType");
                QTreeWidgetItem* pItemDataType = createItem(domDataType,pAttrItem);
                pItemDataType->setFlags(pItemDataType->flags() | Qt::ItemIsEditable);
                pItemDataType->setText(0,domDataType.tagName());
                pItemDataType->setText(1,domDataType.text());

                QDomElement domDataLen = domAttr.firstChildElement("dataLen");
                QTreeWidgetItem* pItemDataLen = createItem(domDataLen,pAttrItem);
                pItemDataLen->setFlags(pItemDataLen->flags() | Qt::ItemIsEditable);
                pItemDataLen->setText(0,domDataLen.tagName());
                pItemDataLen->setText(1,domDataLen.text());

                QDomElement domDisplayType = domAttr.firstChildElement("displayType");
                QTreeWidgetItem* pItemDisplayType = createItem(domDisplayType,pAttrItem);
                pItemDisplayType->setFlags(pItemDisplayType->flags() | Qt::ItemIsEditable);
                pItemDisplayType->setText(0,domDisplayType.tagName());
                pItemDisplayType->setText(1,domDisplayType.text());

                QDomElement domParamType = domAttr.firstChildElement("select");
                QTreeWidgetItem* pItemParamType = createItem(domParamType,pAttrItem);
                pItemParamType->setFlags(pItemParamType->flags() | Qt::ItemIsEditable);
                pItemParamType->setText(0,domParamType.tagName());
                pItemParamType->setText(1,domParamType.text());

                QDomElement domDefault = domAttr.firstChildElement("default");
                QTreeWidgetItem* pItemDefault = createItem(domDefault,pAttrItem);
                pItemDefault->setFlags(pItemDefault->flags() | Qt::ItemIsEditable);
                pItemDefault->setText(0,domDefault.tagName());
                pItemDefault->setText(1,domDefault.text());

                QDomElement domRegExp = domAttr.firstChildElement("regExp");
                QTreeWidgetItem* pItemRegExp = createItem(domRegExp,pAttrItem);
                pItemRegExp->setFlags(pItemRegExp->flags() | Qt::ItemIsEditable);
                pItemRegExp->setText(0,domRegExp.tagName());
                pItemRegExp->setText(1,domRegExp.text());

                QDomElement domRegTips = domAttr.firstChildElement("regTips");
                QTreeWidgetItem* pItemRegTips = createItem(domRegTips,pAttrItem);
                pItemRegTips->setFlags(pItemRegTips->flags() | Qt::ItemIsEditable);
                pItemRegTips->setText(0,domRegTips.tagName());
                pItemRegTips->setText(1,domRegTips.text());

                domAttr = domAttr.nextSiblingElement();
            }
        }
        domUnit = domUnit.nextSiblingElement();
    }
}

//protect slots;
void XbelTree::updateDomElement(QTreeWidgetItem* pItem, int column)
{
    QDomElement element = getDomElementForItem(pItem);
    if (!element.isNull()) {
        //屏蔽所有标签列修改
        if (element.tagName() == "root"){
            element.setAttribute("en", pItem->text(1));
            element.setAttribute("zh", pItem->text(3));
            element.setAttribute("id", pItem->text(5));
        }
        if (element.tagName() == "class"){
            element.setAttribute("en", pItem->text(1));
            element.setAttribute("zh", pItem->text(3));
            element.setAttribute("type", pItem->text(5));
        }
        else if (element.tagName() == "unit"){
            element.setAttribute("en", pItem->text(1));
            element.setAttribute("zh", pItem->text(3));
            element.setAttribute("cmd", pItem->text(5));
        }
        else if (element.tagName() == "id"
                ||element.tagName() == "en"
                ||element.tagName() == "zh"
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
