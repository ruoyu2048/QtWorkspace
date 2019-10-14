#ifndef QTHTTPREPLY_H
#define QTHTTPREPLY_H

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QList>

class QtHttpRequest;

class QtHttpReply : public QObject {
    Q_OBJECT
    Q_ENUMS (StatusCode)

public:
    explicit QtHttpReply (QObject * parent = Q_NULLPTR);

    enum StatusCode {
        Ok                 = 200,
        SeeOther           = 303,
        BadRequest         = 400,
        Forbidden          = 403,
        NotFound           = 404,
        MethodNotAllowed   = 405,
        InternalError      = 500,
        NotImplemented     = 501,
        BadGateway         = 502,
        ServiceUnavailable = 503,
    };

    int                                   getRawDataSize (void) const;
    bool                                  useChunked     (void) const;
    StatusCode                            getStatusCode  (void) const;
    const QByteArray &                    getRawData     (void) const;
    const QHash<QByteArray, QByteArray> & getHeaders     (void) const;

    QList<QByteArray> getHeadersList (void) const;

    QByteArray getHeader (const QByteArray & header) const;

    const QtHttpRequest * getRequest (void) const;

    static const QByteArray getStatusTextForCode (StatusCode statusCode);

public slots:
    void setRequest    (QtHttpRequest * request);
    void setUseChunked (bool chunked = false);
    void setStatusCode (StatusCode statusCode);
    void appendRawData (const QByteArray & data);
    void addHeader     (const QByteArray & header, const QByteArray & value);
    void resetRawData  (void);

signals:
    void requestSendHeaders (void);
    void requestSendData    (void);

private:
    bool                          m_useChunked;
    StatusCode                    m_statusCode;
    QByteArray                    m_data;
    QtHttpRequest *               m_request;
    QHash<QByteArray, QByteArray> m_headersHash;
};

#endif // QTHTTPREPLY_H
