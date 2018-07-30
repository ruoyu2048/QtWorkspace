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
    ~BtnDelegate();
    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index) const;
    bool editorEvent(QEvent *event,QAbstractItemModel *model,const QStyleOptionViewItem &option,const QModelIndex &index);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QString m_strBtnText;   //按钮显示的文字
    QMap<QModelIndex,QStyleOptionButton*>m_BtnsMap;
signals:
    void BtnClicked(QModelIndex modelIndex);
public slots:
};

#endif // BTNDELEGATE_H
