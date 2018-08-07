#ifndef SIZEHANDLE
#define SIZEHANDLE

#include <QGraphicsRectItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//控制手柄大小
enum { SELECTION_HANDLE_SIZE = 6,
       SELECTION_MARGIN = 10
     };
//手柄选中状态
enum SelectionHandleState { SelectionHandleOff, SelectionHandleInactive, SelectionHandleActive };

enum Direction { LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left , Center, None};

class SizeHandleRect :public QGraphicsRectItem
{
public:
    //手柄方向
//    enum Direction { LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left , Center, None};

    SizeHandleRect(QGraphicsItem* parent , Direction d, QGraphicsItem *resizable);

    Direction dir() const  { return m_dir; }
    void updateCursor();
    void setState(SelectionHandleState st);
    bool hitTest( const QPointF & point );
    void move(qreal x, qreal y );

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    const Direction m_dir;          //方向
    QPoint m_startPos;              //起始点
    QPoint m_curPos;                //当前点
    QSize m_startSize;              //起始尺寸
    QSize m_curSize;                //当前尺寸
    QGraphicsItem *m_resizable;     //可调整的图形对象
    SelectionHandleState m_state;   //句柄选中状态
};


#endif // SIZEHANDLE

