QT += core
QT -= gui

TARGET = QRtsp
CONFIG += c++11
CONFIG += staticlib
CONFIG -= app_bundle

TEMPLATE = lib


CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../../libs_static_d
    #目标文件生成类型(调试版本或发布版本)
    TARGET = $$join(TARGET,,,d)
}else{
    DESTDIR = $$OUT_PWD/../../libs_static
}

###########################################################################################
#OpenSSL configure
win32-msvc{
    #MSVC2017_x64
    INCLUDEPATH += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_vs2017_x64/include
    LIBS += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_vs2017_x64/lib/ssleay32.lib
    LIBS += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_vs2017_x64/lib/libeay32.lib
    LIBS+=user32.lib
    LIBS+=gdi32.lib
    LIBS+=advapi32.lib
}

linux{
    INCLUDEPATH += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_gcc_x64/include
    LIBS += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_gcc_x64/lib/libssl.a
    LIBS += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_gcc_x64/lib/libcrypto.a
    LIBS += -ldl
}
###########################################################################################

INCLUDEPATH += $${PWD}/include
SOURCES += \
    src/AC3AudioFileServerMediaSubsession.cpp \
    src/AC3AudioRTPSink.cpp \
    src/AC3AudioRTPSource.cpp \
    src/AC3AudioStreamFramer.cpp \
    src/ADTSAudioFileServerMediaSubsession.cpp \
    src/ADTSAudioFileSource.cpp \
    src/ADTSAudioStreamDiscreteFramer.cpp \
    src/AMRAudioFileServerMediaSubsession.cpp \
    src/AMRAudioFileSink.cpp \
    src/AMRAudioFileSource.cpp \
    src/AMRAudioRTPSink.cpp \
    src/AMRAudioRTPSource.cpp \
    src/AMRAudioSource.cpp \
    src/AudioInputDevice.cpp \
    src/AudioRTPSink.cpp \
    src/AVIFileSink.cpp \
    src/Base64.cpp \
    src/BasicHashTable.cpp \
    src/BasicTaskScheduler.cpp \
    src/BasicTaskScheduler0.cpp \
    src/BasicUDPSink.cpp \
    src/BasicUDPSource.cpp \
    src/BasicUsageEnvironment.cpp \
    src/BasicUsageEnvironment0.cpp \
    src/BitVector.cpp \
    src/ByteStreamFileSource.cpp \
    src/ByteStreamMemoryBufferSource.cpp \
    src/ByteStreamMultiFileSource.cpp \
    src/DelayQueue.cpp \
    src/DeviceSource.cpp \
    src/DigestAuthentication.cpp \
    src/DVVideoFileServerMediaSubsession.cpp \
    src/DVVideoRTPSink.cpp \
    src/DVVideoRTPSource.cpp \
    src/DVVideoStreamFramer.cpp \
    src/EBMLNumber.cpp \
    src/FileServerMediaSubsession.cpp \
    src/FileSink.cpp \
    src/FramedFileSource.cpp \
    src/FramedFilter.cpp \
    src/FramedSource.cpp \
    src/GenericMediaServer.cpp \
    src/GroupEId.cpp \
    src/Groupsock.cpp \
    src/GroupsockHelper.cpp \
    src/GSMAudioRTPSink.cpp \
    src/H261VideoRTPSource.cpp \
    src/H263plusVideoFileServerMediaSubsession.cpp \
    src/H263plusVideoRTPSink.cpp \
    src/H263plusVideoRTPSource.cpp \
    src/H263plusVideoStreamFramer.cpp \
    src/H263plusVideoStreamParser.cpp \
    src/H264or5VideoFileSink.cpp \
    src/H264or5VideoRTPSink.cpp \
    src/H264or5VideoStreamDiscreteFramer.cpp \
    src/H264or5VideoStreamFramer.cpp \
    src/H264VideoFileServerMediaSubsession.cpp \
    src/H264VideoFileSink.cpp \
    src/H264VideoRTPSink.cpp \
    src/H264VideoRTPSource.cpp \
    src/H264VideoStreamDiscreteFramer.cpp \
    src/H264VideoStreamFramer.cpp \
    src/H265VideoFileServerMediaSubsession.cpp \
    src/H265VideoFileSink.cpp \
    src/H265VideoRTPSink.cpp \
    src/H265VideoRTPSource.cpp \
    src/H265VideoStreamDiscreteFramer.cpp \
    src/H265VideoStreamFramer.cpp \
    src/HashTable.cpp \
    src/HLSSegmenter.cpp \
    src/HMAC_SHA1.cpp \
    src/InputFile.cpp \
    src/IOHandlers.cpp \
    src/JPEG2000VideoRTPSink.cpp \
    src/JPEG2000VideoRTPSource.cpp \
    src/JPEGVideoRTPSink.cpp \
    src/JPEGVideoRTPSource.cpp \
    src/JPEGVideoSource.cpp \
    src/Locale.cpp \
    src/MatroskaDemuxedTrack.cpp \
    src/MatroskaFile.cpp \
    src/MatroskaFileParser.cpp \
    src/MatroskaFileServerDemux.cpp \
    src/MatroskaFileServerMediaSubsession.cpp \
    src/Media.cpp \
    src/MediaSession.cpp \
    src/MediaSink.cpp \
    src/MediaSource.cpp \
    src/MIKEY.cpp \
    src/MP3ADU.cpp \
    src/MP3ADUdescriptor.cpp \
    src/MP3ADUinterleaving.cpp \
    src/MP3ADURTPSink.cpp \
    src/MP3ADURTPSource.cpp \
    src/MP3ADUTranscoder.cpp \
    src/MP3AudioFileServerMediaSubsession.cpp \
    src/MP3AudioMatroskaFileServerMediaSubsession.cpp \
    src/MP3FileSource.cpp \
    src/MP3Internals.cpp \
    src/MP3InternalsHuffman.cpp \
    src/MP3InternalsHuffmanTable.cpp \
    src/MP3StreamState.cpp \
    src/MP3Transcoder.cpp \
    src/MPEG1or2AudioRTPSink.cpp \
    src/MPEG1or2AudioRTPSource.cpp \
    src/MPEG1or2AudioStreamFramer.cpp \
    src/MPEG1or2Demux.cpp \
    src/MPEG1or2DemuxedElementaryStream.cpp \
    src/MPEG1or2DemuxedServerMediaSubsession.cpp \
    src/MPEG1or2FileServerDemux.cpp \
    src/MPEG1or2VideoFileServerMediaSubsession.cpp \
    src/MPEG1or2VideoRTPSink.cpp \
    src/MPEG1or2VideoRTPSource.cpp \
    src/MPEG1or2VideoStreamDiscreteFramer.cpp \
    src/MPEG1or2VideoStreamFramer.cpp \
    src/MPEG2IndexFromTransportStream.cpp \
    src/MPEG2TransportFileServerMediaSubsession.cpp \
    src/MPEG2TransportStreamAccumulator.cpp \
    src/MPEG2TransportStreamDemux.cpp \
    src/MPEG2TransportStreamDemuxedTrack.cpp \
    src/MPEG2TransportStreamFramer.cpp \
    src/MPEG2TransportStreamFromESSource.cpp \
    src/MPEG2TransportStreamFromPESSource.cpp \
    src/MPEG2TransportStreamIndexFile.cpp \
    src/MPEG2TransportStreamMultiplexor.cpp \
    src/MPEG2TransportStreamParser.cpp \
    src/MPEG2TransportStreamParser_PAT.cpp \
    src/MPEG2TransportStreamParser_PMT.cpp \
    src/MPEG2TransportStreamParser_STREAM.cpp \
    src/MPEG2TransportStreamTrickModeFilter.cpp \
    src/MPEG2TransportUDPServerMediaSubsession.cpp \
    src/MPEG4ESVideoRTPSink.cpp \
    src/MPEG4ESVideoRTPSource.cpp \
    src/MPEG4GenericRTPSink.cpp \
    src/MPEG4GenericRTPSource.cpp \
    src/MPEG4LATMAudioRTPSink.cpp \
    src/MPEG4LATMAudioRTPSource.cpp \
    src/MPEG4VideoFileServerMediaSubsession.cpp \
    src/MPEG4VideoStreamDiscreteFramer.cpp \
    src/MPEG4VideoStreamFramer.cpp \
    src/MPEGVideoStreamFramer.cpp \
    src/MPEGVideoStreamParser.cpp \
    src/MultiFramedRTPSink.cpp \
    src/MultiFramedRTPSource.cpp \
    src/NetAddress.cpp \
    src/NetInterface.cpp \
    src/OggDemuxedTrack.cpp \
    src/OggFile.cpp \
    src/OggFileParser.cpp \
    src/OggFileServerDemux.cpp \
    src/OggFileServerMediaSubsession.cpp \
    src/OggFileSink.cpp \
    src/OnDemandServerMediaSubsession.cpp \
    src/ourMD5.cpp \
    src/OutputFile.cpp \
    src/PassiveServerMediaSubsession.cpp \
    src/ProxyServerMediaSession.cpp \
    src/QCELPAudioRTPSource.cpp \
    src/QuickTimeFileSink.cpp \
    src/QuickTimeGenericRTPSource.cpp \
    src/RawVideoRTPSink.cpp \
    src/RawVideoRTPSource.cpp \
    src/RTCP.cpp \
    src/RTPInterface.cpp \
    src/RTPSink.cpp \
    src/RTPSource.cpp \
    src/RTSPClient.cpp \
    src/RTSPCommon.cpp \
    src/RTSPRegisterSender.cpp \
    src/RTSPServer.cpp \
    src/RTSPServerRegister.cpp \
    src/ServerMediaSession.cpp \
    src/SimpleRTPSink.cpp \
    src/SimpleRTPSource.cpp \
    src/SIPClient.cpp \
    src/SRTPCryptographicContext.cpp \
    src/strDup.cpp \
    src/StreamParser.cpp \
    src/StreamReplicator.cpp \
    src/T140TextRTPSink.cpp \
    src/TextRTPSink.cpp \
    src/TheoraVideoRTPSink.cpp \
    src/TheoraVideoRTPSource.cpp \
    src/TLSState.cpp \
    src/uLawAudioFilter.cpp \
    src/UsageEnvironment.cpp \
    src/VideoRTPSink.cpp \
    src/VorbisAudioRTPSink.cpp \
    src/VorbisAudioRTPSource.cpp \
    src/VP8VideoRTPSink.cpp \
    src/VP8VideoRTPSource.cpp \
    src/VP9VideoRTPSink.cpp \
    src/VP9VideoRTPSource.cpp \
    src/WAVAudioFileServerMediaSubsession.cpp \
    src/WAVAudioFileSource.cpp \
    src/inet.c \
    src/rtcp_from_spec.c



