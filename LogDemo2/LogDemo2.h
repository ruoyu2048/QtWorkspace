#ifndef LOGDEMO2_H
#define LOGDEMO2_H

#include <QWidget>
#include <QTableView>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QTimer>

namespace Ui {
class LogDemo2;
}

class LogDemo2 : public QWidget
{
    Q_OBJECT

public:
    explicit LogDemo2(QWidget *parent = 0);
    ~LogDemo2();

    void InitMainWindow();

private:
    Ui::LogDemo2 *ui;
    QTimer* m_pTimer;

    qint32 m_nMaxRow;
    QGridLayout* m_pMainLayout;
    QTableView* m_pMainView;
    QStandardItemModel* m_pMainModel;

private slots:
    void UpdateLog();
};

#endif // LOGDEMO2_H
