#include "PluginMoinitorDisplay.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QComboBox>
#include <QToolTip>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include "CDataPacket.h"
#include <QHostAddress>

PluginMoinitorDisplay::PluginMoinitorDisplay(QWidget *parent) : QWidget(parent){
    InitMainWindow();
}

void PluginMoinitorDisplay::InitMainWindow(){
    this->setMinimumSize(900,600);
    m_pHLayout = new QHBoxLayout(this);
    m_pSubMachine = new SubMachine(this);
    m_pHLayout->addWidget(m_pSubMachine);
}

void PluginMoinitorDisplay::closeEvent(QCloseEvent* ev){
    ev->ignore();
    this->hide();
}

void PluginMoinitorDisplay::setSubMachine(QString strName){
    if( NULL != m_pSubMachine ){
        m_pSubMachine->switchSubMachine(strName);
    }
}

SubMachine::SubMachine(QWidget *parent) : QTabWidget(parent)
{
    InitTabMain();
}

void SubMachine::InitTabMain(){
    m_pXML = new XML();
    if(m_pXML->getConfigureInfo(":/config")){
        QList<Rader_Total>::iterator it = m_pXML->mRaderTotals.begin();
        for( ;it!= m_pXML->mRaderTotals.end();it++ ){
            Rader_Total raderTotal = *it;
            //Subject
            QList<Subject> subjects = raderTotal.subjects;
            QList<Subject>::iterator itSubject = subjects.begin();
            for( ;itSubject!=subjects.end();itSubject++ ){
                Subject subject = *itSubject;
                //Tab Tag
                QTreeWidget* pCurTree = new QTreeWidget(this);
                pCurTree->setHeaderHidden(true);
                pCurTree->setColumnCount(4);
                pCurTree->setColumnWidth(0,300);
                pCurTree->setColumnWidth(1,200);
                pCurTree->setColumnWidth(2,100);
                pCurTree->setColumnWidth(3,100);

                m_subDevIds.push_back(subject.cSubId);
                //保存到队列中
                m_subDevTabs.push_back(pCurTree);
                //将分机名称放入列表
                m_subDevNames.push_back(subject.name);
                this->addTab(pCurTree,subject.displayName);
                //SubjectInfo
                QList<SubjectInfo> subjectInfos = subject.subjectInfos;
                QList<SubjectInfo>::iterator itSubInfo = subjectInfos.begin();
                for( ;itSubInfo != subjectInfos.end(); itSubInfo++){
                    SubjectInfo subjectInfo = *itSubInfo;
                    QTreeWidgetItem* pSubInfo = new QTreeWidgetItem(pCurTree);
                    pSubInfo->setText(0,subjectInfo.displayName);
                    //测试添加
                    pSubInfo->setText(3,subjectInfo.strCmdType);

                    //Entity
                    QList<Entity> ents = subjectInfo.ents;
                    QList<Entity>::iterator itEnts = ents.begin();
                    for( ;itEnts!=ents.end();itEnts++ ){
                        Entity ent = *itEnts;
                        QTreeWidgetItem* pEnt = new QTreeWidgetItem(pSubInfo);
                        pEnt->setText(0,ent.displayName);
                        pEnt->setText(3,ent.strCmd);

                        //Attr
                        QList<Attr> attrs = ent.attrs;
                        QList<Attr>::iterator itAttr = attrs.begin();
                        for( ;itAttr!=attrs.end(); ){
                            Attr attr = *itAttr;
                            if( attr.show ){
                                QTreeWidgetItem* pAttrItem = new QTreeWidgetItem(pEnt);
                                QLabel* pLable = new QLabel(attr.displayName);
                                pLable->setObjectName("Lable");
                                pCurTree->setItemWidget(pAttrItem,0,pLable);
                                if( "control" == subjectInfo.type ){
                                    if( "text" == attr.displayType ){
                                        QLineEdit* pLineEdit = new QLineEdit();
                                        pLineEdit->setObjectName("LineEdit");
                                        pLineEdit->setFrame(true);
                                        pLineEdit->setMaximumWidth(500);
                                        pLineEdit->setText(attr.defValue);
                                        pCurTree->setItemWidget(pAttrItem,1,pLineEdit);
                                        connect(pLineEdit,SIGNAL(textEdited(QString)),this,SLOT(lineTextEdited(QString)));
                                        QStringList strValidTips;
                                        strValidTips<<attr.validator<<attr.tips;
                                        m_lineEditTips.insert(pLineEdit,strValidTips);
                                    }
                                    else if("select"==attr.displayType){
                                        QComboBox* pCombo = new QComboBox();
                                        pCombo->setObjectName("ComboBox");
                                        pCombo->setFrame(true);
                                        pCombo->setMaximumWidth(500);
                                        //pCombo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                                        QStringList paramTypes = attr.paramType.split(";");
                                        pCombo->addItems(paramTypes);
                                        pCurTree->setItemWidget(pAttrItem,1,pCombo);
                                    }
                                    if( attr.addBtn ){
                                        QPushButton* pBtn = new QPushButton("设置",this);
                                        pBtn->setMaximumWidth(100);
                                        pBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                                        connect(pBtn,SIGNAL(clicked(bool)),this,SLOT(btnSetClicked()));
                                        pCurTree->setItemWidget(pAttrItem,2,pBtn);
                                        //将设置按钮与父Item关联
                                        m_btnMap.insert(pBtn,pEnt);
                                    }
                                }
                                if( "monitor" == subjectInfo.type ){
                                    pAttrItem->setText(1,attr.defValue);
                                }
                                pAttrItem->setText(3,attr.strID);
                                //记录当前数组对应的位置
                                m_DataProcess.updateRowItemMap(subject.cSubId,attr,subjectInfo.type,pAttrItem);
                            }
                            else{
                                m_DataProcess.updateRowItemMap(subject.cSubId,attr,subjectInfo.type,NULL);
                            }
                            itAttr++;
                        }
                    }
                }
                pCurTree->expandAll();
            }
        }
    }
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));

    //启动数据处理模块
    this->setCurrentIndex(0);
    m_pCurTree = m_subDevTabs.at(0);
    m_curSubId = m_subDevIds.at(0);
    m_DataProcess.startDataProcess(m_curSubId);
}

