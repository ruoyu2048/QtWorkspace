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

PluginMoinitorDisplay::PluginMoinitorDisplay(QWidget *parent) : QWidget(parent){
    InitMainWindow();
}

void PluginMoinitorDisplay::InitMainWindow(){
    this->setMinimumSize(600,400);
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
    startCommu();
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
                pCurTree->setColumnCount(3);
                pCurTree->setColumnWidth(0,300);
                pCurTree->setColumnWidth(1,200);
                pCurTree->setColumnWidth(2,100);

                m_pCurTree = pCurTree;
                //保存到队列中
                m_treeWidgetList.push_back(pCurTree);
                //将分机名称放入列表
                m_subMachines.push_back(subject.name);
                this->addTab(pCurTree,subject.displayName);
                //SubjectInfo
                QList<SubjectInfo> subjectInfos = subject.subjectInfos;
                QList<SubjectInfo>::iterator itSubInfo = subjectInfos.begin();
                for( ;itSubInfo != subjectInfos.end(); itSubInfo++){
                    SubjectInfo subjectInfo = *itSubInfo;
                    QTreeWidgetItem* pSubInfo = new QTreeWidgetItem(pCurTree);
                    pSubInfo->setText(0,subjectInfo.displayName);
                    //测试添加
                    pSubInfo->setText(1,subjectInfo.strCmdType);

                    //Entity
                    QList<Entity> ents = subjectInfo.ents;
                    QList<Entity>::iterator itEnts = ents.begin();
                    for( ;itEnts!=ents.end();itEnts++ ){
                        Entity ent = *itEnts;
                        QTreeWidgetItem* pEnt = new QTreeWidgetItem(pSubInfo);
                        pEnt->setText(0,ent.displayName);
                        //测试添加
                        pEnt->setText(1,ent.strCmd);
                        if( ent.name=="channel")
                            mpEnt = pEnt;

                        //Attr
                        QList<Attr> attrs = ent.attrs;
                        QList<Attr>::iterator itAttr = attrs.begin();
                        for( ;itAttr!=attrs.end(); ){
                            Attr attr = *itAttr;
                            if( attr.show ){
                                QTreeWidgetItem* pAttr = new QTreeWidgetItem(pEnt);
                                QLabel* pLable = new QLabel(attr.displayName);
                                pLable->setObjectName("Lable");
                                pCurTree->setItemWidget(pAttr,0,pLable);
                                if( "control" == subjectInfo.type ){
                                    if( "text" == attr.displayType ){
                                        QLineEdit* pLineEdit = new QLineEdit();
                                        pLineEdit->setObjectName("LineEdit");
                                        pLineEdit->setFrame(true);
                                        pLineEdit->setMaximumWidth(500);
                                        pLineEdit->setText(attr.defValue);
                                        pCurTree->setItemWidget(pAttr,1,pLineEdit);
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
                                        pCurTree->setItemWidget(pAttr,1,pCombo);
                                    }
                                    if( attr.addBtn ){
                                        QPushButton* pBtn = new QPushButton("设置",this);
                                        pBtn->setMaximumWidth(100);
                                        pBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                                        connect(pBtn,SIGNAL(clicked(bool)),this,SLOT(btnSetClicked()));
                                        pCurTree->setItemWidget(pAttr,2,pBtn);
                                        //将设置按钮与父Item关联
                                        m_btnMap.insert(pBtn,pEnt);
                                    }
                                }
                                if( "monitor" == subjectInfo.type ){
                                    pAttr->setText(1,attr.defValue);
                                }
                            }
                            itAttr++;
                        }
                        QString strIds=subject.strSubId;
                        strIds.append("+").append(subjectInfo.strCmdType);
                        strIds.append("+").append(ent.strCmd);
                        if( "control" == subjectInfo.type ){
                            mCtrlMap.insert(pEnt,strIds);
                        }
                        if( "monitor" == subjectInfo.type ){
                            mMonitorMap.insert(strIds,pEnt);
                        }
                    }
                }
                pCurTree->expandAll();
            }
        }
    }
    connect(this,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
}

void SubMachine::switchSubMachine(QString strName){
    for( int i=0; i< m_subMachines.size();i++ ){
        if( strName == m_subMachines.at(i) ){
            m_pCurTree = m_treeWidgetList.at(i);
            this->setCurrentIndex(i);
        }
    }
}

