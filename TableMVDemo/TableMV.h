#ifndef TABLEMV_H
#define TABLEMV_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include "TableModel.h"
#include "BtnDelegate.h"

namespace Ui {
class TableMV;
}

class TableMV : public QWidget
{
    Q_OBJECT

public:
    explicit TableMV(QWidget *parent = 0);
    ~TableMV();
private:
    void InitMainWindow();
private:
    Ui::TableMV *ui;
    QVBoxLayout*    m_pVBoxLayout;
    QTableView*     m_pTableView;
    TableModel*     m_pTableModel;
    BtnDelegate*    m_pBtnDelegate;
};

#endif // TABLEMV_H
