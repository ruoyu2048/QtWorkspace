#include "PluginMacroConfig.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlStreamWriter>

#include "FileNameDlg.h"

PluginMacroConfig::PluginMacroConfig(QWidget *parent):
    QWidget(parent),
    m_pGLMain(nullptr),
    m_pLBCfgFileName(nullptr),
    m_pCBCfgFileName(nullptr),
    m_pBtnStartCfg(nullptr),
    m_pBtnSaveCfg(nullptr),
    m_pBtnGetCfg(nullptr),
    m_pBtnAddCfg(nullptr),
    m_pBtnDeleteCfg(nullptr),
    m_pBtnJobRelationCfg(nullptr),
    m_pCfgManageTree(nullptr),
    m_pCfgManageRootItem(nullptr),
    m_pCfgDispalyTab(nullptr),
    m_pCfgSubTree(nullptr)
{
    initMainWindow();
}

PluginMacroConfig::~PluginMacroConfig()
{

}

void PluginMacroConfig::initMainWindow()
{
    initContrals();
    initDeviceCfgManageTree();
    initSubCfgDisplayTab();
    showWindow(0);
}

void PluginMacroConfig::initContrals()
{
    m_pGLMain = new QGridLayout();
    this->setLayout(m_pGLMain);
    this->setMinimumWidth(940);
    this->setMinimumHeight(600);

//    m_pLBCfgFileName = new QLabel(tr("Configuration file name:"));
//    m_pCBCfgFileName = new QComboBox();
//    m_pBtnStartCfg = new QPushButton("Start configuring");
//    m_pBtnSaveCfg = new QPushButton("Save configuration");
//    m_pBtnGetCfg = new QPushButton("Get configuration");
//    m_pBtnAddCfg = new QPushButton("Add configuration");
//    m_pBtnDeleteCfg = new QPushButton("Delete configuration");
//    m_pBtnJobRelationCfg = new QPushButton("Job relation configuration");

    m_pLBCfgFileName = new QLabel(QStringLiteral("配置文件名称:"));
    m_pCBCfgFileName = new QComboBox();
    m_pBtnStartCfg = new QPushButton(QStringLiteral("开始配置"));
    m_pBtnSaveCfg = new QPushButton(QStringLiteral("保存配置"));
    m_pBtnGetCfg = new QPushButton(QStringLiteral("获取配置"));
    m_pBtnAddCfg = new QPushButton(QStringLiteral("添加配置"));
    m_pBtnDeleteCfg = new QPushButton(QStringLiteral("删除配置"));
    m_pBtnJobRelationCfg = new QPushButton(QStringLiteral("作业关系配置"));

    m_pBtnGetCfg->setEnabled(false);
    m_pBtnJobRelationCfg->setEnabled(false);

    m_pGLMain->addWidget(m_pLBCfgFileName,0,0,1,1);
    m_pGLMain->addWidget(m_pCBCfgFileName,0,1,1,1);
    m_pGLMain->addWidget(m_pBtnStartCfg,0,2,1,1);
    m_pGLMain->addWidget(m_pBtnSaveCfg,0,3,1,1);
    m_pGLMain->addWidget(m_pBtnGetCfg,0,4,1,1);
    m_pGLMain->addWidget(m_pBtnAddCfg,0,5,1,1);
    m_pGLMain->addWidget(m_pBtnDeleteCfg,0,6,1,1);
    m_pGLMain->addWidget(m_pBtnJobRelationCfg,0,7,1,1);

    connect(m_pCBCfgFileName,SIGNAL(currentIndexChanged(int)),this,SLOT(onCBCfgFileName(int)));
    connect(m_pBtnStartCfg,&QPushButton::clicked,this,&PluginMacroConfig::onBtnStartCfg);
    connect(m_pBtnSaveCfg,&QPushButton::clicked,this,&PluginMacroConfig::onBtnSaveCfg);
    connect(m_pBtnGetCfg,&QPushButton::clicked,this,&PluginMacroConfig::onBtnGetCfg);
    connect(m_pBtnAddCfg,&QPushButton::clicked,this,&PluginMacroConfig::onBtnAddCfg);
    connect(m_pBtnDeleteCfg,&QPushButton::clicked,this,&PluginMacroConfig::onBtnDeleteCfg);
    connect(m_pBtnJobRelationCfg,&QPushButton::clicked,this,&PluginMacroConfig::onBtnJobRelationCfg);

    //初始化宏配置文件列表
    initConfigFileNameCommbox();

}

