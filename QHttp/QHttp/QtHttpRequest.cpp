
#include "QtHttpRequest.h"
#include "QtHttpHeader.h"

QtHttpRequest::QtHttpRequest (QObject * parent)
    : QObject (parent)
{ }

QtHttpRequest::~QtHttpRequest (void) { }

const QUrl & QtHttpRequest::getUrl (void) const {
    return m_url;
}

const QByteArray & QtHttpRequest::getCommand (void) const {
    return m_command;
}

int QtHttpRequest::getRawDataSize (void) const {
    return m_data.size ();
}

const QByteArray & QtHttpRequest::getVersion (void) const {
    return m_version;
}

const QByteArray & QtHttpRequest::getRawData (void) const {
    return m_data;
}

const QHash<QByteArray, QByteArray> & QtHttpRequest::getHeaders (void) const {
    return m_headersHash;
}

QList<QByteArray> QtHttpRequest::getHeadersList (void) const {
    return m_headersHash.keys ();
}

QByteArray QtHttpRequest::getHeader (const QByteArray & header) const {
    return m_headersHash.value (header, QByteArray ());
}

void QtHttpRequest::setVersion (const QByteArray & version) {
    m_version = version;
}

void QtHttpRequest::setUrl (const QUrl & url) {
    m_url = url;
}

void QtHttpRequest::setCommand (const QByteArray & command) {
    m_command = command;
}

void QtHttpRequest::addHeader (const QByteArray & header, const QByteArray & value) {
    const QByteArray key = header.trimmed ();
    if (!key.isEmpty ()) {
        m_headersHash.insert (key, value);
    }
}

void QtHttpRequest::appendRawData (const QByteArray & data) {
    m_data.append (data);
}
