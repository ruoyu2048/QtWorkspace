/*
 * MIT License
 *
 * Copyright (c) 2016-2019 xiongziliang <771730766@qq.com>
 *
 * This file is part of ZLMediaKit(https://github.com/xiongziliang/ZLMediaKit).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <map>
#include <signal.h>
#include <iostream>

#include "Util/logger.h"
#include "Common/config.h"
#include "Player/PlayerProxy.h"

using namespace std;
//using namespace toolkit;
//using namespace mediakit;


#if !defined(SIGHUP)
#define SIGHUP 1
#endif

int main(int argc,char *argv[]) {
    //设置日志
    Logger::Instance().add(std::make_shared<ConsoleChannel>());
    Logger::Instance().add(std::make_shared<FileChannel>());
    Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());
    //加载配置文件，如果配置文件不存在就创建一个
    loadIniConfig();
    string url = "rtsp://192.168.1.22:554/live2.sdp";

    PlayerProxy::Ptr player(new PlayerProxy(DEFAULT_VHOST,
                                            "live",
                                            std::string("chn") + to_string(0).data(),
                                            true, false, false, true));
    //指定RTP over TCP(播放rtsp时有效)
    //(*player)[kRtpType] = Rtsp::RTP_TCP;
    (*player)[kRtpType] = Rtsp::RTP_UDP;
    //开始播放，如果播放失败或者播放中止，将会自动重试若干次，重试次数在配置文件中配置，默认一直重试
    player->play(url);


    getchar();
//    //设置退出信号处理函数
//    static semaphore sem;
//    signal(SIGINT, [](int) { sem.post(); });// 设置退出信号
//    signal(SIGHUP, [](int) { loadIniConfig(); });
//    sem.wait();

    Recorder::stopAll();
    return 0;
}

