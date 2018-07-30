#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QSplitter>
#include <QListView>
#include <QTableView>
#include <QTreeView>
#include <QColumnView>
#include <QFileDialog>
#include <qdebug.h>
#include <QMessageBox>
#include "DB.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nameFont = QFont("宋体", 11, QFont::Bold);
    //软件状态模型设置
    m_model = new QStandardItemModel(this);
    m_proxy = new FilterModel(this);
    m_proxy->setSourceModel(m_model);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setStyleSheet("QTreeView::item{height:20}");
    ui->treeView->setUniformRowHeights(true);
    //节点状态设置
    m_nodeModel = new QStandardItemModel(this);
    m_nodeProxy = new NodeFilterModel(this);
    m_nodeProxy->setSourceModel(m_nodeModel);
    ui->nodeTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->nodeTreeView->setAlternatingRowColors(true);
    ui->nodeTreeView->setStyleSheet("QTreeView::item{height:20}");
    ui->nodeTreeView->setUniformRowHeights(true);

    //ui->filterBtn->setEnabled(false);
    //ui->recoverBtn->setEnabled(false);
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    m_strLogPath = CDB::GetParameterFromConfig("APP", "LOGPATH");
    ui->filePathEdit->setText(m_strLogPath);

    QImage tmpImage;
    tmpImage.load(tr(":/images/green.png"));
    tmpImage.save(tr(":/images/green.png"));
    tmpImage.load(tr(":/images/red.png"));
    tmpImage.save(tr(":/images/red.png"));
    tmpImage.load(tr(":/images/yellow.png"));
    tmpImage.save(tr(":/images/yellow.png"));

    if(!CDB::ConnectDatabase())
    {
        qDebug() << "连接数据库失败";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::readSwStateFromFile(QString filePath, QString filterWord)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    //清空pcInfo内容，重新读取
    QMapIterator<QString, PcInfo*> iDel(pcMaps);
    while (iDel.hasNext()) {
        iDel.next();
        PcInfo* pTmpPC = iDel.value();
        delete pTmpPC;
    }
    pcMaps.clear();

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForLocale());
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.count(filterWord) == 0)
        {
            continue;
        }

        QStringList strList1 = line.split('\t', QString::SkipEmptyParts);
        if(strList1.size() < 3)
            continue;
        QString tmpStamp = strList1.front();
        strList1.pop_front();

        QString strIP = strList1.front();
        strList1.pop_front();

        QString strTmp = strList1.join(' ');
        QStringList strList2 = strTmp.split(':', QString::SkipEmptyParts);
        if(strList2.size() < 3)
            continue;
        strList2.pop_front();
        QString procName = strList2.front();
        strList2.pop_front();
        QString strInfo = strList2.join(' ');

        InsertSwStateInfo(strIP, procName, tmpStamp, strInfo);
    }
    return true;
}

bool MainWindow::readNodeStateFromFile(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    //清空m_nodeStates内容，重新读取
    for(int i = 0; i < m_nodeStates.size(); i++)
    {
        delete m_nodeStates.at(i);
    }
    m_nodeStates.clear();

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForLocale());
    while (!in.atEnd()) {
        QString line = in.readLine();

        QStringList strList1 = line.split('\t', QString::SkipEmptyParts);
        if(strList1.size() < 4)
            continue;
        QString tmpStamp = strList1.front();
        strList1.pop_front();

        QString strIP = strList1.front();
        strList1.pop_front();

        QString strIf = strList1.front();
        strList1.pop_front();

        QString strState = strList1.front();

        InsertNodeStateInfo(strIP, strIf, tmpStamp, strState);
    }
    return true;
}

