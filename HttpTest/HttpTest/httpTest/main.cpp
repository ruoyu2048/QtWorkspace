#include <QCoreApplication>

#include "RequestSender.h"
#include "GetRequest.h"

//using namespace requests;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QString strUrl="http://192.168.1.22";
    std::unique_ptr<GetRequest> taskPtr = std::make_unique<GetRequest>();
    taskPtr.get()->setAddress(strUrl);
    RequestSender rs;
    rs.sendRequest(std::unique_ptr<Request>(taskPtr.get()));



    getchar();

    return a.exec();
}
