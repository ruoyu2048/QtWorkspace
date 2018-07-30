#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDateTime>
#include <QMap>
#include <QVector>
#include <QSplitter>
#include <QListView>
#include <QTableView>
#include <QTreeView>
#include <QColumnView>

namespace Ui {
class MainWindow;
}

//该结构用来记录软件状态条目信息
struct SoftWareState
{
    QString stateTime;
    QString swStateInfo;
};
//该结构用来记录软件信息
struct SoftWareInfo
{
    QString swName;
    QVector<SoftWareState> swStates;
};
//该结构用来记录计算机信息
struct PcInfo
{
    QString pcIP;
    QVector<SoftWareInfo> swInfos;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool readSwStateFromFile(QString filePath);

    QStandardItemModel* createModel(QString filterWord);

private slots:
    void on_openBtn_clicked();

    void on_readBtn_clicked();

    void on_expandBtn_clicked();

    void on_filterBtn_clicked();

    void on_recoverBtn_clicked();

private:
    Ui::MainWindow *ui;

    QMap<QString, PcInfo*> pcMaps;

    QStandardItemModel* m_model;

    void InsertSwStateInfo(QString strIP, QString procName, QString stamp, QString strInfo);
};

#endif // MAINWINDOW_H