void PluginMacroConfig::initConfigFileNameCommbox()
{
    QString strDirPath=QDir::currentPath()+"/config/macro_config_files";
    QDir dir(strDirPath);
    if( !dir.isEmpty() ){
        QFileInfoList filesList = dir.entryInfoList(QStringList("*.xml"));
        foreach (QFileInfo fi, filesList) {
            m_pCBCfgFileName->addItem(fi.baseName(),QVariant(fi.absoluteFilePath()));
        }
    }
}

void PluginMacroConfig::initDeviceCfgManageTree()
{
    m_pCfgManageTree = new QTreeWidget();
    m_pCfgManageTree->setColumnCount(2);
    //m_pCfgManageTree->setColumnHidden(1,true);
    m_pCfgManageTree->header()->setVisible(false);
    m_pCfgManageTree->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    m_pGLMain->addWidget(m_pCfgManageTree,1,0,1,2);
    connect(m_pCfgManageTree,&QTreeWidget::itemChanged,this,&PluginMacroConfig::onItemChanged);
    connect(m_pCfgManageTree,&QTreeWidget::itemClicked,this,&PluginMacroConfig::onItemClicked);

    QStringList headerList;
    //headerList<<tr("Device Configuration Management")<<tr("SubID");
    headerList<<QStringLiteral("设备配置管理")<<QStringLiteral("分机ID");
    m_pCfgManageRootItem = new QTreeWidgetItem(m_pCfgManageTree,headerList);
    m_pCfgManageRootItem->setCheckState(0,Qt::Unchecked);
    m_pCfgManageRootItem->setExpanded(true);
}


void PluginMacroConfig::initSubCfgDisplayTab()
{
    m_pCfgDispalyTab=new QTabWidget();
    m_pCfgDispalyTab->setTabShape(QTabWidget::Rounded);
    connect(m_pCfgDispalyTab,&QTabWidget::currentChanged,this,&PluginMacroConfig::onCurrentTabChanged);

    m_pGLMain->addWidget(m_pCfgDispalyTab,1,2,1,6);
}

void PluginMacroConfig::showWindow(int nRadarId)
{
//    if( !this->isHidden() )
//        this->close();
//    this->show();

    resetSubCfgDisplayTabItem(nRadarId);
}

void PluginMacroConfig::resetSubCfgDisplayTabItem(int nRadarId)
{
    QList<SubCfgInfo> cfgInfoList;
    if( getSubCfgInfo(nRadarId,cfgInfoList) ){
        foreach( SubCfgInfo subCfgInfo,cfgInfoList){
            //分机配置文件Tab控件
            SubConfigDispalyTree* pSubCfgTree = new SubConfigDispalyTree(subCfgInfo.strAbsFilePath);
            m_pCfgDispalyTab->addTab(pSubCfgTree,subCfgInfo.strName);

            //设备管理树
            QStringList columValueList;
            columValueList<<subCfgInfo.strName<<subCfgInfo.strSubId;
            QTreeWidgetItem* pSubItem = new QTreeWidgetItem(m_pCfgManageRootItem,columValueList);
            pSubItem->setCheckState(0,Qt::Unchecked);
            pSubItem->setExpanded(true);
        }
    }
}

