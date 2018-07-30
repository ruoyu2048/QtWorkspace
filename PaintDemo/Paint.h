#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QSpacerItem>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHBoxLayout>
#include "CPaintView.h"


namespace Ui {
class Paint;
}

class Paint : public QWidget
{
    Q_OBJECT

public:
    explicit Paint(QWidget *parent = 0);
    ~Paint();

private:
    void InitMainWindow();
    void Connections();

private:
    Ui::Paint *ui;
    QLabel*     m_pLBShape;         //图形形状
    QLabel*     m_pLBPenColor;      //画笔颜色
    QLabel*     m_pLBPenStyle;      //画笔风格
    QLabel*     m_pLBPenCap;        //画笔帽顶风格
    QLabel*     m_pLBPJoinPoint;    //连接点类型
    QLabel*     m_pLBFillMode;      //填充类型
    QLabel*     m_pSpredEffect;     //扩展风格
    QLabel*     m_pLBBrushColer;    //画刷颜色
    QLabel*     m_pLBBrushStyle;    //画刷风格

    QPushButton*    m_pBtnUndo;
    QPushButton*    m_pBtnClear;
    QPushButton*    m_pBtnSave;
    QPushButton*    m_pBtnRecover;
    QSpacerItem*    m_pBtnsSpacer;

    QGridLayout*    m_pMainLayout;
    QHBoxLayout*    m_pHBtnsLayout;
    CPaintView*     m_pPaintView;

private slots:
    void BtnUndo();
    void BtnClear();
    void BtnSave();
    void BtnRecover();
};

#endif // PAINT_H
