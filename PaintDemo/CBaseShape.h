#ifndef CBASESHAPE_H
#define CBASESHAPE_H

#include <QtGui>
#include <QGraphicsScene>

class CBaseShape
{
public:
    enum Code {Line,Rect};
    CBaseShape();
    virtual bool IsValide() = 0;
    //准备绘图==mousePressEvent
    virtual void ReadyToDraw(QGraphicsSceneMouseEvent * event) = 0;
    //正在绘图==mouseMoveEvent
    virtual void Drawing(QGraphicsSceneMouseEvent * event) = 0;
    //结束绘图==mouseReleaseEvent
    virtual void Drown(QGraphicsSceneMouseEvent * event) = 0;
};

#endif // CBASESHAPE_H
