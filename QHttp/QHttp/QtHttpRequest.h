#ifndef QTHTTPREQUEST_H
#define QTHTTPREQUEST_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QHash>
#include <QUrl>

class QtHttpRequest : public QObject {
    Q_OBJECT

public:
    explicit QtHttpRequest (QObject * parent = Q_NULLPTR);
    virtual ~QtHttpRequest (void);

    int                                   getRawDataSize (void) const;
    const QUrl &                          getUrl         (void) const;
    const QByteArray &                    getVersion     (void) const;
    const QByteArray &                    getCommand     (void) const;
    const QByteArray &                    getRawData     (void) const;
    const QHash<QByteArray, QByteArray> & getHeaders     (void) const;

    QList<QByteArray> getHeadersList (void) const;

    QByteArray getHeader (const QByteArray & header) const;

public slots:
    void setVersion    (const QByteArray & version);
    void setUrl        (const QUrl & url);
    void setCommand    (const QByteArray & command);
    void addHeader     (const QByteArray & header, const QByteArray & value);
    void appendRawData (const QByteArray & data);

private:
    QByteArray                    m_version;
    QUrl                          m_url;
    QByteArray                    m_command;
    QByteArray                    m_data;
    QHash<QByteArray, QByteArray> m_headersHash;
};

#endif // QTHTTPREQUEST_H
