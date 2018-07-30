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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new QStandardItemModel(parent);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::readSwStateFromFile(QString filePath)
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

void MainWindow::InsertSwStateInfo(QString strIP, QString procName, QString stamp, QString strInfo)
{
    if((procName == "96A.exe") || (procName == "04步.exe"))
        return;

    QMap<QString, PcInfo*>::const_iterator i = pcMaps.find(strIP);
    PcInfo* pTmpPC = NULL;
    if(i == pcMaps.end())
    {
        pTmpPC = new PcInfo();
        pTmpPC->pcIP = strIP;
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
    pswInfo->swStates.append(tmpSWS);
}

QStandardItemModel* MainWindow::createModel(QString filterWord)
{
    //QStandardItemModel* model =
    //        new QStandardItemModel(parent);//(pcMaps.size(), 3, parent);
    m_model->clear();

    QMapIterator<QString, PcInfo*> i(pcMaps);
    int pcPos = 0;
    while (i.hasNext()) {
        i.next();
        QStandardItem* item = new QStandardItem(i.key());
        PcInfo* tmpPC = i.value();
        if(i.key().count(filterWord) > 0)
        {
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
            pcPos++;
        }
    }

    m_model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
    m_model->setHorizontalHeaderItem(1, new QStandardItem("Time"));
    m_model->setHorizontalHeaderItem(2, new QStandardItem("Info"));

    return m_model;
}

void MainWindow::on_openBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open File"), "./", tr("Log Files (*.txt)"));
    ui->filePathEdit->setText(fileName);
}

void MainWindow::on_readBtn_clicked()
{
    statusBar()->showMessage(tr("读取中..."));

    readSwStateFromFile(ui->filePathEdit->text());
    QStandardItemModel* model = createModel("");

    ui->treeView->setModel(model);

    ui->treeView->setColumnWidth(0,200);
    ui->treeView->setColumnWidth(1,200);
    if(ui->expandBtn->text() == "合并")
    {
        ui->treeView->expandAll();
    }

    statusBar()->showMessage(tr("读取完毕"));
}

void MainWindow::on_expandBtn_clicked()
{
    if(ui->expandBtn->text() == "展开")
    {
        ui->treeView->expandAll();
        ui->expandBtn->setText("合并");
    }
    else
    {
        ui->treeView->collapseAll();
        ui->expandBtn->setText("展开");
    }
}

void MainWindow::on_filterBtn_clicked()
{
    statusBar()->showMessage(tr("读取中..."));
    QString ipStr = ui->ipEdit->text();
    if(!ipStr.isEmpty())
    {
        QStandardItemModel* model = createModel(ipStr);
        ui->treeView->setModel(model);

        ui->treeView->setColumnWidth(0,200);
        ui->treeView->setColumnWidth(1,200);
        if(ui->expandBtn->text() == "合并")
        {
            ui->treeView->expandAll();
        }
    }
    statusBar()->showMessage(tr("读取完毕"));
}

void MainWindow::on_recoverBtn_clicked()
{
    on_readBtn_clicked();
}
