#include "BtnDelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QPushButton>

BtnDelegate::BtnDelegate(QString strBtnText,QObject *parent):
    QItemDelegate(parent)
{
    m_bAddPix = false;
    m_strBtnText = strBtnText;
}


BtnDelegate::BtnDelegate(QString strBtnText,QString strPixPath,QObject *parent):
    QItemDelegate(parent)
{
    m_bAddPix = true;
    m_strPixPath = strPixPath;
    m_strBtnText = strBtnText;
}

BtnDelegate::~BtnDelegate()
{
    foreach (QStyleOptionButton *pBtn, m_BtnsMap.values()) {
        if (NULL != pBtn) {
            delete pBtn;
            pBtn = NULL;
        }
    }
}

void BtnDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    QStyleOptionButton * pBtn = m_BtnsMap.value(index);
    if (NULL == pBtn) {
        pBtn = new QStyleOptionButton();
        pBtn->text = m_strBtnText;
        pBtn->state |= QStyle::State_Enabled;
        (const_cast<BtnDelegate *>(this))->m_BtnsMap.insert(index, pBtn);
    }
//    int dx1 = 46/2;
//    int dy1 = 4;
//    int dx2 = (30-(option.rect.width()/2 + 16))/2;
//    int dy2 = -4;
//    pBtn->rect = option.rect.adjusted(dx1, dy1, dx2, dy2); //设置按钮的大小
    pBtn->rect = option.rect;
    painter->save();
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.light());
    }
    if( true == m_bAddPix )
    {
        const QPixmap &pixmap = QPixmap(m_strPixPath);
        int width=pixmap.width();
        int height=pixmap.height();
        int x=pBtn->rect.x()+pBtn->rect.width()/2-width/2;
        int y=pBtn->rect.y()+pBtn->rect.height()/2-height/2;
        painter->drawPixmap(x,y,pixmap);  //设置按钮的背景图片
    }
    painter->restore();
//    QApplication::style()->drawControl(QStyle::CE_PushButtonLabel, pBtn, painter);
    QApplication::style()->drawControl(QStyle::CE_PushButton, pBtn, painter,new QPushButton());
}

bool BtnDelegate::editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent * ev = static_cast<QMouseEvent*>(event);
        if (m_BtnsMap.contains(index)) {
            QStyleOptionButton * pBtn = m_BtnsMap.value(index);
            if (pBtn->rect.contains(ev->x(), ev->y())) {
                pBtn->state |= QStyle::State_Sunken;
            }
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent * ev = static_cast<QMouseEvent*>(event);
        if (m_BtnsMap.contains(index)) {
            QStyleOptionButton * pBtn = m_BtnsMap.value(index);
            if (pBtn->rect.contains(ev->x(), ev->y())) {
                pBtn->state &= (~QStyle::State_Sunken);
                emit BtnClicked(index);
            }
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

