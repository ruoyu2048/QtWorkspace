#include "FilterDlg.h"
#include "MacroDef.h"

FilterDlg::FilterDlg(QWidget *parent) :
    QDialog(parent)
{
    InitMainView();
}

void FilterDlg::InitMainView()
{
    setWindowTitle("查找窗口");
    setFixedSize(300,152);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    m_pLableText = new QLabel;
    m_pLableText->setText("查找内容:");
    m_pComboText = new QComboBox;
    m_pComboText->setEditable(true);
    connect(m_pComboText,SIGNAL(currentTextChanged(QString)),this,SLOT(FilterRegExpChanged()));

    m_pGroupBox = new QGroupBox();
    m_pGroupBox->setTitle("查找选项");
    m_pVBLyout = new QVBoxLayout();
    m_pCaseSensitivity = new QCheckBox();
    m_pCaseSensitivity->setText("大小写匹配");
    connect(m_pCaseSensitivity,SIGNAL(clicked(bool)),this,SLOT(FilterRegExpChanged()));
    m_pComboPattern = new QComboBox;
    m_pComboPattern->addItem(tr("通配符"), QRegExp::Wildcard);
    m_pComboPattern->addItem(tr("正则表达式"), QRegExp::RegExp2);
    m_pComboPattern->addItem(tr("全字符匹配"), QRegExp::FixedString);
    m_pComboPattern->setCurrentIndex(1);
    connect(m_pComboPattern,SIGNAL(currentIndexChanged(int)),this,SLOT(FilterRegExpChanged()));

    m_pVBLyout->addWidget(m_pCaseSensitivity,1);
    m_pVBLyout->addWidget(m_pComboPattern,1);
    m_pGroupBox->setLayout(m_pVBLyout);

    m_pMainLayout = new QGridLayout(this);
    m_pMainLayout->addWidget(m_pLableText,0,0,1,10);
    m_pMainLayout->addWidget(m_pComboText,1,0,1,10);
    m_pMainLayout->addWidget(m_pGroupBox,2,0,5,10);
    setLayout(m_pMainLayout);
}

void FilterDlg::ResetEditText()
{
    m_pComboText->setEditText("");
}

//Key Event
void FilterDlg::keyPressEvent(QKeyEvent* keyEvent)
{
    int Key=keyEvent->key();
    switch(Key)
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        this->close();
        break;
    case Qt::Key_Space:
        break;
    }
}

void FilterDlg::FilterRegExpChanged()
{
    int iCurIndex=m_pComboPattern->currentIndex();
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(m_pComboPattern->itemData(iCurIndex).toInt());
    Qt::CaseSensitivity caseSensitivity = m_pCaseSensitivity->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    QString strCurText=m_pComboText->currentText();
    if(iCurIndex==1 && !strCurText.isEmpty())
    {
        //如果在正则表达式模式下，只有一个关键字，则做一次裁剪，去除无效的占位符
        strCurText=strCurText.trimmed();
        //按空格拆分搜索字符串
        QStringList strListText=strCurText.split(" ");
        int iStrListSize=strListText.size();
        if(iStrListSize==1)
            strCurText=strListText.at(0);
        else
        {
            //拼凑正则表达式字符串
            strCurText=strListText.at(0);
            for(int i=1;i<iStrListSize;++i)
            {
                strCurText.append("|").append(strListText.at(i));
            }
        }
    }
    QRegExp regExp(strCurText, caseSensitivity, syntax);
    emit FilterChanged(regExp);
}
