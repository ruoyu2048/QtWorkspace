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
                pCurTree->setColumnCount(4);
                pCurTree->setColumnWidth(0,300);
                pCurTree->setColumnWidth(1,200);
                pCurTree->setColumnWidth(2,100);
                pCurTree->setColumnWidth(3,100);

                m_pCurTree = pCurTree;
                m_curSubId = subject.cSubId;
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
                                RowItem* pRowItem = new RowItem();
                                pRowItem->strId = attr.strID;
                                pRowItem->dataLen = attr.dataLen;
                                pRowItem->dataType = attr.dataType;
                                pRowItem->pAttrItem = pAttrItem;
                                mRowItemMap.insert(attr.strID,pRowItem);
                            }
                            else{
                                RowItem* pRowItem = new RowItem();
                                pRowItem->strId = attr.strID;
                                pRowItem->dataLen = attr.dataLen;
                                pRowItem->dataType = attr.dataType;
                                pRowItem->pAttrItem = NULL;
                                mRowItemMap.insert(attr.strID,pRowItem);
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
//    mCommCfg.strCommPara = "192.168.1.101:9999";
//    mCommCfg.strCommOther = "192.168.1.100:8080+0x21";
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

void SubMachine::packMessage(uchar cmd,QList<RowItem*> attrList){
    CDataPacket dataPkt;
    dataPkt.msgHead = 0xAA;
    dataPkt.msgDst = 0x21;
    dataPkt.msgSrc = 0x31;
    dataPkt.msgType = 0x20;
    CMD_DEV_BOOK cmdDevBook;
    cmdDevBook.cmd = cmd;
    int nSz = attrList.size();
    for(int i=0; i<nSz;i++ ){
        RowItem* pRowItem = attrList.at(i);
        if( "00002" == pRowItem->strId ){
            cmdDevBook.addrCode = pRowItem->displayValue.toInt();
        }
        if( "00003" == pRowItem->strId ){
            cmdDevBook.realAngle = pRowItem->displayValue.toShort();
        }
        if( "00004" == pRowItem->strId ){
            cmdDevBook.addrLong = pRowItem->displayValue.toDouble();
        }
        if( "00005" == pRowItem->strId ){
            cmdDevBook.addrLati = pRowItem->displayValue.toDouble();
        }
        if( "00006" == pRowItem->strId ){
            cmdDevBook.addrHeigh = pRowItem->displayValue.toFloat();
        }
        if( "00007" == pRowItem->strId ){
            cmdDevBook.gpsAngle = pRowItem->displayValue.toShort();
        }
        if( "00008" == pRowItem->strId ){
            cmdDevBook.addrInfo = pRowItem->displayValue.toInt();
        }
        if( "00010" == pRowItem->strId ){
            cmdDevBook.distance = pRowItem->displayValue.toShort();
        }
        if( "00011" == pRowItem->strId ){
            cmdDevBook.speed = pRowItem->displayValue.toShort();
        }
        if( "00012" == pRowItem->strId ){
            cmdDevBook.direction = pRowItem->displayValue.toShort();
        }
        if( "00013" == pRowItem->strId ){
            cmdDevBook.pitch = pRowItem->displayValue.toShort();
        }
        if( "00015" == pRowItem->strId ){
            //cmdDevBook.channel = pRowItem->displayValue.toShort();
            cmdDevBook.channel[0]=1;
            cmdDevBook.channel[1]=2;
        }
        if( "00016" == pRowItem->strId ){
            //cmdDevBook.ortho;
            cmdDevBook.ortho[0]=3;
            cmdDevBook.ortho[1]=4;
        }
        if( "00017" == pRowItem->strId ){
            //cmdDevBook.beam;
        }
    }
    dataPkt.msgData.append((char*)&cmdDevBook,sizeof(CMD_DEV_BOOK));
    QByteArray ary=dataPkt.packPacketToEncodedBytes();
    qDebug()<<"SendSz:"<<ary.length();
    mComm.sendDataPacket(&dataPkt);
}

void SubMachine::mappingToCmd(uchar cmd,QList<RowItem*> attrList){

    switch (m_curSubId) {
    case 0x21:{//dsp1
        packMessage(cmd,attrList);
        break;
    }
    }
}

RowItem* SubMachine::getRowItem(QString strId){
    auto it = mRowItemMap.find(strId);
    if( it != mRowItemMap.end() )
        return it.value();

    return NULL;
}

void SubMachine::updateRowItem(QString strId,QString itemVal){
    auto it = mRowItemMap.find(strId);
    if( it != mRowItemMap.end() )
        it.value()->pAttrItem->setText(1,itemVal);
}

void SubMachine::currentTabChanged(int index){
    m_pCurTree = m_treeWidgetList.at(index);
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

                RowItem* pRowItem = getRowItem(strAttrId);
                if( NULL != pRowItem ){
                    pRowItem->displayValue = strValue;
                    rowItems.push_back(pRowItem);
                }
            }
            mappingToCmd(cmd,rowItems);
        }
    }
}

