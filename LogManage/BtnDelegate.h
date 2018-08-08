#ifndef BTNDELEGATE_H
#define BTNDELEGATE_H

//#include <QObject>
#include <QMap>
#include <QItemDelegate>
#include <QStyleOptionButton>

class BtnDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit BtnDelegate(QString strBtnText,QObject *parent = nullptr);
    explicit BtnDelegate(QString strBtnText,QString strPixPath,QObject *parent = nullptr);
    ~BtnDelegate();
    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);

private:
    bool    m_bAddPix;      //是否使用背景图
    QString m_strPixPath;   //背景图路径
    QString m_strBtnText;   //按钮显示的文字
    QMap<QModelIndex,QStyleOptionButton*>m_BtnsMap;
signals:
    void BtnClicked(QModelIndex modelIndex);
public slots:
};

#endif // BTNDELEGATE_H