HEADERS += \
    include/AC3AudioFileServerMediaSubsession.hh \
    include/AC3AudioRTPSink.hh \
    include/AC3AudioRTPSource.hh \
    include/AC3AudioStreamFramer.hh \
    include/ADTSAudioFileServerMediaSubsession.hh \
    include/ADTSAudioFileSource.hh \
    include/ADTSAudioStreamDiscreteFramer.hh \
    include/AMRAudioFileServerMediaSubsession.hh \
    include/AMRAudioFileSink.hh \
    include/AMRAudioFileSource.hh \
    include/AMRAudioRTPSink.hh \
    include/AMRAudioRTPSource.hh \
    include/AMRAudioSource.hh \
    include/AudioInputDevice.hh \
    include/AudioRTPSink.hh \
    include/AVIFileSink.hh \
    include/Base64.hh \
    include/BasicHashTable.hh \
    include/BasicUDPSink.hh \
    include/BasicUDPSource.hh \
    include/BasicUsageEnvironment.hh \
    include/BasicUsageEnvironment_version.hh \
    include/BasicUsageEnvironment0.hh \
    include/BitVector.hh \
    include/Boolean.hh \
    include/ByteStreamFileSource.hh \
    include/ByteStreamMemoryBufferSource.hh \
    include/ByteStreamMultiFileSource.hh \
    include/DelayQueue.hh \
    include/DeviceSource.hh \
    include/DigestAuthentication.hh \
    include/DVVideoFileServerMediaSubsession.hh \
    include/DVVideoRTPSink.hh \
    include/DVVideoRTPSource.hh \
    include/DVVideoStreamFramer.hh \
    include/FileServerMediaSubsession.hh \
    include/FileSink.hh \
    include/FramedFileSource.hh \
    include/FramedFilter.hh \
    include/FramedSource.hh \
    include/GenericMediaServer.hh \
    include/GroupEId.hh \
    include/Groupsock.hh \
    include/groupsock_version.hh \
    include/GroupsockHelper.hh \
    include/GSMAudioRTPSink.hh \
    include/H261VideoRTPSource.hh \
    include/H263plusVideoFileServerMediaSubsession.hh \
    include/H263plusVideoRTPSink.hh \
    include/H263plusVideoRTPSource.hh \
    include/H263plusVideoStreamFramer.hh \
    include/H264or5VideoFileSink.hh \
    include/H264or5VideoRTPSink.hh \
    include/H264or5VideoStreamDiscreteFramer.hh \
    include/H264or5VideoStreamFramer.hh \
    include/H264VideoFileServerMediaSubsession.hh \
    include/H264VideoFileSink.hh \
    include/H264VideoRTPSink.hh \
    include/H264VideoRTPSource.hh \
    include/H264VideoStreamDiscreteFramer.hh \
    include/H264VideoStreamFramer.hh \
    include/H265VideoFileServerMediaSubsession.hh \
    include/H265VideoFileSink.hh \
    include/H265VideoRTPSink.hh \
    include/H265VideoRTPSource.hh \
    include/H265VideoStreamDiscreteFramer.hh \
    include/H265VideoStreamFramer.hh \
    include/HandlerSet.hh \
    include/HashTable.hh \
    include/HLSSegmenter.hh \
    include/HMAC_hash.hh \
    include/HMAC_SHA1.hh \
    include/InputFile.hh \
    include/IOHandlers.hh \
    include/JPEG2000VideoRTPSink.hh \
    include/JPEG2000VideoRTPSource.hh \
    include/JPEGVideoRTPSink.hh \
    include/JPEGVideoRTPSource.hh \
    include/JPEGVideoSource.hh \
    include/liveMedia.hh \
    include/liveMedia_version.hh \
    include/Locale.hh \
    include/MatroskaFile.hh \
    include/MatroskaFileServerDemux.hh \
    include/Media.hh \
    include/MediaSession.hh \
    include/MediaSink.hh \
    include/MediaSource.hh \
    include/MediaTranscodingTable.hh \
    include/MIKEY.hh \
    include/MP3ADU.hh \
    include/MP3ADUinterleaving.hh \
    include/MP3ADURTPSink.hh \
    include/MP3ADURTPSource.hh \
    include/MP3ADUTranscoder.hh \
    include/MP3AudioFileServerMediaSubsession.hh \
    include/MP3FileSource.hh \
    include/MP3Transcoder.hh \
    include/MPEG1or2AudioRTPSink.hh \
    include/MPEG1or2AudioRTPSource.hh \
    include/MPEG1or2AudioStreamFramer.hh \
    include/MPEG1or2Demux.hh \
    include/MPEG1or2DemuxedElementaryStream.hh \
    include/MPEG1or2DemuxedServerMediaSubsession.hh \
    include/MPEG1or2FileServerDemux.hh \
    include/MPEG1or2VideoFileServerMediaSubsession.hh \
    include/MPEG1or2VideoRTPSink.hh \
    include/MPEG1or2VideoRTPSource.hh \
    include/MPEG1or2VideoStreamDiscreteFramer.hh \
    include/MPEG1or2VideoStreamFramer.hh \
    include/MPEG2IndexFromTransportStream.hh \
    include/MPEG2TransportFileServerMediaSubsession.hh \
    include/MPEG2TransportStreamAccumulator.hh \
    include/MPEG2TransportStreamDemux.hh \
    include/MPEG2TransportStreamFramer.hh \
    include/MPEG2TransportStreamFromESSource.hh \
    include/MPEG2TransportStreamFromPESSource.hh \
    include/MPEG2TransportStreamIndexFile.hh \
    include/MPEG2TransportStreamMultiplexor.hh \
    include/MPEG2TransportStreamTrickModeFilter.hh \
    include/MPEG2TransportUDPServerMediaSubsession.hh \
    include/MPEG4ESVideoRTPSink.hh \
    include/MPEG4ESVideoRTPSource.hh \
    include/MPEG4GenericRTPSink.hh \
    include/MPEG4GenericRTPSource.hh \
    include/MPEG4LATMAudioRTPSink.hh \
    include/MPEG4LATMAudioRTPSource.hh \
    include/MPEG4VideoFileServerMediaSubsession.hh \
    include/MPEG4VideoStreamDiscreteFramer.hh \
    include/MPEG4VideoStreamFramer.hh \
    include/MPEGVideoStreamFramer.hh \
    include/MultiFramedRTPSink.hh \
    include/MultiFramedRTPSource.hh \
    include/NetAddress.hh \
    include/NetCommon.h \
    include/NetInterface.hh \
    include/OggFile.hh \
    include/OggFileServerDemux.hh \
    include/OggFileSink.hh \
    include/OnDemandServerMediaSubsession.hh \
    include/ourMD5.hh \
    include/OutputFile.hh \
    include/PassiveServerMediaSubsession.hh \
    include/ProxyServerMediaSession.hh \
    include/QCELPAudioRTPSource.hh \
    include/QuickTimeFileSink.hh \
    include/QuickTimeGenericRTPSource.hh \
    include/RawVideoFrameParameters.hh \
    include/RawVideoRTPSink.hh \
    include/RawVideoRTPSource.hh \
    include/RTCP.hh \
    include/RTPInterface.hh \
    include/RTPSink.hh \
    include/RTPSource.hh \
    include/RTSPClient.hh \
    include/RTSPCommon.hh \
    include/RTSPRegisterSender.hh \
    include/RTSPServer.hh \
    include/ServerMediaSession.hh \
    include/SimpleRTPSink.hh \
    include/SimpleRTPSource.hh \
    include/SIPClient.hh \
    include/SRTPCryptographicContext.hh \
    include/strDup.hh \
    include/StreamReplicator.hh \
    include/T140TextRTPSink.hh \
    include/TextRTPSink.hh \
    include/TheoraVideoRTPSink.hh \
    include/TheoraVideoRTPSource.hh \
    include/TLSState.hh \
    include/TunnelEncaps.hh \
    include/uLawAudioFilter.hh \
    include/UsageEnvironment.hh \
    include/UsageEnvironment_version.hh \
    include/VideoRTPSink.hh \
    include/VorbisAudioRTPSink.hh \
    include/VorbisAudioRTPSource.hh \
    include/VP8VideoRTPSink.hh \
    include/VP8VideoRTPSource.hh \
    include/VP9VideoRTPSink.hh \
    include/VP9VideoRTPSource.hh \
    include/WAVAudioFileServerMediaSubsession.hh \
    include/WAVAudioFileSource.hh \
    src/EBMLNumber.hh \
    src/H263plusVideoStreamParser.hh \
    src/MatroskaDemuxedTrack.hh \
    src/MatroskaFileParser.hh \
    src/MatroskaFileServerMediaSubsession.hh \
    src/MP3ADUdescriptor.hh \
    src/MP3AudioMatroskaFileServerMediaSubsession.hh \
    src/MP3Internals.hh \
    src/MP3InternalsHuffman.hh \
    src/MP3StreamState.hh \
    src/MPEG2TransportStreamDemuxedTrack.hh \
    src/MPEG2TransportStreamParser.hh \
    src/MPEGVideoStreamParser.hh \
    src/OggDemuxedTrack.hh \
    src/OggFileParser.hh \
    src/OggFileServerMediaSubsession.hh \
    src/rtcp_from_spec.h \
    src/StreamParser.hh





