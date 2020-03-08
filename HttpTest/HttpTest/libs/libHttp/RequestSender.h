#ifndef REQUESTSENDER_H
#define REQUESTSENDER_H

#include <QObject>
#include <memory>

/*eg:
 *  RequestSender rs;
    QString sss="http://192.168.1.222:80/";
    std::unique_ptr<GetRequest> taskPtr = std::make_unique<GetRequest>();
    taskPtr.get()->setAddress(sss);
    rs.sendRequest(std::unique_ptr<Request>(taskPtr.get()));
*/


class QNetworkAccessManager;
class Request;

class RequestSender : public QObject
{
    Q_OBJECT
public:
    explicit RequestSender(QObject *parent = nullptr);
public slots:
    QByteArray sendRequest(const std::unique_ptr<Request> &request) const;
private:
    QNetworkAccessManager *mManager;

    int a;
    int *ppp;
};

#endif // REQUESTSENDER_H