void SubMachine::startCommu(){
    mCommCfg.bSimmulator = false;
    mCommCfg.commType = UDP;
    mCommCfg.strCommPara = "127.0.0.1:9999";
    mCommCfg.strCommOther = "127.0.0.1:9998+0xA2";
    mComm.startCommunication(&mCommCfg);
    connect(&mComm,SIGNAL(receivedDataPacket(CDataPacket*)),this,SLOT(updateTabView(CDataPacket*)));
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

void SubMachine::packMessage(QString strIds, QStringList attrList){
    QStringList cmdList = strIds.split("+");
    if( cmdList.size() >= 3 ){
        CDataPacket dataPkt;
        dataPkt.msgHead = 0xAA;
        dataPkt.msgEnd = 0xA5;
        uchar cSubId = hexToByteArray(cmdList.at(0));
        uchar cMsgType = hexToByteArray(cmdList.at(1));
        uchar cCmd = hexToByteArray(cmdList.at(2));
        if( 0x11 == cSubId && 0x02 == cMsgType ){
            switch (cCmd) {
            case 0x20:{//控制类型
                dataPkt.msgDst = 0xA2;
                dataPkt.msgSrc = 0xA3;
                dataPkt.msgType = 0x11;

                //某分机的某报文
                CTTest ctTest;
                ctTest.cCmd = cCmd;
                ctTest.cWorkMode = attrList.at(0).toInt();
                ctTest.nShort = attrList.at(1).toShort();
                ctTest.fData = attrList.at(2).toFloat();

                dataPkt.msgData.append((char*)&ctTest,sizeof(CTTest));
                dataPkt.packPacketToEncodedBytes();
                mComm.sendDataPacket(&dataPkt);
                qDebug()<<"发送报文";
                }
                break;
            }
        }
    }
}

void SubMachine::mappingToCmd(QTreeWidgetItem* pEntItem,QStringList strInfos){
    if( NULL != pEntItem ){
        auto it = mCtrlMap.find(pEntItem);
        if( it != mCtrlMap.end() ){
            packMessage(it.value(),strInfos);
        }
    }
}

void SubMachine::currentTabChanged(int index){
    m_pCurTree = m_treeWidgetList.at(index);
}

void SubMachine::lineTextEdited(QString strText){
    QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(sender());
    QMap<QLineEdit*,QStringList>::iterator it = m_lineEditTips.find(pLineEdit);
    if( it != m_lineEditTips.end() ){
        //QRegExp regx("[0-9]+$");
        QStringList strValidTips = it.value();
        QString strValid = strValidTips.at(0);
        QString strTips = strValidTips.at(1);
        QRegExp regx(strValid);
        QValidator *validator = new QRegExpValidator(regx, 0);
        int nPos = 0;
        if( QValidator::Acceptable != validator->validate(strText,nPos) ){
            // 错误提示
            QToolTip::showText(pLineEdit->mapToGlobal(QPoint(100, 0)),strTips);
            pLineEdit->clear();
        }
    }
}

void SubMachine::btnSetClicked(){
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    QMap<QPushButton*,QTreeWidgetItem*>::iterator it = m_btnMap.find(pBtn);
    if( it != m_btnMap.end() ){
        QTreeWidgetItem* pEntItem = it.value();
        if( NULL != pEntItem ){
            int nAttrs = pEntItem->childCount();
            QStringList attrList;
            for( int i=0;i<nAttrs;i++ ){
                QTreeWidgetItem* pAttrItem = pEntItem->child(i);
                QWidget* pAttr = m_pCurTree->itemWidget(pAttrItem,0);
                if( "Lable" == pAttr->objectName() ){
                    QLabel* pLabel = (QLabel*)pAttr;
                    qDebug()<<pLabel->text();
                }
                QWidget* pValue = m_pCurTree->itemWidget(pAttrItem,1);
                if( "LineEdit" == pValue->objectName() ){
                    QLineEdit* pLineEdit = (QLineEdit*)pValue;
                    qDebug()<<pLineEdit->text();
                    attrList.append(pLineEdit->text().trimmed());
                }
                else if( "ComboBox" == pValue->objectName() ){
                    QComboBox* pComboBox = (QComboBox*)pValue;
                    qDebug()<<pComboBox->currentText();
                    QString strComVal = pComboBox->currentText().split(":").at(0);
                    attrList.append(strComVal.trimmed());
                }
            }
            mappingToCmd(pEntItem,attrList);
        }
    }
}

void SubMachine::updateTabView(CDataPacket *dataPkt){
    dataPkt->decodeData();
    uchar cMsgDst = dataPkt->msgDst;
    uchar cMsgSrc = dataPkt->msgSrc;
    uchar cmsgType = dataPkt->msgType;

    CTTest stats;
    dataPkt->parseDataToStruct((uchar*)&stats,sizeof(stats));
    qDebug()<<stats.nShort<<"|";
    QString strWM = QString("%1").arg((int)stats.cWorkMode);
    QString strShort = QString("%1").arg(stats.nShort);
    QString strFloat = QString("%1").arg(stats.fData);
    QStringList ss;
    ss<<strWM<<strShort<<strFloat;

    int n = mpEnt->childCount();
    for(int i=0;i<n;i++){
        QTreeWidgetItem* pAttr = mpEnt->child(i);
        pAttr->setText(1,ss.at(i));
    }

}

