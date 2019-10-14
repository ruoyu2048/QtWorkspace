
#include "QtHttpReply.h"
#include "QtHttpHeader.h"

#include <QDateTime>
#include <QLocale>

QtHttpReply::QtHttpReply (QObject * parent)
    : QObject        (parent)
    , m_useChunked   (false)
    , m_statusCode   (Ok)
    , m_data         (QByteArray ())
    , m_request      (Q_NULLPTR)
{ }

int QtHttpReply::getRawDataSize (void) const {
    return m_data.size ();
}

bool QtHttpReply::useChunked (void) const {
    return m_useChunked;
}

QtHttpReply::StatusCode QtHttpReply::getStatusCode (void) const {
    return m_statusCode;
}

const QByteArray & QtHttpReply::getRawData (void) const {
    return m_data;
}

QList<QByteArray> QtHttpReply::getHeadersList (void) const {
    return m_headersHash.keys ();
}

const QHash<QByteArray, QByteArray> & QtHttpReply::getHeaders (void) const {
    return m_headersHash;
}

QByteArray QtHttpReply::getHeader (const QByteArray & header) const {
    return m_headersHash.value (header, QByteArray ());
}

const QtHttpRequest * QtHttpReply::getRequest (void) const {
    return m_request;
}

const QByteArray QtHttpReply::getStatusTextForCode (QtHttpReply::StatusCode statusCode) {
    switch (statusCode) {
        case Ok:         return QByteArrayLiteral ("OK.");
        case BadRequest: return QByteArrayLiteral ("Bad request !");
        case Forbidden:  return QByteArrayLiteral ("Forbidden !");
        case NotFound:   return QByteArrayLiteral ("Not found !");
        default:         return QByteArrayLiteral ("");
    }
}

void QtHttpReply::setRequest (QtHttpRequest * request) {
    m_request = request;
}

void QtHttpReply::setUseChunked (bool chunked){
    m_useChunked = chunked;
}

void QtHttpReply::setStatusCode (QtHttpReply::StatusCode statusCode) {
    m_statusCode = statusCode;
}

void QtHttpReply::appendRawData (const QByteArray & data) {
    m_data.append (data);
}

void QtHttpReply::addHeader (const QByteArray & header, const QByteArray & value) {
    const QByteArray key = header.trimmed ();
    if (!key.isEmpty ()) {
        m_headersHash.insert (key, value);
    }
}

void QtHttpReply::resetRawData (void) {
    m_data.clear ();
}
