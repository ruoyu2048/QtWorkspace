
TARGET = MediaKit
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib
CONFIG -= qt
include(../libs.pri)

DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += \
    Codec/AACEncoder.cpp \
    Codec/H264Encoder.cpp \
    Common/config.cpp \
    Common/Device.cpp \
    Common/MediaSink.cpp \
    Common/MediaSource.cpp \
    Common/Parser.cpp \
    Common/Stamp.cpp \
    Http/HttpBody.cpp \
    Http/HttpChunkedSplitter.cpp \
    Http/HttpClient.cpp \
    Http/HttpClientImp.cpp \
    Http/HttpCookie.cpp \
    Http/HttpCookieManager.cpp \
    Http/HttpDownloader.cpp \
    Http/HttpFileManager.cpp \
    Http/HttpRequester.cpp \
    Http/HttpRequestSplitter.cpp \
    Http/HttpSession.cpp \
    Http/strCoding.cpp \
    Http/WebSocketSplitter.cpp \
    Player/MediaPlayer.cpp \
    Player/PlayerBase.cpp \
    Player/PlayerProxy.cpp \
    Pusher/MediaPusher.cpp \
    Pusher/PusherBase.cpp \
    Record/HlsMaker.cpp \
    Record/HlsMakerImp.cpp \
    Record/HlsMediaSource.cpp \
    Record/MP4Muxer.cpp \
    Record/MP4Reader.cpp \
    Record/MP4Recorder.cpp \
    Record/Recorder.cpp \
    Record/TsMuxer.cpp \
    Rtmp/amf.cpp \
    Rtmp/FlvMuxer.cpp \
    Rtmp/Rtmp.cpp \
    Rtmp/RtmpDemuxer.cpp \
    Rtmp/RtmpMuxer.cpp \
    Rtmp/RtmpPlayer.cpp \
    Rtmp/RtmpProtocol.cpp \
    Rtmp/RtmpPusher.cpp \
    Rtmp/RtmpSession.cpp \
    Rtmp/utils.cpp \
    Rtp/PSDecoder.cpp \
    Rtp/RtpDecoder.cpp \
    Rtp/RtpProcess.cpp \
    Rtp/RtpSelector.cpp \
    Rtp/RtpSession.cpp \
    Rtp/RtpSplitter.cpp \
    Rtp/UdpRecver.cpp \
    Rtsp/RtpCodec.cpp \
    Rtsp/RtpMultiCaster.cpp \
    Rtsp/RtpReceiver.cpp \
    Rtsp/Rtsp.cpp \
    Rtsp/RtspDemuxer.cpp \
    Rtsp/RtspMuxer.cpp \
    Rtsp/RtspPlayer.cpp \
    Rtsp/RtspPusher.cpp \
    Rtsp/RtspSession.cpp \
    Rtsp/RtspSplitter.cpp \
    Rtsp/UDPServer.cpp \
    Shell/ShellSession.cpp \
    Extension/AAC.cpp \
    Extension/AACRtmp.cpp \
    Extension/AACRtp.cpp \
    Extension/Factory.cpp \
    Extension/Frame.cpp \
    Extension/H264.cpp \
    Extension/H264Rtmp.cpp \
    Extension/H264Rtp.cpp \
    Extension/H265.cpp \
    Extension/H265Rtp.cpp \
    Extension/SPSParser.c


HEADERS += \
    Codec/AACEncoder.h \
    Codec/H264Encoder.h \
    Common/config.h \
    Common/Device.h \
    Common/MediaSink.h \
    Common/MediaSource.h \
    Common/MultiMediaSourceMuxer.h \
    Common/Parser.h \
    Common/Stamp.h \
    Http/HttpBody.h \
    Http/HttpChunkedSplitter.h \
    Http/HttpClient.h \
    Http/HttpClientImp.h \
    Http/HttpCookie.h \
    Http/HttpCookieManager.h \
    Http/HttpDownloader.h \
    Http/HttpFileManager.h \
    Http/HttpRequester.h \
    Http/HttpRequestSplitter.h \
    Http/HttpSession.h \
    Http/strCoding.h \
    Http/strptime_win.h \
    Http/WebSocketClient.h \
    Http/WebSocketSession.h \
    Http/WebSocketSplitter.h \
    Player/MediaPlayer.h \
    Player/PlayerBase.h \
    Player/PlayerProxy.h \
    Pusher/MediaPusher.h \
    Pusher/PusherBase.h \
    Record/HlsMaker.h \
    Record/HlsMakerImp.h \
    Record/HlsMediaSource.h \
    Record/HlsRecorder.h \
    Record/MP4Muxer.h \
    Record/MP4Reader.h \
    Record/MP4Recorder.h \
    Record/Recorder.h \
    Record/TsMuxer.h \
    Rtmp/amf.h \
    Rtmp/FlvMuxer.h \
    Rtmp/Rtmp.h \
    Rtmp/RtmpCodec.h \
    Rtmp/RtmpDemuxer.h \
    Rtmp/RtmpMediaSource.h \
    Rtmp/RtmpMediaSourceImp.h \
    Rtmp/RtmpMediaSourceMuxer.h \
    Rtmp/RtmpMuxer.h \
    Rtmp/RtmpPlayer.h \
    Rtmp/RtmpPlayerImp.h \
    Rtmp/RtmpProtocol.h \
    Rtmp/RtmpPusher.h \
    Rtmp/RtmpSession.h \
    Rtmp/utils.h \
    Rtp/PSDecoder.h \
    Rtp/RtpDecoder.h \
    Rtp/RtpProcess.h \
    Rtp/RtpSelector.h \
    Rtp/RtpSession.h \
    Rtp/RtpSplitter.h \
    Rtp/UdpRecver.h \
    Rtsp/RtpCodec.h \
    Rtsp/RtpMultiCaster.h \
    Rtsp/RtpReceiver.h \
    Rtsp/Rtsp.h \
    Rtsp/RtspDemuxer.h \
    Rtsp/RtspMediaSource.h \
    Rtsp/RtspMediaSourceImp.h \
    Rtsp/RtspMediaSourceMuxer.h \
    Rtsp/RtspMuxer.h \
    Rtsp/RtspPlayer.h \
    Rtsp/RtspPlayerImp.h \
    Rtsp/RtspPusher.h \
    Rtsp/RtspSession.h \
    Rtsp/RtspSplitter.h \
    Rtsp/UDPServer.h \
    Shell/ShellCMD.h \
    Shell/ShellSession.h \
    Extension/AAC.h \
    Extension/AACRtmp.h \
    Extension/AACRtp.h \
    Extension/Factory.h \
    Extension/Frame.h \
    Extension/H264.h \
    Extension/H264Rtmp.h \
    Extension/H264Rtp.h \
    Extension/H265.h \
    Extension/H265Rtp.h \
    Extension/SPSParser.h \
    Extension/Track.h






INCLUDEPATH += $$PWD/../libmov/include
DEPENDPATH += $$PWD/../libmov/include

INCLUDEPATH += $$PWD/../libmpeg/include
DEPENDPATH += $$PWD/../libmpeg/include

INCLUDEPATH += $$PWD/../libflv/include
DEPENDPATH += $$PWD/../libflv/include

INCLUDEPATH += $$PWD/../librtp/include
DEPENDPATH += $$PWD/../librtp/include

INCLUDEPATH += $$PWD/../ToolKit
DEPENDPATH += $$PWD/../ToolKit
LIBS += -L$$OUT_PWD/../ToolKit/debug/ -lToolKit
