#ifndef NODEFILTERMODEL_H
#define NODEFILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QDateTime>

#define NODE_DATETIME_FORMAT "yyyy-MM-dd:::hh:mm:ss.zzz"

class NodeFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit NodeFilterModel(QObject *parent = 0);
    void SetBeginTime(QDateTime btime){
        m_beginTime = btime;
        invalidateFilter();
    }
    void SetEndTime(QDateTime etime){
        m_endTime = etime;
        invalidateFilter();
    }

signals:

public slots:

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const;

private:
    bool dateInRange(const QString &strDateTime) const;

    bool isParentValid(QModelIndex sourceParent) const;

    int getTreeDepth(QModelIndex sourceParent) const;

    QDateTime m_beginTime;
    QDateTime m_endTime;
    QString m_dtFormat;
};

#endif // NODEFILTERMODEL_H
