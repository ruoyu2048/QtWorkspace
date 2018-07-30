#ifndef CPAINTVIEW_H
#define CPAINTVIEW_H

#include <QObject>
#include <QGraphicsView>
#include "CPaintScene.h"
#include "CItem.h"

class CPaintView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CPaintView(QWidget *parent = nullptr);
    void PVUndo();
    void PVClear();
    void PVSave();
    void PVRecover();

private:
    CPaintScene* m_pPaintScene;
    CustomScene  mCScene;
signals:
    void Undo();
    void Clear();
    void Save();
    void Recover();
public slots:
};

#endif // CPAINTVIEW_H