bool PluginMacroConfig::getSubCfgInfo(int nRadarId, QList<SubCfgInfo> &cfgInfoList)
{
    QString strDirPath=QDir::currentPath()+"/config/monitor_config/Rander_01";
    QDir dir(strDirPath);
    if( !dir.isEmpty() ){
        //先找总的配置文件
        QFileInfoList filesList = dir.entryInfoList(QStringList("*.xml"),QDir::Files,QDir::DirsLast);
        QFileInfoList foldersList = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot,QDir::DirsFirst);
        //判断是否有冗余的配置文件
        if( 1 == filesList.size() && 1 == foldersList.size() ){
            //再获取分机文件的路径
            QString strSubDirPath = foldersList.first().absoluteFilePath();
            QDir subDir(strSubDirPath);
            if( !subDir.isEmpty() ){
                //检查分机配置文件是否存在
                QFileInfoList subFilesInfo = subDir.entryInfoList(QStringList("*.xml"),QDir::Files,QDir::DirsLast);
                if( subFilesInfo.size()>0 ){
                    //先解析总配置文件，获取分机Id，分机名称等
                    QString strPath=filesList.first().absoluteFilePath();

                    QFile file(strPath);
                    if (!file.open(QIODevice::ReadOnly)){
                        qDebug()<<"Open XML Failed,FilePath:"<<strPath;
                        return false;
                    }

                    QDomDocument doc;
                    QString error = "";
                    int row = 0, column = 0;
                    if(!doc.setContent(&file, false, &error, &row, &column)){
                        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
                        file.close();
                        return false;
                    }
                    file.close();

                    QDomElement root = doc.documentElement();
                    if( "Radar"==root.tagName() )
                    {
                        SubCfgInfo subCfgInfo;
                        QDomNode node = root.firstChild();
                        while(!node.isNull()) {
                           QDomElement element = node.toElement(); // try to convert the node to an element.
                           if(!element.isNull()) {
                               if( "Subject" == element.tagName() ){
                                   subCfgInfo.strName=element.attribute("displayName");
                                   subCfgInfo.strSubId=element.attribute("subId").trimmed();
                                   subCfgInfo.strBaseName=element.attribute("cfg").trimmed();

                                   //补充分机配置文件路径信息
                                   foreach (QFileInfo fi, subFilesInfo) {
                                       if(fi.baseName()==subCfgInfo.strBaseName){
                                           subCfgInfo.strAbsFilePath=fi.absoluteFilePath();
                                           cfgInfoList.push_back(subCfgInfo);
                                           break;
                                       }
                                   }
                              }
                           }
                           node = node.nextSibling();
                        }
                    }
                    return true;
                }
            }
            else{
                qDebug()<<QStringLiteral("当前设备没有分机配置文件，请检查核实！");
            }
        }
    }
    return false;
}

bool PluginMacroConfig::questionMsgBox(QString strInfo)
{
    int nRet = QMessageBox::question(this,QStringLiteral("提示"),
                                     strInfo,
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes);
    if( QMessageBox::Yes == nRet )
        return true;

    return false;
}

void PluginMacroConfig::onCBCfgFileName(int nIndex)
{
    QString strMacroFile=m_pCBCfgFileName->itemData(nIndex).toString();

    QFile file(strMacroFile);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Open XML Failed,FilePath:"<<strMacroFile;
        return;
    }

    QDomDocument doc;
    QString error = "";
    int row = 0, column = 0;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return;
    }
    file.close();

    QMap<QString,bool>subCfgCheckedInfo;
    QDomElement root = doc.documentElement();
    if( QStringLiteral("设备配置参数")==root.tagName() ){
        QDomElement devInfoNode=root.firstChildElement(QStringLiteral("设备信息"));
        if( !devInfoNode.isNull() ){
            QDomNode devNode = devInfoNode.firstChild();
            while(!devNode.isNull()) {
                QDomElement element = devNode.toElement(); // try to convert the node to an element.
                if(!element.isNull()) {
                    if( QStringLiteral("设备")== element.tagName() ){
                        QString strSubDevId=element.attribute(QStringLiteral("subDevId"));
                        QVariant checkedVar(element.attribute(QStringLiteral("checked")));

                        subCfgCheckedInfo.insert(strSubDevId,checkedVar.toBool());
                    }
                }
               devNode = devNode.nextSibling();
            }
        }
    }

    //更新配置文件管理树勾选状态
    if( m_pCfgManageRootItem ){
        int nChild = m_pCfgManageRootItem->childCount();
        for( int i=0;i<nChild;i++ ){
            QTreeWidgetItem* pChild=m_pCfgManageRootItem->child(i);
            if( pChild ){
                QString strDevId=pChild->text(1).trimmed();
                auto it = subCfgCheckedInfo.find(strDevId);
                if( it!=subCfgCheckedInfo.end() ){
                    if( it.value() )
                        pChild->setCheckState(0,Qt::Checked);
                    else
                        pChild->setCheckState(0,Qt::Unchecked);
                }
            }
        }
    }
}

void PluginMacroConfig::onBtnStartCfg()
{

}