void MainWindow::InsertSwStateInfo(QString strIP, QString procName, QString stamp, QString strInfo)
{
    //if((procName == "96A.exe") || (procName == "04步.exe"))
    //    return;

    QMap<QString, PcInfo*>::const_iterator i = pcMaps.find(strIP);
    PcInfo* pTmpPC = NULL;
    if(i == pcMaps.end())
    {
        pTmpPC = new PcInfo();
        pTmpPC->pcIP = strIP;
        pTmpPC->pcName = CDB::GetNameByIP(strIP);
        pcMaps.insert(strIP, pTmpPC);
    }
    else
    {
        pTmpPC = i.value();
    }

    //pTmpPC->swInfos.indexOf()
    int swPos = 0;
    SoftWareInfo* pswInfo = NULL;
    for(; swPos < pTmpPC->swInfos.size(); swPos++)
    {
        if(pTmpPC->swInfos.at(swPos).swName == procName)
        {
            pswInfo = &(pTmpPC->swInfos[swPos]);
            break;
        }
    }
    if(swPos >= pTmpPC->swInfos.size())
    {
        SoftWareInfo tmpSWI;
        tmpSWI.swName = procName;
        pTmpPC->swInfos.append(tmpSWI);
        pswInfo = &(pTmpPC->swInfos[swPos]);
    }
    SoftWareState tmpSWS;
    tmpSWS.stateTime = stamp;
    tmpSWS.swStateInfo = strInfo;
    int swsCount = pswInfo->swStates.size();
    if(swsCount > 0)
    {
        SoftWareState tmpLast = pswInfo->swStates.at(swsCount - 1);
        if(tmpLast.swStateInfo != strInfo)
            pswInfo->swStates.append(tmpSWS);
    }
    else
        pswInfo->swStates.append(tmpSWS);
}

void MainWindow::InsertNodeStateInfo(QString strIP, QString strIf,
                                     QString timeStamp, QString strNodeState)
{
    PcNodeInfos* matchNode = NULL;
    for(int i = 0; i < m_nodeStates.size(); i++)
    {
        PcNodeInfos* tmpNode = m_nodeStates.at(i);
        if((tmpNode->m_strPcIP == strIP) && (tmpNode->m_strIf == strIf))
        {
            matchNode = tmpNode;
            break;
        }
    }
    if(matchNode == NULL)
    {
        matchNode = new PcNodeInfos();
        matchNode->m_strPcIP = strIP;
        matchNode->m_strIf = strIf;
        if(strIP != strIf)
        {
            matchNode->m_strPcName = CDB::GetNameByIP(strIP, strIf);
        }
        else
        {
            matchNode->m_strPcName = CDB::GetNameByIP(strIP);
        }

        m_nodeStates.append(matchNode);
    }
    NodeStateInfo tmpNodeState;
    tmpNodeState.m_strRecodeTime = timeStamp;
    tmpNodeState.m_strState = strNodeState;
    if(matchNode->m_nodeStates.empty())
    {
        matchNode->m_nodeStates.append(tmpNodeState);
    }
    else
    {
        //过滤掉与上次相同的状态
        if(strNodeState != matchNode->m_nodeStates.last().m_strState)
        {
            matchNode->m_nodeStates.append(tmpNodeState);
        }
    }
}

