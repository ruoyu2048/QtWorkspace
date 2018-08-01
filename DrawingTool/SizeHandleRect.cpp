#include "SizeHandleRect.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <qdebug.h>

//SizeHandleRect::SizeHandleRect(QGraphicsItem* parent , Direction d, QGraphicsItem *resizable)
//    :QGraphicsRectItem(0,0,SELECTION_HANDLE_SIZE,SELECTION_HANDLE_SIZE,parent)
//    ,m_dir(d)
//    ,m_resizable(resizable)
//    ,m_state(SelectionHandleOff)
//{
//    setParentItem(parent);
//    hide();
//}

SizeHandleRect::SizeHandleRect(QGraphicsItem* parent , enumQuadrant qua, QGraphicsItem* pResizeItem)
    :QGraphicsRectItem(0,0,SELECTION_HANDLE_SIZE,SELECTION_HANDLE_SIZE,parent)
{
    m_Quadrant = qua;
    m_pResizeItem=pResizeItem;
    m_State = SelectionHandleOff;
    setParentItem(parent);
    hide();
}

void SizeHandleRect::UpdateCursor()
{
    switch (m_Quadrant) {
    case Right:
        setCursor(Qt::SizeHorCursor);
        return;
    case RightTop:
        setCursor(Qt::SizeBDiagCursor);
        return;
    case RightBottom:
        setCursor(Qt::SizeFDiagCursor);
        return;
    case LeftBottom:
        setCursor(Qt::SizeBDiagCursor);
        return;
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        return;
    case LeftTop:
        setCursor(Qt::SizeFDiagCursor);
        return;
    case Left:
        setCursor(Qt::SizeHorCursor);
        return;
    case Top:
        setCursor(Qt::SizeVerCursor);
        return;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

//手柄的绘制部分，画个6x6的小矩形。
void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QColor c = QColor("limegreen");
  //  c.setAlpha(200);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(c));

    //if ( m_dir == Center )
    if ( m_Quadrant == Center )
    {
        //如果是额外的控制点，就画圈。
        painter->setBrush(QBrush(Qt::blue));
        painter->drawEllipse(rect());
    }
    else
        painter->drawRect(rect());
}


//根据父对象状态改变，决定是否可见。
void SizeHandleRect::SetState(SelectionHandleState st)
{
    if (st == m_State)
        return;
    switch (st) {
    case SelectionHandleOff:
        hide();
        break;
    case SelectionHandleInactive:
    case SelectionHandleActive:
        show();
        break;
    }
    m_State = st;
}

//检查是否选中。
bool SizeHandleRect::hitTest(const QPointF &point)
{
    //qDebug()<<"SizeHandleRect::hitTest(x="<<point.x()<<" y="<<point.y()<<")";
    QPointF pt = mapFromScene(point);
    return rect().contains(pt);
}

//移动到指定的位置。
void SizeHandleRect::move(qreal x, qreal y)
{
    setPos(x,y);
    qDebug()<<"SizeHandleRect::move(x="<<x<<" y="<<y<<")";
}
