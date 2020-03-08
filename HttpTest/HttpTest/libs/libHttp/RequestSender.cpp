#include "RequestSender.h"
#include "Request.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>

const int timeout = 500;

RequestSender::RequestSender(QObject *parent) :
    QObject(parent),
    mManager(new QNetworkAccessManager(this)) {
    a = 100;
    ppp = &a;
}

QByteArray RequestSender::sendRequest(const std::unique_ptr<Request> &request) const {
    QTimer timer;
    timer.setInterval(timeout);
    timer.setSingleShot(true);
    qDebug() << "Sending " << request->requestName();
    QEventLoop loop;
    auto reply = request->sendRequest(mManager);
    if(!reply)
        return {};
    connect(reply.get(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(&timer,      &QTimer::timeout,    reply.get(), &QNetworkReply::abort);
    timer.start();
    loop.exec();

    (*ppp)++;
    QByteArray result;
    if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {
        result = reply->readAll();
    }
    else if(reply->error() != QNetworkReply::NoError) {
        result.append("ERROR: ");
        result.append(reply->errorString().toUtf8());
    }
    qDebug()<<result<<reply->error()<<a;
    return result;
}