QStandardItemModel* MainWindow::createModel()
{
    /*
    m_model->clear();

    QMapIterator<QString, PcInfo*> i(pcMaps);
    int pcPos = 0;
    while (i.hasNext()) {
        i.next();
        PcInfo* tmpPC = i.value();
        QStandardItem* item = NULL;
        QStandardItem* pcNameItem = NULL;
        pcNameItem = new QStandardItem(tmpPC->pcName);
        pcNameItem->setFont(nameFont);
        item = new QStandardItem(i.key());
        item->setFont(nameFont);

        for(int iSw=0; iSw < tmpPC->swInfos.size(); iSw++)
        {
            SoftWareInfo* tmpSWS = &tmpPC->swInfos[iSw];
            QStandardItem* switem = new QStandardItem(tmpSWS->swName);
            for(int iSi = 0; iSi < tmpSWS->swStates.size(); iSi++)
            {
                QList<QStandardItem *> stateRow;
                stateRow.append(new QStandardItem(QString::number(iSi+1)));
                stateRow.append(new QStandardItem(tmpSWS->swStates[iSi].stateTime));
                stateRow.append(new QStandardItem(tmpSWS->swStates[iSi].swStateInfo));
                switem->appendRow(stateRow);
            }
            item->appendRow(switem);
        }
        m_model->setItem(pcPos, 0, item);
        m_model->setItem(pcPos, 1, pcNameItem);
        pcPos++;
    }

    m_model->setHorizontalHeaderItem(0, new QStandardItem("IP地址"));
    m_model->setHorizontalHeaderItem(1, new QStandardItem("名称/时间"));
    m_model->setHorizontalHeaderItem(2, new QStandardItem("状态"));
    */
    QStandardItemModel *newModel = new QStandardItemModel(this);
    QMapIterator<QString, PcInfo*> i(pcMaps);
    int pcPos = 0;
    while (i.hasNext()) {
        i.next();
        PcInfo* tmpPC = i.value();
        QStandardItem* item = NULL;
        QStandardItem* pcNameItem = NULL;
        pcNameItem = new QStandardItem(tmpPC->pcName);
        pcNameItem->setFont(nameFont);
        item = new QStandardItem(i.key());
        item->setFont(nameFont);

        for(int iSw=0; iSw < tmpPC->swInfos.size(); iSw++)
        {
            SoftWareInfo* tmpSWS = &tmpPC->swInfos[iSw];
            QStandardItem* switem = new QStandardItem(tmpSWS->swName);
            for(int iSi = 0; iSi < tmpSWS->swStates.size(); iSi++)
            {
                QList<QStandardItem *> stateRow;
                stateRow.append(new QStandardItem(QString::number(iSi+1)));
                stateRow.append(new QStandardItem(tmpSWS->swStates[iSi].stateTime));
                stateRow.append(new QStandardItem(tmpSWS->swStates[iSi].swStateInfo));
                switem->appendRow(stateRow);
            }
            item->appendRow(switem);
        }
        newModel->setItem(pcPos, 0, item);
        newModel->setItem(pcPos, 1, pcNameItem);
        pcPos++;
    }

    newModel->setHorizontalHeaderItem(0, new QStandardItem("IP地址"));
    newModel->setHorizontalHeaderItem(1, new QStandardItem("名称/时间"));
    newModel->setHorizontalHeaderItem(2, new QStandardItem("状态"));

    m_proxy->setSourceModel(newModel);
    m_model->clear();
    delete m_model;
    m_model = newModel;

    return m_model;
}

