#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include "TableModel.h"

class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = 0);
    ~TableView();

    void SetTableViewStyle();
    void InitTableView();
    void SetHHeaders();
    void SetVHeaders();
    void RefrushView();

private:
    TableModel* m_pTableModel;

signals:

public slots:

};

#endif // TABLEVIEW_H
