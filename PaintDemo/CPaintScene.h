#ifndef CPAINTSCENE_H
#define CPAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include "CBaseShape.h"

class CPaintScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CPaintScene(QObject *parent = nullptr);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event);

private:
    CBaseShape* m_pCurShape;
    QGraphicsItem* m_pGItem;
    QGraphicsLineItem* m_pGLine;
signals:

public slots:
    void Undo();
    void Clear();
    void Save();
    void Recover();
private:
    void IsSelected();
};

#endif // CPAINTSCENE_H