QStandardItemModel* MainWindow::createNodeModel()
{   
    /*
    m_nodeModel->clear();
    //QStandardItemModel *newModel = new QStandardItemModel(this);
    for(int pcPos = 0; pcPos < m_nodeStates.size(); pcPos++)
    {
        PcNodeInfos* tmpPC = m_nodeStates.at(pcPos);
        QStandardItem* item = NULL;
        QStandardItem* pcNameItem = NULL;
        pcNameItem = new QStandardItem(tmpPC->m_strPcName);
        pcNameItem->setFont(nameFont);
        item = new QStandardItem(tmpPC->m_strPcIP);
        item->setFont(nameFont);

        for(int iState=0; iState < tmpPC->m_nodeStates.size(); iState++)
        {
            NodeStateInfo tmpNodeState = tmpPC->m_nodeStates.at(iState);
            QList<QStandardItem *> stateRow;
            stateRow.append(new QStandardItem(QString::number(iState+1)));
            stateRow.append(new QStandardItem(tmpNodeState.m_strRecodeTime));
            stateRow.append(new QStandardItem(tmpNodeState.m_strState));
            item->appendRow(stateRow);
        }
        m_nodeModel->setItem(pcPos, 0, item);
        m_nodeModel->setItem(pcPos, 1, pcNameItem);
    }

    m_nodeModel->setHorizontalHeaderItem(0, new QStandardItem("IP地址"));
    m_nodeModel->setHorizontalHeaderItem(1, new QStandardItem("名称/时间"));
    m_nodeModel->setHorizontalHeaderItem(2, new QStandardItem("状态"));
    */
    QStandardItemModel *newModel = new QStandardItemModel(this);
    for(int pcPos = 0; pcPos < m_nodeStates.size(); pcPos++)
    {
        PcNodeInfos* tmpPC = m_nodeStates.at(pcPos);
        QStandardItem* item = NULL;
        QStandardItem* pcNameItem = NULL;
        pcNameItem = new QStandardItem(tmpPC->m_strPcName);
        pcNameItem->setFont(nameFont);
        item = new QStandardItem(tmpPC->m_strPcIP);
        item->setFont(nameFont);

        for(int iState=0; iState < tmpPC->m_nodeStates.size(); iState++)
        {
            NodeStateInfo tmpNodeState = tmpPC->m_nodeStates.at(iState);
            QList<QStandardItem *> stateRow;
            stateRow.append(new QStandardItem(QString::number(iState+1)));
            stateRow.append(new QStandardItem(tmpNodeState.m_strRecodeTime));
            //stateRow.append(new QStandardItem(tmpNodeState.m_strState));
            QStandardItem* tmpItem = new QStandardItem(tmpNodeState.m_strState);
            if(tmpNodeState.m_strState == tr("开机"))
            {
                tmpItem->setIcon(QIcon(":/images/green.png"));
            }
            else if(tmpNodeState.m_strState == tr("关机"))
            {
                tmpItem->setIcon(QIcon(":/images/red.png"));
            }
            else
            {
                tmpItem->setIcon(QIcon(":/images/yellow.png"));
            }
            stateRow.append(tmpItem);

            item->appendRow(stateRow);
        }
        newModel->setItem(pcPos, 0, item);
        newModel->setItem(pcPos, 1, pcNameItem);
    }
    newModel->setHorizontalHeaderItem(0, new QStandardItem("IP地址"));
    newModel->setHorizontalHeaderItem(1, new QStandardItem("名称/时间"));
    newModel->setHorizontalHeaderItem(2, new QStandardItem("状态"));
    m_nodeProxy->setSourceModel(newModel);
    m_nodeModel->clear();
    delete m_nodeModel;
    m_nodeModel = newModel;

    return m_nodeModel;
}

void MainWindow::on_openBtn_clicked()
{
    /*
     * QFileDialog::Directory
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open File"), "./", tr("Log Files (*.txt)"));
     */
    /*
    QFileDialog dialog(this);
    QString fileName;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(m_strLogPath);
    if (dialog.exec())
          fileName = dialog.directory().absolutePath();
    ui->filePathEdit->setText(fileName);
    m_strLogPath = fileName;
    CDB::SetParameterFromConfig("APP", "LOGPATH", m_strLogPath);
    */
    m_strLogPath = QFileDialog::getExistingDirectory(this, tr("打开目录"),
                                                      m_strLogPath,
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    CDB::SetParameterFromConfig("APP", "LOGPATH", m_strLogPath);
}

void MainWindow::on_readBtn_clicked()
{
    statusBar()->showMessage(tr("读取中..."));
    QString strLogFile;

    ui->readBtn->setEnabled(false);
    ui->expandBtn->setEnabled(false);
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
        strLogFile = m_strLogPath + tr("/软件异常状态信息.txt");
        readSwStateFromFile(strLogFile, "");

        createModel();
        m_proxy->SetBeginTime(ui->beginDateTimeEdit->dateTime());
        m_proxy->SetEndTime(ui->endDateTimeEdit->dateTime());

        ui->treeView->setModel(m_proxy);

        ui->treeView->setColumnWidth(0,200);
        ui->treeView->setColumnWidth(1,300);
        if(ui->expandBtn->text() == "合并")
        {
            ui->treeView->expandAll();
        }
        break;
    case 1:
        strLogFile = m_strLogPath + tr("/节点状态变化信息.txt");
        readNodeStateFromFile(strLogFile);//ui->filePathEdit->text()

        createNodeModel();
        m_nodeProxy->SetBeginTime(ui->beginDateTimeEdit->dateTime());
        m_nodeProxy->SetEndTime(ui->endDateTimeEdit->dateTime());

        ui->nodeTreeView->setModel(m_nodeProxy);

        ui->nodeTreeView->setColumnWidth(0,200);
        ui->nodeTreeView->setColumnWidth(1,300);
        if(ui->expandBtn->text() == "合并")
        {
            ui->nodeTreeView->expandAll();
        }
        break;
    default:
        break;
    }

    ui->readBtn->setEnabled(true);
    ui->expandBtn->setEnabled(true);
    statusBar()->showMessage(tr("读取完毕"));
}

