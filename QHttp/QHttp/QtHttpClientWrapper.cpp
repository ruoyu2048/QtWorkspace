
#include "QtHttpClientWrapper.h"
#include "QtHttpRequest.h"
#include "QtHttpReply.h"
#include "QtHttpServer.h"
#include "QtHttpHeader.h"

#include <QCryptographicHash>
#include <QTcpSocket>
#include <QStringBuilder>
#include <QStringList>
#include <QDateTime>

const char       QtHttpClientWrapper::SPACE (' ');
const char       QtHttpClientWrapper::COLON (':');
const QByteArray QtHttpClientWrapper::CRLF  (QByteArrayLiteral ("\r\n"));

QtHttpClientWrapper::QtHttpClientWrapper (QTcpSocket * sock, QtHttpServer * parent)
    : QObject          (parent)
    , m_guid           (QCryptographicHash::hash (
                            QByteArray::number (quint64 (this)),
                            QCryptographicHash::Md5
                            ).toHex ()
                        )
    , m_parsingStatus  (AwaitingRequest)
    , m_sockClient     (sock)
    , m_currentRequest (Q_NULLPTR)
    , m_serverHandle   (parent)
{
    connect (m_sockClient, &QTcpSocket::readyRead, this, &QtHttpClientWrapper::onClientDataReceived);
}

QtHttpClientWrapper::~QtHttpClientWrapper (void) { }

const QByteArray & QtHttpClientWrapper::getGuid (void) const {
    return m_guid;
}

void QtHttpClientWrapper::onClientDataReceived (void) {
    if (m_sockClient != Q_NULLPTR) {
        while (m_sockClient->bytesAvailable () > 0) {
            const QByteArray line = m_sockClient->readLine ();
            switch (m_parsingStatus) { // handle parsing steps
                case AwaitingRequest: { // "command url version" × 1
                    const QString str = QString::fromUtf8 (line).trimmed ();
                    const QStringList parts = str.split (SPACE, QString::SkipEmptyParts);
                    if (parts.size () == 3) {
                        const QByteArray & command = parts.at (0).toLatin1 ();
                        const QString    & url     = parts.at (1);
                        const QByteArray & version = parts.at (2).toLatin1 ();
                        if (version == QtHttpServer::HTTP_VERSION_1_1 ||
                            version == QtHttpServer::HTTP_VERSION_1_0) {
                            m_currentRequest = new QtHttpRequest (this);
                            m_currentRequest->setVersion (version);
                            m_currentRequest->setUrl     (QUrl (url));
                            m_currentRequest->setCommand (command);
                            m_parsingStatus = AwaitingHeaders;
                        }
                        else {
                            m_parsingStatus = ParsingError;
                        }
                    }
                    else {
                        m_parsingStatus = ParsingError;
                    }
                    break;
                }
                case AwaitingHeaders: { // "header: value" × N (until empty line)
                    const QByteArray raw = line.trimmed ();
                    if (!raw.isEmpty ()) { // parse headers
                        const int pos = raw.indexOf (COLON);
                        if (pos > 0) {
                            const QByteArray header = raw.left (pos).trimmed ();
                            const QByteArray value  = raw.mid  (pos +1).trimmed ();
                            m_currentRequest->addHeader (header, value);
                            if (header == QtHttpHeader::ContentLength) {
                                bool ok = false;
                                const int len = value.toInt (&ok, 10);
                                if (ok) {
                                    m_currentRequest->addHeader (QtHttpHeader::ContentLength, QByteArray::number (len));
                                }
                            }
                        }
                        else {
                            m_parsingStatus = ParsingError;
                            qWarning () << "Error : incorrect HTTP headers line :" << line;
                        }
                    }
                    else { // end of headers
                        if (m_currentRequest->getHeader (QtHttpHeader::ContentLength).toInt () > 0) {
                            m_parsingStatus = AwaitingContent;
                        }
                        else {
                            m_parsingStatus = RequestParsed;
                        }
                    }
                    break;
                }
                case AwaitingContent: { // raw data × N (until EOF ??)
                    m_currentRequest->appendRawData (line);
                    if (m_currentRequest->getRawDataSize () == m_currentRequest->getHeader (QtHttpHeader::ContentLength).toInt ()) {
                        m_parsingStatus = RequestParsed;
                    }
                    break;
                }
                default: { break; }
            }
            switch (m_parsingStatus) { // handle parsing status end/error
                case RequestParsed: { // a valid request has ben fully parsed
                    QtHttpReply reply (this);
                    reply.setRequest (m_currentRequest);
                    connect (&reply, &QtHttpReply::requestSendHeaders, this, &QtHttpClientWrapper::onReplySendHeadersRequested);
                    connect (&reply, &QtHttpReply::requestSendData,    this, &QtHttpClientWrapper::onReplySendDataRequested);
                    emit m_serverHandle->requestNeedsReply (m_currentRequest, &reply); // allow app to handle request
                    m_parsingStatus = sendReplyToClient (&reply);
                    break;
                }
                case ParsingError: { // there was an error durin one of parsing steps
                    m_sockClient->readAll (); // clear remaining buffer to ignore content
                    QtHttpReply reply (this);
                    reply.setRequest    (m_currentRequest);
                    reply.setStatusCode (QtHttpReply::BadRequest);
                    reply.appendRawData (QByteArrayLiteral ("<h1>Bad Request (HTTP parsing error) !</h1>"));
                    reply.appendRawData (CRLF);
                    m_parsingStatus = sendReplyToClient (&reply);
                    break;
                }
                default: { break; }
            }
        }
    }
}

