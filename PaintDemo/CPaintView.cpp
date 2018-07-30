#include "CPaintView.h"

CPaintView::CPaintView(QWidget *parent) : QGraphicsView(parent)
{
    m_pPaintScene = new CPaintScene(this);
    m_pPaintScene->setSceneRect(-300,-200,600,400);
    this->setScene(m_pPaintScene);

//      mCScene.setSceneRect(0, 0, 400, 300);
//      this->setScene(&mCScene);
}

void CPaintView::PVUndo(){
    emit Undo();
}
void CPaintView::PVClear(){
    emit Clear();
}
void CPaintView::PVSave(){
    emit Save();
}
void CPaintView::PVRecover(){
    emit Recover();
}
