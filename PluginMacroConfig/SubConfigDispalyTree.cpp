#include "SubConfigDispalyTree.h"
#include <QHeaderView>
#include <QDomDocument>
#include <QDebug>
#include <QFile>
#include <QLineEdit>
#include <QToolTip>
#include <QComboBox>

SubConfigDispalyTree::SubConfigDispalyTree( QString strSubCfgPath )
{
    this->setObjectName("SubConfigDispalyTree");
    initSubConfigDispalyTree(strSubCfgPath);
}

void SubConfigDispalyTree::exportSubCofig(QXmlStreamWriter *pWriter)
{
    if( nullptr!=pWriter ){
        int nTopCnt = this->topLevelItemCount();
        for( int i=0;i<nTopCnt;i++ ){
            QTreeWidgetItem* pCMDItem = this->topLevelItem(i);
            pWriter->writeStartElement(QStringLiteral("命令"));
            pWriter->writeAttribute(QStringLiteral("cmd"),pCMDItem->text(2).trimmed());

            int nChild = pCMDItem->childCount();
            for( int j=0;j<nChild;j++ ){
                QTreeWidgetItem* pAttrItem=pCMDItem->child(j);
                pWriter->writeStartElement(QStringLiteral("参数"));
                pWriter->writeAttribute(QStringLiteral("paraId"),pAttrItem->text(2).trimmed());
                QString strParaId=pAttrItem->text(2).trimmed();
                auto it=m_IdLinkedMap.find(strParaId);
                if( it!=m_IdLinkedMap.end() ){
                    QString strDisplayType=pAttrItem->text(3).trimmed();
                    if( "text"==strDisplayType ){
                        QLineEdit* pLEValue=qobject_cast<QLineEdit*>(it.value().pWidget);
                        pWriter->writeAttribute(QStringLiteral("value"),pLEValue->text().trimmed());
                    }
                    else if( "select"==strDisplayType ){
                        QComboBox* pComBox=qobject_cast<QComboBox*>(it.value().pWidget);
                        pWriter->writeAttribute(QStringLiteral("value"),pComBox->currentData().toString());
                    }
                }
                pWriter->writeEndElement();
            }
            pWriter->writeEndElement();
        }
    }
}

void SubConfigDispalyTree::initSubConfigDispalyTree( QString strSubCfgPath )
{
    this->setColumnCount(7);
//    this->setColumnHidden(2,true);
//    this->setColumnHidden(3,true);
//    this->setColumnHidden(4,true);
//    this->setColumnHidden(5,true);
//    this->setColumnHidden(6,true);
    QStringList headers;
    headers<<tr("Paramater Name")
                <<tr("Paramater Value")
                <<tr("Paramater ID")
                <<tr("Display Type")
                <<tr("Paramater Type")
                <<tr("Regular Expression")
                <<tr("Tips");
    this->setHeaderLabels(headers);
    this->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    readEquipmentInfo(strSubCfgPath);
}