void QtHttpClientWrapper::onReplySendHeadersRequested (void) {
    if (QtHttpReply * reply = qobject_cast<QtHttpReply *> (sender ())) {
        // HTTP Version + Status Code + Status Msg
        QByteArray data = (reply->getRequest ()->getVersion () %
                           SPACE %
                           QByteArray::number (reply->getStatusCode ()) %
                           SPACE %
                           QtHttpReply::getStatusTextForCode (reply->getStatusCode ()) %
                           CRLF);
        // Header name: header value
        reply->addHeader (QtHttpHeader::Date, QLocale ("C").toString (QDateTime::currentDateTimeUtc (), "ddd, dd MMM yyyy hh:mm:ss t").toUtf8 ());
        reply->addHeader (QtHttpHeader::Server, m_serverHandle->getServerName ().toUtf8 ());
        if (reply->useChunked ()) {
            static const QByteArray CHUNKED ("chunked");
            reply->addHeader (QtHttpHeader::TransferEncoding, CHUNKED);
        }
        else {
            reply->addHeader (QtHttpHeader::ContentLength, QByteArray::number (reply->getRawDataSize ()));
        }
        if (reply->getRequest ()->getVersion () == QtHttpServer::HTTP_VERSION_1_1) {
            reply->addHeader (QtHttpHeader::Connection, QByteArrayLiteral ("Keep-Alive"));
        }
        const QHash<QByteArray, QByteArray> & headers = reply->getHeaders ();
        for (QHash<QByteArray, QByteArray>::const_iterator it = headers.constBegin (), end = headers.constEnd (); it != end; ++it) {
            data += (it.key () % COLON % SPACE % it.value () % CRLF);
        }
        // empty line
        data += CRLF;
        // write to socket
        m_sockClient->write (data);
    }
}

void QtHttpClientWrapper::onReplySendDataRequested (void) {
    if (QtHttpReply * reply = qobject_cast<QtHttpReply *> (sender ())) {
        // content raw data
        QByteArray data = reply->getRawData ();
        if (reply->useChunked ()) {
            data.prepend (QByteArray::number (data.size (), 16) % CRLF);
            data.append (CRLF);
            reply->resetRawData ();
        }
        // write to socket
        m_sockClient->write (data);
    }
}

QtHttpClientWrapper::ParsingStatus QtHttpClientWrapper::sendReplyToClient (QtHttpReply * reply) {
    if (reply != Q_NULLPTR) {
        if (!reply->useChunked ()) {
            reply->appendRawData (CRLF);
            // send all headers and all data in one shot
            reply->requestSendHeaders ();
            reply->requestSendData ();
        }
        else {
            // last chunk
            static const QByteArray MARKER ('0' % CRLF % CRLF);
            m_sockClient->write (MARKER);
        }
    }
    if (m_currentRequest != Q_NULLPTR) {
        static const QByteArray CLOSE ("close");
        if (m_currentRequest->getHeader (QtHttpHeader::Connection).toLower () == CLOSE) {
            // must close connection after this request
            m_sockClient->close ();
        }
        m_currentRequest->deleteLater ();
        m_currentRequest = Q_NULLPTR;
    }
    return AwaitingRequest;
}
