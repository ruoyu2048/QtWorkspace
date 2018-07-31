#include "drawtool.h"
#include "drawobj.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QtMath>

#define PI 3.14

QList<DrawTool*> DrawTool::c_tools;
QPointF DrawTool::c_down;
QPointF DrawTool::c_last;
quint32 DrawTool::c_nDownFlags;

DrawShape DrawTool::c_drawShape = selection;

static SelectTool selectTool;               //选择对象
static RectTool   lineTool(line);           //线形对象
static RectTool   rectTool(rectangle);      //矩形对象
static RectTool   roundRectTool(roundrect); //圆角矩形
static RectTool   ellipseTool(ellipse);     //椭圆对象
static RotationTool rotationTool;           //旋转工具

enum SelectMode
{
    none,
    netSelect,
    move,
    size,
    rotate
};

SelectMode selectMode = none;

SizeHandleRect::Direction nDragHandle = SizeHandleRect::None;

static void setCursor(QGraphicsScene * scene , const QCursor & cursor )
{
    QList<QGraphicsView*> views  = scene->views();
    if ( views.count() > 0 ){
        QGraphicsView * view = views.first();
        view->setCursor(cursor);
    }
}

DrawTool::DrawTool(DrawShape shape)
{
    //将所有画图的工具保存到QList缓存中，减少对象的重复创建
    m_drawShape = shape ;
    c_tools.push_back(this);
}

void DrawTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    //记录当前鼠标按下的点位置
    c_down = event->scenePos();
    c_last = event->scenePos();
}

void DrawTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    //记录鼠标移动的最新位置
    c_last = event->scenePos();
}

void DrawTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    //如果鼠标释放位置与按下位置相同，则当前的画图方式设置为默认的选择模式
    if (event->scenePos() == c_down)
        c_drawShape = selection;
    setCursor(scene,Qt::ArrowCursor);
}

DrawTool *DrawTool::findTool(DrawShape drawShape)
{
    QList<DrawTool*>::const_iterator iter = c_tools.constBegin();
    for ( ; iter != c_tools.constEnd() ; ++iter ){
        if ((*iter)->m_drawShape == drawShape )
            return (*iter);
    }
    return 0;
}

