#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QDateTime>

#define DATETIME_FORMAT "yyyy-MM-dd:::hh:mm:ss.zzz"

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FilterModel(QObject *parent = 0);  
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

    bool isSubValid(QModelIndex sourceIndex) const;

    int getTreeDepth(QModelIndex sourceParent) const;

    QDateTime m_beginTime;
    QDateTime m_endTime;
    QString m_dtFormat;
};

#endif // FILTERMODEL_H
