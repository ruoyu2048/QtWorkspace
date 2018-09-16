#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include "SubMachine.h"

class PluginMoinitorDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit PluginMoinitorDisplay(QWidget *parent = nullptr);
    void InitMainWindow();
    void setSubMachine(QString strName);

protected:
    virtual void closeEvent(QCloseEvent* ev)override;

private:
    QGridLayout*        m_pGLayout;
    QHBoxLayout*        m_pHLayout_Main;
    QHBoxLayout*        m_pHLayout;
    SubMachine*         m_pSubMachine;
signals:

public slots:
};

#endif // MAINWINDOW_H