void PluginMacroConfig::onBtnSaveCfg()
{
    QString strInfo=QString("Do you save the configuration?");
    if( questionMsgBox(strInfo) ){
        QString strPath=m_pCBCfgFileName->currentData().toString();
        QFile file(strPath);
        if (!file.open(QFile::WriteOnly | QFile::Text)) { // 只写模式打开文件
            qDebug() << QString("Cannot write file %1(%2).").arg(strPath).arg(file.errorString());
            return;
        }

        QXmlStreamWriter xmlWriter(&file);
        //xmlWriter.setCodec("GBK");//XML 编码
        xmlWriter.writeStartDocument("1.0", true);// 开始文档（XML 声明）
        xmlWriter.setAutoFormatting(true);//自动格式化

        xmlWriter.writeStartElement(QStringLiteral("设备配置参数"));//开始根元素 <设备配置参数>
        xmlWriter.writeStartElement(QStringLiteral("设备信息"));//开始根元素 <设备信息>

        int nChild = m_pCfgManageRootItem->childCount();
        for( int i=0;i<nChild;i++ ){
            QTreeWidgetItem* pChild=m_pCfgManageRootItem->child(i);
            if( pChild ){
                xmlWriter.writeStartElement(QStringLiteral("设备"));
                xmlWriter.writeAttribute("subDevId",pChild->text(1).trimmed());
                if( Qt::Checked==pChild->checkState(0) ){
                    xmlWriter.writeAttribute("checked","true");
                }
                else{
                    xmlWriter.writeAttribute("checked","false");
                }
                xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeEndElement();  // 结束设备信息元素 </设备信息>

        //设备参数
        for( int i=0;i<nChild;i++ ){
            QTreeWidgetItem* pChild=m_pCfgManageRootItem->child(i);
            if( pChild && Qt::Checked==pChild->checkState(0) ){
                QString strName=pChild->text(0).trimmed();
                QString strDevId=pChild->text(1).trimmed();
                xmlWriter.writeStartElement(QStringLiteral("设备"));
                xmlWriter.writeAttribute(QStringLiteral("subDevId"),strDevId);
                int nAllTab=m_pCfgDispalyTab->count();
                for( int k=0;k<nAllTab;k++ ){
                    if( strName==m_pCfgDispalyTab->tabText(k).trimmed() ) {
                        SubConfigDispalyTree*pSubCfgTree=qobject_cast<SubConfigDispalyTree*>(m_pCfgDispalyTab->widget(k));
                        pSubCfgTree->exportSubCofig(&xmlWriter);
                        break;
                    }
                }
                xmlWriter.writeEndElement();
            }
        }

        xmlWriter.writeEndDocument();  //结束根元素</设备配置参数>
        file.close();  // 关闭文件
    }
}

void PluginMacroConfig::onBtnGetCfg()
{

}

void PluginMacroConfig::onBtnAddCfg()
{
    FileNameDlg fileNameDlg;
    if( QDialog::Accepted==fileNameDlg.exec()){
        QString strBaseName=fileNameDlg.getFileName();
        int nCBItems = m_pCBCfgFileName->count();
        for( int i=0;i<nCBItems;i++ ){
            if( strBaseName.trimmed()==m_pCBCfgFileName->itemText(i) ){
                //QString strInfo=QString("该文件已存在，是否覆盖?");
                QString strInfo=QString("The file already exists. Does it cover?");
                if( questionMsgBox(strInfo) )
                    break;
                else
                    return;
            }
        }

        QString strDirPath=QDir::currentPath()+"/config/macro_config_files/";
        QString strAbsPath=strDirPath+strBaseName.trimmed()+".xml";
        QFile file(strAbsPath);
        if (!file.open(QFile::WriteOnly | QFile::Text)) { // 只写模式打开文件
            qDebug() << QString("Cannot write file %1(%2).").arg(strAbsPath).arg(file.errorString());
            return;
        }

        QXmlStreamWriter xmlWriter(&file);
        //xmlWriter.setCodec("GB2312");//XML 编码
        xmlWriter.writeStartDocument("1.0", true);// 开始文档（XML 声明）
        xmlWriter.setAutoFormatting(true);//自动格式化

        xmlWriter.writeStartElement(QStringLiteral("设备配置参数"));//开始根元素 <设备配置参数>
        xmlWriter.writeStartElement(QStringLiteral("设备信息"));//开始根元素 <设备信息>

        int nChild = m_pCfgManageRootItem->childCount();
        for( int i=0;i<nChild;i++ ){
            QTreeWidgetItem* pChild=m_pCfgManageRootItem->child(i);
            if( pChild ){
                xmlWriter.writeStartElement(QStringLiteral("设备"));
                xmlWriter.writeAttribute("devId",pChild->text(1).trimmed());
                if( Qt::Checked==pChild->checkState(0) ){
                    xmlWriter.writeAttribute("checked","true");
                }
                else{
                    xmlWriter.writeAttribute("checked","false");
                }
                xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeEndElement();  // 结束设备信息元素 </设备信息>

        //设备参数
        for( int i=0;i<nChild;i++ ){
            QTreeWidgetItem* pChild=m_pCfgManageRootItem->child(i);
            if( pChild && Qt::Checked==pChild->checkState(0) ){
                QString strName=pChild->text(0).trimmed();
                QString strDevId=pChild->text(1).trimmed();
                xmlWriter.writeStartElement(QStringLiteral("设备"));
                xmlWriter.writeAttribute(QStringLiteral("subDevId"),strDevId);
                int nAllTab=m_pCfgDispalyTab->count();
                for( int k=0;k<nAllTab;k++ ){
                    if( strName==m_pCfgDispalyTab->tabText(k).trimmed() ) {
                        SubConfigDispalyTree*pSubCfgTree=qobject_cast<SubConfigDispalyTree*>(m_pCfgDispalyTab->widget(k));
                        pSubCfgTree->exportSubCofig(&xmlWriter);
                        break;
                    }
                }
                xmlWriter.writeEndElement();
            }
        }

        xmlWriter.writeEndDocument();  //结束根元素</设备配置参数>
        file.close();  // 关闭文件

        m_pCBCfgFileName->addItem(strBaseName,QVariant(strAbsPath));
        m_pCBCfgFileName->setCurrentIndex(m_pCBCfgFileName->count()-1);
    }
}

void PluginMacroConfig::onBtnDeleteCfg()
{
    QString strPath=m_pCBCfgFileName->currentData().toString();
    QFileInfo fileInfo(strPath);
    if( fileInfo.exists() ){
        //QString strInfo=QString("是否删除文件[%1]?").arg(fileInfo.fileName());
        QString strInfo=QString("Whether to delete file[%1]?").arg(fileInfo.fileName());
        if( questionMsgBox(strInfo) ){
            if(QFile::remove(strPath)){
                m_pCBCfgFileName->removeItem(m_pCBCfgFileName->currentIndex());
            }
        }
    }
}

void PluginMacroConfig::onBtnJobRelationCfg()
{

}

void PluginMacroConfig::onItemChanged(QTreeWidgetItem *pItem, int column)
{
    if(Qt::PartiallyChecked!=pItem->checkState(column))
        setChildCheckState(pItem,pItem->checkState(column));

    if(Qt::PartiallyChecked==pItem->checkState(column)){
        if( pItem && pItem->parent() ){
            pItem->parent()->setCheckState(column,Qt::PartiallyChecked);
        }
    }
}

void PluginMacroConfig::onItemClicked(QTreeWidgetItem *pItem, int column)
{
    Q_UNUSED(column);
    QString strName=pItem->text(0).trimmed();
    int nAllTab=m_pCfgDispalyTab->count();
    for( int i=0;i<nAllTab;i++ ){
        if( strName==m_pCfgDispalyTab->tabText(i).trimmed() ) {
            m_pCfgDispalyTab->setCurrentIndex(i);
            break;
        }
    }
}

void PluginMacroConfig::onCurrentTabChanged(int nIndex)
{
    QString strName=m_pCfgDispalyTab->tabText(nIndex).trimmed();
    int nChild = m_pCfgManageRootItem->childCount();
    for( int i=0;i<nChild;i++ ){
        QTreeWidgetItem* pChild=m_pCfgManageRootItem->child(i);
        if( pChild ){
            if( strName==pChild->text(0).trimmed() )
                pChild->setSelected(true);
            else
                pChild->setSelected(false);
        }
    }
}

void PluginMacroConfig::setChildCheckState(QTreeWidgetItem* pItem, Qt::CheckState cs)
{
    if( pItem ){
        for (int i=0;i<pItem->childCount();i++)
        {
            QTreeWidgetItem* pChildItem=pItem->child(i);
            if(pChildItem->checkState(0)!=cs){
                pChildItem->setCheckState(0, cs);
            }
        }
        setParentCheckState(pItem->parent());
    }
}

void PluginMacroConfig::setParentCheckState(QTreeWidgetItem* pItem)
{
    if( pItem ){
        int nSelectedCount=0;
        int nChildCount = pItem->childCount();
        for (int i=0;i<nChildCount;i++){
            QTreeWidgetItem* pChildItem= pItem->child(i);
            if(pChildItem->checkState(0)==Qt::Checked){
                nSelectedCount++;
            }
        }

        if(nSelectedCount == 0) {
            pItem->setCheckState(0,Qt::Unchecked);
        }
        else if (nSelectedCount == nChildCount) {
            pItem->setCheckState(0,Qt::Checked);
        }
        else {
            pItem->setCheckState(0,Qt::PartiallyChecked);
        }
    }
}