void MainWindow::on_expandBtn_clicked()
{
    if(ui->expandBtn->text() == "展开")
    {
        switch(ui->tabWidget->currentIndex())
        {
        case 0:
            ui->treeView->expandAll();
            break;
        case 1:
            ui->nodeTreeView->expandAll();
            break;
        default:
            break;
        }
        ui->expandBtn->setText("合并");
    }
    else
    {
        switch(ui->tabWidget->currentIndex())
        {
        case 0:
            ui->treeView->collapseAll();
            break;
        case 1:
            ui->nodeTreeView->collapseAll();
            break;
        default:
            break;
        }
        ui->expandBtn->setText("展开");
    }
}

/*
void MainWindow::on_filterBtn_clicked()
{
    statusBar()->showMessage(tr("读取中..."));
    QString ipStr = ui->ipEdit->text();

    QStandardItemModel* model = NULL;
    if(!ipStr.isEmpty())
    {
        switch(ui->tabWidget->currentIndex())
        {
        case 0:
            readSwStateFromFile(ui->filePathEdit->text(), ipStr);
            model = createModel();
            ui->treeView->setModel(model);

            ui->treeView->setColumnWidth(0,200);
            ui->treeView->setColumnWidth(1,300);
            if(ui->expandBtn->text() == "合并")
            {
                ui->treeView->expandAll();
            }
            break;
        case 1:
            readNodeStateFromFile(ui->filePathEdit->text());
            model = createModel();
            ui->treeView->setModel(model);

            ui->treeView->setColumnWidth(0,200);
            ui->treeView->setColumnWidth(1,300);
            if(ui->expandBtn->text() == "合并")
            {
                ui->treeView->expandAll();
            }
            break;
        default:
            break;
        }
    }
    statusBar()->showMessage(tr("读取完毕"));
}

void MainWindow::on_recoverBtn_clicked()
{
    on_readBtn_clicked();
}
*/

void MainWindow::dateFilterChanged()
{
    statusBar()->showMessage(tr("提示:编辑完毕后点击其它区域即可完成过滤"));
}

void MainWindow::on_ipEdit_editingFinished()
{
    QRegExp regExp(ui->ipEdit->text(), Qt::CaseSensitive);
                //filterWidget->text(),
                   //filterWidget->caseSensitivity(),
                   //filterWidget->patternSyntax());
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
        m_proxy->setFilterRegExp(regExp);
        break;
    case 1:
        m_nodeProxy->setFilterRegExp(regExp);
        break;
    default:
        break;
    }
    statusBar()->showMessage(tr("完成关键字过滤"));
}

void MainWindow::on_beginDateTimeEdit_editingFinished()
{
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
        m_proxy->SetBeginTime(ui->beginDateTimeEdit->dateTime());
        break;
    case 1:
        m_nodeProxy->SetBeginTime(ui->beginDateTimeEdit->dateTime());
        break;
    default:
        break;
    }
    statusBar()->showMessage(tr("完成时间过滤"));
}

