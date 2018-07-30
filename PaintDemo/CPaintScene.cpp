#include "CPaintScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QKeyEvent>
#include <QMimeData>
#include <QDebug>

CPaintScene::CPaintScene(QObject *parent) : QGraphicsScene(parent)
{

}

void CPaintScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    if (!mouseEvent->isAccepted()) {
        if (mouseEvent->button() == Qt::LeftButton) {
            QGraphicsLineItem* pLine = new QGraphicsLineItem();
            pLine->setLine(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            addItem(pLine);
            m_pGLine = pLine;
            m_pGItem = pLine;
            qDebug()<<"CPaintScene::mousePressEvent";
        }
    }

}

void CPaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    if (!mouseEvent->isAccepted()) {
        if (mouseEvent->button() == Qt::LeftButton) {
            QLineF newLine(m_pGItem->line().p1(), mouseEvent->scenePos());
            line->setLine(newLine);
            qDebug()<<"CPaintScene::mousePressEvent";
        }
    }
}

void CPaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CPaintScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{

}

void CPaintScene::keyPressEvent(QKeyEvent *event)
{

    QGraphicsScene::keyPressEvent(event);
}

void CPaintScene::Undo(){

}

void CPaintScene::Clear(){

}
void CPaintScene::Save(){

}
void CPaintScene::Recover(){

}