int SubConfigDispalyTree::readEquipmentInfo(QString strPath)
{
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Open XML Failed,FilePath:"<<strPath;
        return -2;
    }

    QDomDocument doc;
    QString error = "";
    int row = 0, column = 0;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return -1;
    }
    file.close();

    //Subject
    QDomElement rootElem = doc.documentElement();
    if( !rootElem.isNull() ){
        if( "Subject" == rootElem.tagName() ){
            //subjectInfo
            QDomNode secNode = rootElem.firstChildElement();
            while( !secNode.isNull() ){
                QDomElement subInfoElement = secNode.toElement();
                if(!subInfoElement.isNull() && "subjectInfo" == subInfoElement.tagName()) {
                    if( "control"==subInfoElement.attribute("type") ){
                        //entity
                        QDomNode entNode = secNode.firstChildElement();
                        while( !entNode.isNull() ){
                            QDomElement entElem = entNode.toElement();
                            if( !entElem.isNull() && "entity" == entElem.tagName()){
                                if( "C"==entElem.attribute("flag") ){
                                    QTreeWidgetItem* pCMDItem=new QTreeWidgetItem(this);
                                    pCMDItem->setText(0,entElem.attribute("displayName").trimmed());
                                    pCMDItem->setText(2,entElem.attribute("cmd").trimmed());
                                    pCMDItem->setExpanded(true);

                                    //Attr
                                    QDomNode attrNode = entElem.firstChildElement();
                                    while( !attrNode.isNull() ){
                                        QDomElement attrElem = attrNode.toElement();
                                        if(!attrElem.isNull()  && "attribute" == attrElem.tagName()) {
                                            QTreeWidgetItem* pAttrItem=new QTreeWidgetItem(pCMDItem);

                                            QString strName=attrElem.attribute("displayName");
                                            QString strValue=attrElem.attribute("default");
                                            QString strID=attrElem.attribute("id");
                                            QString strDisplayType = attrElem.attribute("displayType");
                                            QString strParamType=attrElem.attribute("paramType");
                                            QString strRegExp = attrElem.attribute("validator");
                                            QString strTips = attrElem.attribute("tips");

                                            pAttrItem->setText(0,strName);
                                            if("text"==strDisplayType ){
                                                QLineEdit* pLEVal = new QLineEdit(strValue);
                                                //pLEVal->setFrame(false);
                                                connect(pLEVal,&QLineEdit::textEdited,this,&SubConfigDispalyTree::lineTextEdited);

                                                this->setItemWidget(pAttrItem,1,pLEVal);
                                                m_widgetMap.insert(pLEVal,pAttrItem);

                                                //刷新数据使用
                                                WidgetTreeItem wtItem;
                                                wtItem.pWidget = pLEVal;
                                                wtItem.pItem = pAttrItem;
                                                m_IdLinkedMap.insert(strID,wtItem);
                                            }
                                            else if("select"==strDisplayType){
                                                QComboBox* pCombo = new QComboBox();
                                                //pCombo->setFrame(false);
                                                QStringList paramTypes = strParamType.split(";");
                                                foreach (QString strPara, paramTypes) {
                                                   QStringList paramPair = strPara.split(":");
                                                   if( 2 == paramPair.size() && !paramPair.at(1).isEmpty()){
                                                       pCombo->addItem(paramPair.at(1),QVariant(paramPair.at(0)));
                                                   }
                                                }
                                                //设置当前默认选项
                                                int nItemCount = pCombo->count();
                                                for( int i=0; i<nItemCount; i++ ){
                                                    if( strValue == pCombo->itemText(i) ){
                                                        pCombo->setCurrentIndex(i);
                                                        break;
                                                    }
                                                }

                                                this->setItemWidget(pAttrItem,1,pCombo);
                                                //m_widgetMap.insert(pCombo,pAttrItem);

                                                //刷新数据使用
                                                WidgetTreeItem wtItem;
                                                wtItem.pWidget = pCombo;
                                                wtItem.pItem = pAttrItem;
                                                m_IdLinkedMap.insert(strID,wtItem);
                                            }
                                            pAttrItem->setText(2,strID);
                                            pAttrItem->setText(3,strDisplayType);
                                            pAttrItem->setText(4,strParamType);
                                            pAttrItem->setText(5,strRegExp);
                                            pAttrItem->setText(6,strTips);
                                        }
                                        attrNode = attrNode.nextSibling();
                                    }
                                }
                            }
                            entNode = entNode.nextSibling();
                        }
                    }
                }
                secNode = secNode.nextSibling();
            }
        }
    }

    return 0;
}

void SubConfigDispalyTree::lineTextEdited(QString strText){
    QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(sender());
    auto it = m_widgetMap.find(qobject_cast<QWidget*>(pLineEdit));
    if( it!=m_widgetMap.end() ){
        QTreeWidgetItem* pItem = it.value();
        qDebug()<<pItem->text(0);
        QString strRegExp = pItem->text(5).trimmed();
        if(!strRegExp.isEmpty()){
            QRegExp regx(strRegExp);
            QValidator *validator = new QRegExpValidator(regx, 0);
            int nPos = 0;
            if( QValidator::Acceptable != validator->validate(strText,nPos) ){
                // 错误提示
                QString strTips = pItem->text(6).trimmed();
                QToolTip::showText(pLineEdit->mapToGlobal(QPoint(100, 0)),strTips);
               // pLineEdit->clear();
            }
            else
            {
                QToolTip::hideText();
            }
        }
    }
}
