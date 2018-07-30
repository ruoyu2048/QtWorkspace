#include "TreeView.h"
#include <QKeyEvent>
#include <QHeaderView>
#include "MacroDef.h"
#include "GlobalDef.h"


TreeView::TreeView(QWidget *parent)
    :QTreeView(parent)
{
    m_bSelAll = false;
    m_bExpandAll = true;
    SetTreeViewStyle();
    InitTreeView();
}

TreeView::~TreeView()
{

}

void TreeView::InitTreeView()
{
    TreeItem* pRoot = globalDS.GetRoot();
    m_pTreeModel = new TreeModel(pRoot,this);

    QStringList headerList;
    headerList<<"名称"<<"位置编码"<<"状态"<<"更新时间"<<"MAC"<<"模拟指挥网IP"<<"仿真对抗网IP"<<"中转主机IP"<<"本机类型"<<"本机类别";
    m_pTreeModel->setHeaderList(headerList);
    connect(m_pTreeModel,SIGNAL(SendMsg(int,QHostAddress,QString,QString)),this,SIGNAL(SendMsg(int,QHostAddress,QString,QString)));
    connect(m_pTreeModel,SIGNAL(AddToMsgQueue(int,QString,QString,QString,QString)),this,SIGNAL(AddToMsgQueue(int,QString,QString,QString,QString))); 
    connect(this,SIGNAL(UpdateHostInfo(QString,int,QString)),m_pTreeModel,SLOT(UpdateHostInfo(QString,int,QString)));

    m_pSFPModel = new SortFilterProxyModel(this);
    m_pSFPModel->setSourceModel(m_pTreeModel);

    //this->setModel(m_pTreeModel);
    this->setModel(m_pSFPModel);

    QVector<int>columnWidth;
    columnWidth<<320<<80<<48<<168<<136<<128<<128<<128<<80;

    for(int i=0;i<columnWidth.size();++i)
    {
        this->setColumnWidth(i,columnWidth[i]);
    }
    this->expandAll();

    m_pFilterDlg = new FilterDlg(this);
    connect(m_pFilterDlg,SIGNAL(FilterChanged(QRegExp)),this,SLOT(Filter(QRegExp)));
}

void TreeView::SetTreeViewStyle()
{
    this->setStyleSheet("QTreeView{background-color:rgb(35,35,35)} QTreeView::item{height:25px}");
    //this->setStyleSheet("QTreeView{background-color:rgb(192,192,192)} QTreeView::item{height:25px}");
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setUniformRowHeights(true);
    QFont defaultFont = this->font();
    defaultFont.setPointSize(ITEM_FONT_SIZE);
    this->header()->setFont(defaultFont);
    this->setFont(defaultFont);
    this->setFocusPolicy(Qt::StrongFocus);
}

void TreeView::ExpandAll()
{
    if(m_bExpandAll)
        this->collapseAll();
    else
        this->expandAll();
    m_bExpandAll = !m_bExpandAll;

    emit ExpandAllFlag(m_bExpandAll);
}

void TreeView::selectAll()
{
    m_bSelAll = !m_bSelAll;
    m_pTreeModel->SelectAll(m_bSelAll);

    emit SelectAllFlag(m_bSelAll);
}

void TreeView::RefrushView()
{
    m_pTreeModel->RefrushModel();
    if(m_bSelAll)
    {
        m_bSelAll = false;
        m_pTreeModel->ResetAllItemSelectState();

        emit SelectAllFlag(m_bSelAll);
    }
}

void TreeView::ShowFilterDlg()
{
    //防止快捷键与按钮调用两个不同的删选对话框
    if(!m_pFilterDlg->isVisible())
    {
        m_pFilterDlg->ResetEditText();
        m_pFilterDlg->show();
        if(m_bSelAll)
        {
            m_bSelAll = false;
            m_pTreeModel->ResetAllItemSelectState();

            emit SelectAllFlag(m_bSelAll);
        }
    }
}

void TreeView::PowerOn()
{
    m_pTreeModel->PowerOn();
}

void TreeView::PowerOff()
{
    m_pTreeModel->PowerOff();
}

void TreeView::Restart()
{
    m_pTreeModel->Restart();
}

void TreeView::Refresh()
{
    m_pTreeModel->Refresh();
}

//public slots:
void TreeView::Filter(QRegExp regExp)
{
    m_pSFPModel->setFilterRegExp(regExp);

    this->expandAll();
    m_bExpandAll = true;

    emit ExpandAllFlag(m_bExpandAll);
}

