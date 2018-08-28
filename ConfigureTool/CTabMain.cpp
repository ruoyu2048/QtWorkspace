#include "CTabMain.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QComboBox>
#include <QToolTip>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>

CTabMain::CTabMain(QWidget *parent) : QTabWidget(parent)
{
    InitTabMain();
}

void CTabMain::InitTabMain()
{
    m_pXML = new XML();
    if(m_pXML->getConfigureInfo(":/config")){
        QList<Rader_Total>::iterator it = m_pXML->mRaderTotals.begin();
        for( ;it!= m_pXML->mRaderTotals.end();it++ ){
            //Tab Tag
            Rader_Total raderTotal = *it;
            //qDebug()<<"A\t"<<raderTotal.displayName<<raderTotal.subjects.size();
            QTreeWidget* pCurTree = new QTreeWidget(this);
            pCurTree->setHeaderHidden(true);
            connect(pCurTree,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
            pCurTree->setColumnCount(3);
            pCurTree->setColumnWidth(0,300);
            this->addTab(pCurTree,raderTotal.displayName);
            //保存到队列中
            m_treeWidgetList.push_back(pCurTree);
            m_pCurTree = pCurTree;

            //Subject
            QList<Subject> subjects = raderTotal.subjects;
            QList<Subject>::iterator itSubject = subjects.begin();
            for( ;itSubject!=subjects.end();itSubject++ ){
                Subject subject = *itSubject;
                //qDebug()<<"B\t\t"<<subject.displayName;
                QTreeWidgetItem* pSubject = new QTreeWidgetItem(pCurTree);
                pSubject->setText(0,subject.displayName);

                //SubjectInfo
                QList<SubjectInfo> subjectInfos = subject.subjectInfos;
                QList<SubjectInfo>::iterator itSubInfo = subjectInfos.begin();
                for( ;itSubInfo != subjectInfos.end(); itSubInfo++){
                    SubjectInfo subjectInfo = *itSubInfo;
                    //qDebug()<<"C\t\t\t"<<subjectInfo.displayName<<subjectInfo.ents.size();
                    QTreeWidgetItem* pSubInfo = new QTreeWidgetItem(pSubject);
                    pSubInfo->setText(0,subjectInfo.displayName);

                    //Entity
                    QList<Entity> ents = subjectInfo.ents;
                    QList<Entity>::iterator itEnts = ents.begin();
                    for( ;itEnts!=ents.end();itEnts++ ){
                        Entity ent = *itEnts;
                        //qDebug()<<"D\t\t\t\t"<<ent.displayName;
                        QTreeWidgetItem* pEnt = new QTreeWidgetItem(pSubInfo);
                        pEnt->setText(0,ent.displayName);

                        //Attr
                        QList<Attr> attrs = ent.attrs;
                        QList<Attr>::iterator itAttr = attrs.begin();
                        for( ;itAttr!=attrs.end();itAttr++ ){
                            Attr attr = *itAttr;
                            //qDebug()<<"E\t\t\t\t\t"<<attr.displayName;
                            QTreeWidgetItem* pAttr = new QTreeWidgetItem(pEnt);

                            QLabel* pLable = new QLabel(attr.displayName);
                            pLable->setObjectName("Lable");
                            pCurTree->setItemWidget(pAttr,0,pLable);
                            if( "control" == subjectInfo.type )
                            {
                                if( "text" == attr.displayType )
                                {
                                    QLineEdit* pLineEdit = new QLineEdit();
                                    pLineEdit->setObjectName("LineEdit");
                                    pLineEdit->setFrame(true);
                                    pLineEdit->setMaximumWidth(100);
                                    pCurTree->setItemWidget(pAttr,1,pLineEdit);
                                    connect(pLineEdit,SIGNAL(textEdited(QString)),this,SLOT(lineTextEdited(QString)));
                                }
                                else if("select"==attr.displayType)
                                {
                                    QComboBox* pCombo = new QComboBox();
                                    pCombo->setObjectName("ComboBox");
                                    pCombo->setFrame(true);
                                    pCombo->setMaximumWidth(100);
                                    pCombo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                                    QStringList paramTypes = attr.paramType.split(";");
                                    pCombo->addItems(paramTypes);
                                    pCurTree->setItemWidget(pAttr,1,pCombo);
                                }
                            }
                            if( "monitor" == subjectInfo.type ){
                                pAttr->setText(1,attr.value);
                            }
                        }
                    }
                }
            }
            pCurTree->expandAll();
        }
    }
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
}

void CTabMain::saveConfigureInfo()
{
    QTreeWidgetItemIterator it(m_pCurTree);
    while( *it )
    {
        QTreeWidgetItem* pItem = (*it);
        if( 0 == pItem->childCount() )
        {
            QWidget* pAttr = m_pCurTree->itemWidget(pItem,0);
            if( "Label" == pAttr->objectName() )
            {
                QLabel* pLabel = (QLabel*)m_pCurTree->itemWidget(pItem,0);
                qDebug()<<pLabel->text();
            }
            QWidget* pValue = m_pCurTree->itemWidget(pItem,1);
            if( "LineEdit" == pValue->objectName() )
            {
                QLineEdit* pLineEdit = (QLineEdit*)pValue;
                qDebug()<<pLineEdit->text();
            }
            else if( "ComboBox" == pValue->objectName() )
            {
                QComboBox* pComboBox = (QComboBox*)pValue;
                qDebug()<<pComboBox->currentText();
            }
        }
        else
        {
            qDebug()<<pItem->text(0);
        }
        it++;
    }
}

void CTabMain::currentTabChanged(int index)
{
    m_pCurTree = m_treeWidgetList.at(index);
}

void CTabMain::itemClicked(QModelIndex modelIndex)
{
    qDebug()<<modelIndex.parent().row()<<modelIndex.row()<<modelIndex.column();
}

void CTabMain::lineTextEdited(QString strText)
{
    QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(sender());
    QMap<QLineEdit*,QString>::iterator it = m_lineEditMap.find(pLineEdit);
    if( it != m_lineEditMap.end() )
    {
        //QRegExp regx("[0-9]+$");
        QRegExp regx(it.value());
        QValidator *validator = new QRegExpValidator(regx, 0);
        int nPos = 0;
        if( QValidator::Acceptable != validator->validate(strText,nPos) )
        {
            // 错误提示
            QToolTip::showText(pLineEdit->mapToGlobal( QPoint(100, 0)  ),"请输入数字!");
            pLineEdit->clear();
        }
    }
}
