#ifndef CUSTOM_ITEM_H
#define CUSTOM_ITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>

//QGraphicsScene管理QGraphicsItem（单击/选择/移动/缩放/删除）
// 自定义 Item
class CustomItem : public QGraphicsRectItem
{
public:
    explicit CustomItem(QGraphicsItem *parent = 0);
protected:
    // Shift+左键：进行选择  Alt：准备缩放
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    // Alt+拖拽：进行缩放  移动
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    // 使item可使用qgraphicsitem_cast
    int type() const;
private:
    QPointF m_centerPointF;
    bool m_bResizing;
};

// 自定义 Scene
class CustomScene : public QGraphicsScene
{
protected:
    // 左键：添加item  右键：移除item
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    // Backspace键移除item
    void keyPressEvent(QKeyEvent *event);
};

#endif // CUSTOM_ITEM_H