void MainWindow::on_endDateTimeEdit_editingFinished()
{
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
        m_proxy->SetEndTime(ui->endDateTimeEdit->dateTime());
        break;
    case 1:
        m_nodeProxy->SetEndTime(ui->endDateTimeEdit->dateTime());
        break;
    default:
        break;
    }
    statusBar()->showMessage(tr("完成时间过滤"));
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //
    QRegExp regExp(ui->ipEdit->text(), Qt::CaseSensitive);
    switch(index)
    {
    case 0://软件状态标签页
        m_proxy->setFilterRegExp(regExp);
        m_proxy->SetBeginTime(ui->beginDateTimeEdit->dateTime());
        m_proxy->SetEndTime(ui->endDateTimeEdit->dateTime());

        if(ui->expandBtn->text() == "合并")
        {
            ui->treeView->expandAll();
        }
        else
        {
            ui->treeView->collapseAll();
        }
        break;
    case 1://节点状态标签页
        m_nodeProxy->setFilterRegExp(regExp);
        m_nodeProxy->SetBeginTime(ui->beginDateTimeEdit->dateTime());
        m_nodeProxy->SetEndTime(ui->endDateTimeEdit->dateTime());
        if(ui->expandBtn->text() == "合并")
        {
            ui->nodeTreeView->expandAll();
        }
        else
        {
            ui->nodeTreeView->collapseAll();
        }
        break;
    default:
        break;
    }
}

void MainWindow::on_beginDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    statusBar()->showMessage(tr("提示:编辑完毕后点击其它区域即可完成过滤"));
}

void MainWindow::on_endDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    statusBar()->showMessage(tr("提示:编辑完毕后点击其它区域即可完成过滤"));
}

void MainWindow::on_ipEdit_textEdited(const QString &arg1)
{
    statusBar()->showMessage(tr("提示:编辑完毕后点击其它区域即可完成过滤"));
}

void MainWindow::on_saveBtn_clicked()
{
    QString fileName = tr("%1.txt").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
    QFile outFile(fileName);

    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("导出文件打开失败!"));
        msgBox.exec();
        return;
    }

    QTextStream out(&outFile);

    switch(ui->tabWidget->currentIndex())
    {
    case 0:
        for(int i = 0; i < m_proxy->rowCount(); i++)
        {
            QModelIndex ipIndex = m_proxy->index(i, 0);
            QModelIndex nameIndex = m_proxy->index(i, 1);
            out << ipIndex.data().toString() << "\t"
                         << nameIndex.data().toString() << "\n";
            for(int j = 0; j < m_proxy->rowCount(ipIndex); j++)
            {
                QModelIndex swIndex = m_proxy->index(j, 0, ipIndex);
                for(int m = 0; m < m_proxy->rowCount(swIndex); m++)
                {
                    QModelIndex noIndex = m_proxy->index(m, 0, swIndex);
                    QModelIndex timeIndex = m_proxy->index(m, 1, swIndex);
                    QModelIndex stateIndex = m_proxy->index(m, 2, swIndex);
                    out << noIndex.data().toString() << "\t"
                                 << timeIndex.data().toString() << "\t"
                                     << stateIndex.data().toString() << "\n";
                }
            }
        }
        break;
    case 1:
        for(int i = 0; i < m_nodeProxy->rowCount(); i++)
        {
            QModelIndex ipIndex = m_nodeProxy->index(i, 0);
            QModelIndex nameIndex = m_nodeProxy->index(i, 1);
            out << ipIndex.data().toString() << "\t"
                         << nameIndex.data().toString() << "\n";
            for(int j = 0; j < m_nodeProxy->rowCount(ipIndex); j++)
            {
                QModelIndex noIndex = m_nodeProxy->index(j, 0, ipIndex);
                QModelIndex timeIndex = m_nodeProxy->index(j, 1, ipIndex);
                QModelIndex stateIndex = m_nodeProxy->index(j, 2, ipIndex);
                out << noIndex.data().toString() << "\t"
                    << timeIndex.data().toString() << "\t"
                    << stateIndex.data().toString() << "\n";
            }
        }
        break;
    default:
        break;
    }

    QMessageBox sucMsgBox;
    sucMsgBox.setText(tr("导出文件成功!"));
    sucMsgBox.exec();
}
