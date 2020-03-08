#ifndef DELETEREQUEST_H
#define DELETEREQUEST_H
#include "GetRequest.h"

class DeleteRequest : public GetRequest
{
public:
    DeleteRequest() = default;
    std::unique_ptr<QNetworkReply> sendRequest(QNetworkAccessManager *manager) override;
    QString requestName() const override;
};

#endif // DELETEREQUEST_H