void SubMachine::updateTabView(CDataPacket *dataPkt){
    //解码
    dataPkt->decodeData();
    //雷达状态信息
    if( 0x31 == dataPkt->msgDst && 0x42 == dataPkt->msgSrc && 0x30 == dataPkt->msgType ){
        LADAR_INFO ladarInfo;
        dataPkt->parseDataToStruct((uchar*)&ladarInfo,sizeof(LADAR_INFO));
        //DSP1状态信息
        updateDSP1State(ladarInfo.dsp1State);
    }
}

void SubMachine::updateDSP1State(DSP1 &dsp1){
    char cBuf[COMMAXLEN] = {'\0'};
    memmove(cBuf,dsp1.ver,3);
    updateRowItem("20001",cBuf);
    updateRowItem("20002",QString("%1").arg((quint8)dsp1.alarmState));
    updateRowItem("20003",QString("%1").arg((quint8)dsp1.alarmCode1));
    updateRowItem("20004",QString("%1").arg((quint8)dsp1.alarmCode2));
    updateRowItem("20005",QString("%1").arg((quint8)dsp1.inState));
    updateRowItem("20006",QHostAddress(dsp1.IP).toString());
    updateRowItem("20007",QString("%1").arg(dsp1.port1));
    updateRowItem("20008",QString("%1").arg(dsp1.port2));
    updateRowItem("20009",QString("%1").arg(dsp1.port3));
    updateRowItem("20010",QString("%1").arg(dsp1.cpu));
    updateRowItem("20011",QString("%1").arg(dsp1.ddr));
    updateRowItem("20012",QString("%1").arg(dsp1.commRate));
    updateRowItem("20013",QString("%1").arg(dsp1.SRIORate));
    updateRowItem("20014",QString("%1").arg(dsp1.SRIOID));
    updateRowItem("20015",QString("%1").arg((quint8)dsp1.SRIOMode));
    updateRowItem("20016",QString("%1").arg((quint8)dsp1.SRIDAlarm));
    //updateRowItem("20017",QString("%1").arg(dsp1.bei));
    //updateRowItem("20018",QString("%1").arg(dsp1.bei1));
    updateRowItem("20019",QString("%1").arg((quint8)dsp1.h0State));
    ///updateRowItem("20020",QString("%1").arg(dsp1.h0bei));
    updateRowItem("20021",QString("%1").arg((quint8)dsp1.h1State));
    //updateRowItem("20022",QString("%1").arg(dsp1.h1be));
    updateRowItem("20023",QString("%1").arg((quint8)dsp1.h2State));
    //updateRowItem("20024",QString("%1").arg(dsp1.h2bei));
    updateRowItem("20025",QString("%1").arg((quint8)dsp1.h3State));
    //updateRowItem("20026",QString("%1").arg(dsp1.h3bei));
    updateRowItem("20027",QString("%1").arg((quint8)dsp1.h4State));
    //updateRowItem("20028",QString("%1").arg(dsp1.h4bei));
    updateRowItem("20029",QString("%1").arg((quint8)dsp1.h5State));
    //updateRowItem("20030",QString("%1").arg(dsp1.h5bei));
    updateRowItem("20031",QString("%1").arg((quint8)dsp1.h6State));
    //updateRowItem("20032",QString("%1").arg(dsp1.h6bei));
    updateRowItem("20033",QString("%1").arg((quint8)dsp1.h7State));
    //updateRowItem("20034",QString("%1").arg(dsp1.h7bei));
    updateRowItem("20035",QString("%1").arg(dsp1.FPGAVer));
    updateRowItem("20036",QString("%1").arg((quint8)dsp1.FPGAAlarm));
    updateRowItem("20037",QHostAddress(dsp1.FPGAip).toString());
    updateRowItem("20038",QString("%1").arg(dsp1.FPGAport1));
    updateRowItem("20039",QString("%1").arg(dsp1.FPGAport2));
    updateRowItem("20040",QString("%1").arg(dsp1.FPGAport3));
    updateRowItem("20041",QString("%1").arg(dsp1.FPGABig));
    updateRowItem("20042",QString("%1").arg((quint8)dsp1.FPGADDR));
    updateRowItem("20043",QString("%1").arg(dsp1.FPGASRAM));
    updateRowItem("20044",QString("%1").arg((quint8)dsp1.FPGAComm1));
    updateRowItem("20045",QString("%1").arg((quint8)dsp1.FPGAComm2));
    updateRowItem("20046",QString("%1").arg((quint8)dsp1.FPGAComm3));
    updateRowItem("20047",QString("%1").arg((quint8)dsp1.FPGAComm4));
    updateRowItem("20048",QString("%1").arg(dsp1.FPGAGx1));
    updateRowItem("20049",QString("%1").arg(dsp1.FPGAGx2));
    updateRowItem("20050",QString("%1").arg(dsp1.FPGAGx3));
    updateRowItem("20051",QString("%1").arg(dsp1.FPGAGx4));
    updateRowItem("20052",QString("%1").arg(dsp1.FPGACPCI));
    updateRowItem("20053",QString("%1").arg(dsp1.FPGAPCIE));
    updateRowItem("20054",QString("%1").arg(dsp1.canRate));
    updateRowItem("20055",QString("%1").arg(dsp1.CANID));
    updateRowItem("20056",QString("%1").arg((quint8)dsp1.canBZ));
    updateRowItem("20057",QString("%1").arg((quint8)dsp1.FPGA_SRIO));
    updateRowItem("20058",QString("%1").arg(dsp1.FPGA_SRIO_ID1));
    updateRowItem("20059",QString("%1").arg(dsp1.FPGA_SRIO_ID2));
    updateRowItem("20060",QString("%1").arg(dsp1.FPGA_SRIO_ID3));
    updateRowItem("20061",QString("%1").arg(dsp1.FPGA_SRIO_ID4));
    updateRowItem("20062",QString("%1").arg((quint8)dsp1.FPGA_SRIO_Mode));
    updateRowItem("20063",QString("%1").arg((quint8)dsp1.FPGA_SRIO_ALARM));
    updateRowItem("20064",QString("%1").arg((quint8)dsp1.FPGA_in));
    updateRowItem("20065",QString("%1").arg((quint8)dsp1.FPGA_alarmCode1));
    updateRowItem("20066",QString("%1").arg((quint8)dsp1.FPGA_alarmCode2));
    //updateRowItem("20067",QString("%1").arg((quint8)dsp1.bei2));
    updateRowItem("20068",QString("%1").arg((quint8)dsp1.kjxs));
    updateRowItem("20069",QString("%1").arg((quint8)dsp1.dwdx));
    //updateRowItem("20070",QString("%1").arg(dsp1.bei3));
    //updateRowItem("20071",QString("%1").arg(dsp1.fpgabei);
}

