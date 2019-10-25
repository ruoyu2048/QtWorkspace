#include "RunTest.h"
#include "ui_RunTest.h"

#include "SQLite.h"

RunTest::RunTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunTest),
    m_TestType(TestType::AutoTest)
{
    ui->setupUi(this);
    initMainWindow();
}

RunTest::~RunTest()
{
    delete ui;
}

void RunTest::resetTestType(TestType testType)
{
    if(m_TestType!=testType){
        m_TestType=testType;
        if( TestType::AutoTest==testType ){
            ui->laTestType->setText(QString::fromLocal8Bit("自动测试"));
        }
        else if(TestType::SequenceTest==testType){
            ui->laTestType->setText(QString::fromLocal8Bit("序列测试"));
        }
        updateProjectList(testType);
    }
}

void RunTest::initMainWindow()
{
    this->setContentsMargins(5,0,5,0);
    this->setLayout(ui->gridLayout);
    this->setMaximumSize(600,320);

    initSqlite();

    updateProjectList(m_TestType);
}

void RunTest::initSqlite()
{
    m_pSqlite = new SQLite("D:\\WireTest\\bklt.db","","","");
}

void RunTest::updateProjectList(TestType testType)
{
    if( TestType::AutoTest==testType ){
        m_pSqlite->dataQuery("SELECT name FROM 'tb_device'");
    }
    else if(TestType::SequenceTest==testType){
        m_pSqlite->dataQuery("SELECT name FROM 'tb_sequence'");
    }

    QSqlQuery* pQuery=m_pSqlite->getSqlQuery();
    QStringList nameList;
    nameList<<QString::fromLocal8Bit("------------选择工程------------");
    while(pQuery->next())
    {
        QStringList rowValues;
        int nCol = pQuery->record().count();
        if( nCol>=1 ){
            nameList<<pQuery->value(0).toString();
        }
    }
    ui->cbProjectList->clear();
    ui->cbProjectList->addItems(nameList);
}

void RunTest::on_btnStartTest_clicked()
{
    updateProjectList(TestType::AutoTest);
}

void RunTest::on_btnRefreshList_clicked()
{
    updateProjectList(m_TestType);
}
