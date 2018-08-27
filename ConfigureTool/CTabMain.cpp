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
            Rader_Total raderTotal = *it;
            qDebug()<<"A\t"<<raderTotal.displayName<<raderTotal.subjects.size();

            QList<Subject> subjects = raderTotal.subjects;
            QList<Subject>::iterator itSubject = subjects.begin();
            for( ;itSubject!=subjects.end();it++ ){
                Subject subject = *itSubject;
                qDebug()<<"B\t\t"<<subject.displayName;
            }
        }

//        QList<RaderInfo>::iterator it = m_pXML->m_raderInfoList.begin();
//        for(;it!=m_pXML->m_raderInfoList.end();it++){
//            QTreeWidget* pCurTree = new QTreeWidget(this);
//            pCurTree->setHeaderHidden(true);
//            connect(pCurTree,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
//            pCurTree->setColumnCount(3);
//            pCurTree->setColumnWidth(0,300);
//            //pCurTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
//            RaderInfo raderInfo = *it;
//            qDebug()<<raderInfo.displayName;
//            this->addTab(pCurTree,raderInfo.displayName);
//            //保存到队列中
//            m_treeWidgetList.push_back(pCurTree);
//            m_pCurTree = pCurTree;

//            qDebug()<<raderInfo.secLevel.description<<raderInfo.secLevel.displayName;
//            QTreeWidgetItem* pFirstItem = new QTreeWidgetItem(pCurTree);
//            pFirstItem->setText(0,raderInfo.secLevel.displayName);

//            QList<Third_Level>::iterator itThird = raderInfo.secLevel.ents.begin();
//            for(;itThird!=raderInfo.secLevel.ents.end();itThird++)
//            {
//                QTreeWidgetItem* pSecItem = new QTreeWidgetItem(pFirstItem);
//                //pData->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
//                pSecItem->setText(0,(*itThird).displayName);

//                QList<Forth_Level>::iterator itForth=(*itThird).attrs.begin();
//                for(;itForth !=(*itThird).attrs.end();itForth++ )
//                {
//                    QTreeWidgetItem* pThirdItem = new QTreeWidgetItem(pSecItem);
//                    pThirdItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
//                    QLabel* pLable = new QLabel((*itForth).displayName);
//                    pLable->setObjectName("Lable");
//                    //pLable->setMinimumWidth(500);
//                    pCurTree->setItemWidget(pThirdItem,0,pLable);
//                    if( "text" == (*itForth).displayType )
//                    {
//                        QLineEdit* pLineEdit = new QLineEdit();
//                        pLineEdit->setObjectName("LineEdit");
//                        pLineEdit->setFrame(false);
//                        pCurTree->setItemWidget(pThirdItem,1,pLineEdit);
//                        connect(pLineEdit,SIGNAL(textEdited(QString)),this,SLOT(lineTextEdited(QString)));
//                        m_lineEditMap.insert(pLineEdit,"[0-9]+$");
//                    }
//                    else if("select"==(*itForth).displayType)
//                    {
//                        QComboBox* pCombo = new QComboBox();
//                        pCombo->setObjectName("ComboBox");
//                        pCombo->setFrame(false);
//                        pCombo->setMaximumWidth(100);
//                        pCombo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//                        QStringList paramTypes = (*itForth).paramType.split(";");
//                        pCombo->addItems(paramTypes);
//                        pCurTree->setItemWidget(pThirdItem,1,pCombo);
//                    }
//                }
//            }
//            pCurTree->expandAll();
//        }
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