SelectTool::SelectTool()
    :DrawTool(selection)
{
    m_lastSize.setHeight(0);
    m_lastSize.setWidth(0);
    dashRect = 0;
    dashLine = 0;
    m_hoverSizer = false;
}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mousePressEvent(event,scene);

    if (!m_hoverSizer)
      scene->mouseEvent(event);

    selectMode = none;
    QList<QGraphicsItem *> items = scene->selectedItems();
    GraphicsItem *item = 0;

    if ( items.count() == 1 )
        item = qgraphicsitem_cast<GraphicsItem*>(items.first());

    if ( item != 0 ){
        //碰撞检测当前矩形坐标点
        nDragHandle = item->hitTest(event->scenePos());
        //如果矩形点在矩形边界上，则将选择的模式置为size模式，否则置为move模式。
        if ( nDragHandle !=SizeHandleRect::None)
            selectMode = size;
        else
            selectMode =  move;
        //记录当前图元的碰撞检测区尺寸信息
        m_lastSize = item->boundingRect().size();
    }

    //如果没有选择目标，则将选择模式置为netSelect模式，
    if( selectMode == none ){
        selectMode = netSelect;
        QList<QGraphicsView*> views  = scene->views();
        if ( views.count() > 0 ){
            QGraphicsView * view = views.first();
            //设置视图的拖拽模式为RubberBandDrag(橡皮筋拖拽)模式
            view->setDragMode(QGraphicsView::RubberBandDrag);
        }
    }

    if ( selectMode == move && items.count() == 1 ){
        //清除旧的拓扑图对象与连接线对象
        if (dashRect ){
            scene->removeItem(dashRect);
            delete dashRect;
            dashRect = 0;
        }
        if (dashLine ) {
            scene->removeItem(dashLine);
            delete dashLine;
            dashLine = 0;
        }
        //创建新的拓扑图对象与连接线对象
        dashRect = new QGraphicsPathItem(item->shape());
        dashRect->setPen(Qt::DashLine);
        //将当前图元的在场景中的相对坐标原点赋值给拓扑图元对象
        dashRect->setPos(item->pos());
        //将当前图元的坐标原点赋值给拓扑图元对象
        dashRect->setTransformOriginPoint(item->transformOriginPoint());
        //将当前图元的转换矩阵赋值给拓扑图元对象
        dashRect->setTransform(item->transform());
        //将当前图源的旋转信息赋值给拓扑图源对象
        dashRect->setRotation(item->rotation());
        scene->addItem(dashRect);

        dashLine = new QGraphicsLineItem();
        dashLine->setPos(item->pos());
        dashLine->setPen(Qt::DashLine);
        scene->addItem(dashLine);

    }
    qDebug() << " selectTool press:" <<event->scenePos() << " mode :" << selectMode << " items :" << items.count();
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);
    bool isGroup = false;
    QList<QGraphicsItem *> items = scene->selectedItems();
    GraphicsItem * item = 0;
    //如果选中的目标只有一个图元
    if ( items.count() == 1 ){
        //获取选中的图元对象
        item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0 ){
            //调整图像大小
            if ( nDragHandle != SizeHandleRect::None && selectMode == size ){
                //Unused
                QSizeF delta(c_last.x() - c_down.x() , c_last.y() - c_down.y());
                item->resizeTo(nDragHandle,c_last);
            }
            //更新调整图形大小时的鼠标指针形状
            else if(nDragHandle == SizeHandleRect::None && selectMode == selection ){
                //碰撞检测当前的矩形点位置
                 SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
                 //如果鼠标停留在矩形框的调整点位置时，则将调整大小的标志至为true
                 if ( handle != SizeHandleRect::None){
                     //将当前图元应显示的鼠标形状更新到场景中
                     setCursor(scene,item->getCursor(handle));
                     m_hoverSizer = true;
                 }else{
                     setCursor(scene,Qt::ArrowCursor);
                     m_hoverSizer = false;
                 }
             }
        }
        QGraphicsItemGroup *itemGroup = qgraphicsitem_cast<QGraphicsItemGroup*>(items.first());
        if ( itemGroup != NULL ){
            isGroup = true;
            qDebug()<<"QGraphicsItemGroup:"<<isGroup;
        }
    }
    //单选时移动图元对象处理逻辑
    if ( selectMode == move ){
        if ( dashRect ){
            //c_last = event->scenePos();
            dashRect->setPos(c_last);
            dashLine->setLine(QLineF(dashLine->mapFromItem(item,item->boundingRect().center()),
                                     dashLine->mapFromScene(c_last)));
            scene->update();
        }
    }
    //多选时处理逻辑
    if ( (selectMode != size  && items.count() > 1) || isGroup )
    {
        scene->mouseEvent(event);
    }
}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);

    //单选时，将目标移动到目标位置
    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && selectMode == move && c_last != c_down ){
             item->setPos(c_last);
        }
    }

    //多选时，重置视图的拖拽状态
    if (selectMode == netSelect ){
        QList<QGraphicsView*> views  = scene->views();
        if ( views.count() > 0 ){
            QGraphicsView * view = views.first();
            view->setDragMode(QGraphicsView::NoDrag);
        }
    }

    //重置成员变量
    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;

        scene->removeItem(dashLine);
        delete dashLine;
        dashLine = 0;
    }

    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    m_hoverSizer = false;
    scene->mouseEvent(event);

    qDebug() << " selectTool release:" <<event->scenePos() << "mode :" << selectMode;

}

RotationTool::RotationTool()
    :DrawTool(rotation)
{
    lastAngle == 0;
    dashRect = 0;
}

void RotationTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0 ){
            nDragHandle = item->hitTest(event->scenePos());
            if ( nDragHandle !=SizeHandleRect::None)
            {
                QPointF origin = item->mapToScene(item->boundingRect().center());

                qreal len_y = c_last.y() - origin.y();
                qreal len_x = c_last.x() - origin.x();

                qreal angle = atan2(len_y,len_x)*180/PI;

                lastAngle = angle;
                selectMode = rotate;

                if (dashRect ){
                    scene->removeItem(dashRect);
                    delete dashRect;
                    dashRect = 0;
                }

                dashRect = new QGraphicsPathItem(item->shape());
                dashRect->setPen(Qt::DashLine);
                dashRect->setPos(item->pos());
                dashRect->setTransformOriginPoint(item->transformOriginPoint());
                dashRect->setTransform(item->transform());
                dashRect->setRotation(item->rotation());
                scene->addItem(dashRect);

                setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
            }
            else{

                    scene->clearSelection();
                    c_drawShape = selection;
                }
        }
    }
}

void RotationTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && nDragHandle !=SizeHandleRect::None && selectMode == rotate ){


             QPointF origin = item->mapToScene(item->boundingRect().center());

             qreal len_y = c_last.y() - origin.y();
             qreal len_x = c_last.x() - origin.x();

             qreal angle = atan2(len_y,len_x)*180/PI;

             angle = item->rotation() + int(angle - lastAngle) ;

             if ( angle > 360 )
                 angle -= 360;

             if ( dashRect ){
                dashRect->setRotation( angle );
                 scene->update();
             }

             setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
        }
        else if ( item )
        {
            SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
            if ( handle != SizeHandleRect::None)
                setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
            else
                setCursor(scene,Qt::ArrowCursor);
        }
    }
}

void RotationTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && nDragHandle !=SizeHandleRect::None && selectMode == rotate ){

             QPointF origin = item->mapToScene(item->boundingRect().center());

             QPointF delta = c_last - origin ;
             qreal len_y = c_last.y() - origin.y();
             qreal len_x = c_last.x() - origin.x();
             qreal angle = atan2(len_y,len_x)*180/PI;
             angle = item->rotation() + int(angle - lastAngle) ;
             if ( angle > 360 )
                 angle -= 360;
             item->setRotation( angle );
        }
    }

    setCursor(scene,Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    lastAngle = 0;
    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;
    }
    qDebug() << " RotationTool release:" <<event->scenePos();
}

RectTool::RectTool(DrawShape drawShape)
    :DrawTool(drawShape)
{

}

void RectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();
    switch ( c_drawShape ){
    case rectangle:
        item = new GraphicsRectItem(QRect(0,0,0,0),NULL);
        item->setPen(Qt::NoPen);
        break;
    case ellipse:
        item = new GraphicsEllipseItem(QRect(0,0,0,0),NULL);
        item->setPen(Qt::NoPen);
        break;
    case line:
        item = new GraphicsLineItem(0);
        item->setPen(QPen(Qt::black,5));
        break;
    }
    if ( item == 0)
        return;
    item->setPos(event->scenePos());
    scene->addItem(item);
    item->setSelected(true);

    //设置当前状态为改变大小
    selectMode = size;

    //设置当前控制点为右下角。
    nDragHandle = SizeHandleRect::RightBottom;

    qDebug() << " RectTool shape :" <<m_drawShape << "  " <<event->scenePos();
}

void RectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    setCursor(scene,Qt::CrossCursor);
    //传递消息给选择工具
    selectTool.mouseMoveEvent(event,scene);
}

void RectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, DrawScene *scene)
{
    if ( event->scenePos() == c_down ){
       if ( item != 0)
         scene->removeItem(item);
       selectTool.mousePressEvent(event,scene);
       qDebug()<<"RectTool removeItem:";
    }
    selectTool.mouseReleaseEvent(event,scene);
    qDebug() << " RectTool shape :" <<m_drawShape << "  "<<event->scenePos();

}
