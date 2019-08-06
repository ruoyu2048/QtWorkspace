#include <QCoreApplication>

#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <memory>
#include <ctime>
#include <iomanip>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/ndc.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

using std::chrono::system_clock;

string getTime() {
    system_clock::time_point tp = system_clock::now();
    time_t raw_time = system_clock::to_time_t(tp);
    struct tm  *timeinfo = std::localtime(&raw_time);

    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

void doTestRun(int i) {
    static log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));

    string threadName = "doRun-" + to_string(i);
    // 使用NDC给线程设置名称
    log4cplus::NDCContextCreator _ndc(LOG4CPLUS_STRING_TO_TSTRING(threadName));
    int nIndex=0;
    while ( nIndex<10 ) {
        this_thread::sleep_for(std::chrono::seconds(1));
        // 如果不加u8，日志文件会是GB2312格式
        string info = u8"你好：" + getTime();

        LOG4CPLUS_WARN(logger, LOG4CPLUS_STRING_TO_TSTRING(info));
        nIndex++;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //用Initializer类进行初始化
    log4cplus::Initializer initializer;
    PropertyConfigurator pc(LOG4CPLUS_TEXT("log4cplus.properties"));
    pc.configure();

    vector<std::thread> threadVector;

    for (int i = 0; i < 5; i++) {
        threadVector.emplace_back(doTestRun, i);
    }

    for (vector<std::thread>::iterator itr = threadVector.begin(); itr != threadVector.end(); itr++) {
        //启动线程
        itr->join();
    }

    return a.exec();
}