void SubMachine::switchSubMachine(QString strName){
    for( int i=0; i< m_subDevNames.size();i++ ){
        if( strName == m_subDevNames.at(i) ){
            m_pCurTree = m_subDevTabs.at(i);
            this->setCurrentIndex(i);
        }
    }
}

uchar SubMachine::hexToByteArray(QString strHex)
{
    QByteArray hexAry;
    strHex = strHex.trimmed();
    strHex = strHex.simplified();
    QStringList strHexList = strHex.split(" ");

    foreach (QString str, strHexList) {
        if( !str.isEmpty() ){
            bool ok = true;
            char cHex = str.toInt(&ok,16)&0xFF;
            if(ok){
                hexAry.append(cHex);
            }else{
                qDebug()<<"非法的16进制字符："<<str;
            }
        }
    }
    return hexAry.at(0);
}

void SubMachine::currentTabChanged(int index){
    m_pCurTree = m_subDevTabs.at(index);
    m_curSubId = m_subDevIds.at(index);
    m_DataProcess.resetSubDevID(m_curSubId);
}

void SubMachine::lineTextEdited(QString strText){
    QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(sender());
    QMap<QLineEdit*,QStringList>::iterator it = m_lineEditTips.find(pLineEdit);
    if( it != m_lineEditTips.end() ){
        QStringList strValidTips = it.value();
        QString strValid = strValidTips.at(0);
        QString strTips = strValidTips.at(1);
        QRegExp regx(strValid);
//        QValidator *validator = new QRegExpValidator(regx, 0);
//        int nPos = 0;
//        if( QValidator::Acceptable != validator->validate(strText,nPos) ){
//            // 错误提示
//            QToolTip::showText(pLineEdit->mapToGlobal(QPoint(100, 0)),strTips);
//            pLineEdit->clear();
//        }
    }
}

void SubMachine::btnSetClicked(){
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    QMap<QPushButton*,QTreeWidgetItem*>::iterator it = m_btnMap.find(pBtn);
    if( it != m_btnMap.end() ){
        QTreeWidgetItem* pEntItem = it.value();
        if( NULL != pEntItem ){
            //命令字
            uchar cmd = hexToByteArray(pEntItem->text(3));
            QList<RowItem*>rowItems;
            int nAttrs = pEntItem->childCount();
            for( int i=0;i<nAttrs;i++ ){
                QTreeWidgetItem* pAttrItem = pEntItem->child(i);
                QString strAttrId = pAttrItem->text(3);
                QString strValue = "";
                QWidget* pAttr = m_pCurTree->itemWidget(pAttrItem,0);
                if( "Lable" == pAttr->objectName() ){
                    QLabel* pLabel = (QLabel*)pAttr;
                }
                QWidget* pValue = m_pCurTree->itemWidget(pAttrItem,1);
                if( "LineEdit" == pValue->objectName() ){
                    QLineEdit* pLineEdit = (QLineEdit*)pValue;
                    strValue = pLineEdit->text().trimmed();
                }
                else if( "ComboBox" == pValue->objectName() ){
                    QComboBox* pComboBox = (QComboBox*)pValue;
                    QString strComVal = pComboBox->currentText().split(":").at(0);
                    strValue = strComVal.trimmed();
                }

                RowItem* pRowItem = m_DataProcess.getRowItem(strAttrId);
                if( NULL != pRowItem ){
                    pRowItem->displayValue = strValue;
                    rowItems.push_back(pRowItem);
                }
            }
            m_DataProcess.mappingToCmd(cmd,rowItems);
        }
    }
}
