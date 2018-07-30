#include "BtnDelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QPushButton>

BtnDelegate::BtnDelegate(QString strBtnText,QObject *parent):
    QItemDelegate(parent)
{
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
        pBtn->state |= QStyle::State_Raised;
        qDebug()<<"BtnInit"<<index.row()<<index.column()<<pBtn->state;
        (const_cast<BtnDelegate *>(this))->m_BtnsMap.insert(index, pBtn);
    }
    pBtn->rect = option.rect;
    //pBtn->rect = option.rect.adjusted(1, 1, -1, -1); //设置按钮的大小
    painter->save();
    if (option.state & QStyle::State_Selected) {
        //painter->fillRect(option.rect, option.palette.light());
        painter->fillRect(option.rect, option.palette.highlight());
    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, pBtn, painter,new QPushButton());
}

bool BtnDelegate::editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index)
{
    if (m_BtnsMap.contains(index)) {
        QStyleOptionButton * pBtn = m_BtnsMap.value(index);
        QMouseEvent * ev = static_cast<QMouseEvent*>(event);
        if (pBtn->rect.contains(ev->x(), ev->y())) {
            if (event->type() == QEvent::MouseButtonPress) {
                pBtn->state &= (~QStyle::State_Raised);
                pBtn->state |= QStyle::State_Sunken;
                qDebug()<<"BtnPress"<<index.row()<<index.column()<<pBtn->state;
            }
            if (event->type() == QEvent::MouseButtonRelease) {
                pBtn->state &= (~QStyle::State_Sunken);
                pBtn->state |= QStyle::State_Raised;
                emit BtnClicked(index);
                qDebug()<<"BtnRelease"<<index.row()<<index.column()<<pBtn->state;
            }
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

void BtnDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug()<<"updateEditorGeometry";
    if (m_BtnsMap.contains(index)) {
        QStyleOptionButton * pBtn = m_BtnsMap.value(index);
        qDebug()<<"updateEditorGeometry"<<pBtn->state;
        if( pBtn->state &= QStyle::State_Sunken ){
            pBtn->rect =option.rect.adjusted(1,1,-1,-1);
            editor->setGeometry(pBtn->rect);
        }

        if( pBtn->state &= QStyle::State_Sunken ){
            pBtn->rect =option.rect;
            editor->setGeometry(pBtn->rect);
        }
    }
}

