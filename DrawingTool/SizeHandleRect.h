#ifndef SIZEHANDLERECT_H
#define SIZEHANDLERECT_H

#include <QGraphicsRectItem>
#include "GlobalDef.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class SizeHandleRect :public QGraphicsRectItem
{
public:
    //手柄方向
    //enum Direction { LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left , Center, None};
    //SizeHandleRect(QGraphicsItem* parent , Direction d, QGraphicsItem *resizable);
    //Direction dir() const  { return m_dir; }


    SizeHandleRect(QGraphicsItem* parent , enumQuadrant qua, QGraphicsItem* pResizeItem);
    enumQuadrant Quadrant() const  { return m_Quadrant; }
    void UpdateCursor();
    void SetState(SelectionHandleState st);

    bool hitTest( const QPointF & point );
    void move(qreal x, qreal y );

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    //const Direction m_dir;        //方向
    enumQuadrant m_Quadrant;        //象限位置
    QGraphicsItem* m_pResizeItem;     //可调整的图形对象
    SelectionHandleState m_State;   //句柄选中状态
};
#endif // SIZEHANDLERECT_H
