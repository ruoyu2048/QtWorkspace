#目标文件生成路径
win32{
    DESTDIR = $$OUT_PWD/../../bin_windows
}

win32-msvc{
    DESTDIR = $$OUT_PWD/../../bin_windows_msvc2017_64
}

linux{
    DESTDIR = $$OUT_PWD/../../bin_linux
}

#Qt核心依赖库
QT += network

#================================================================#
#///////////////////////////第三方依赖库///////////////////////////#
#================================================================#

#添加依赖库路径
INCLUDEPATH += $$PWD/../libs/qsecret
DEPENDPATH += $$PWD/../libs/qsecret

INCLUDEPATH += $$PWD/../libs/qhttp
DEPENDPATH += $$PWD/../libs/qhttp

win32-msvc{
    CONFIG(debug, debug|release){
        LIBS += -L$$OUT_PWD/../../libs_static_d/ -lqhttpd
        LIBS += -L$$OUT_PWD/../../libs_static_d/ -lqsecretd
    }else{
        LIBS += -L$$OUT_PWD/../../libs_static_d/ -lqhttp
        LIBS += -L$$OUT_PWD/../../libs_static_d/ -lqsecret
    }
}


#linux{
#    CONFIG(debug, debug|release){
#        LIBS += $$OUT_PWD/../../libs_static_d/libMediaKitd.a
#    }else{
#        LIBS += $$OUT_PWD/../../libs_static/libMediaKit.a
#    }
#}


###########################################################################################
#OpenSSL configure
win32-msvc{
    #MSVC2017_x64
    INCLUDEPATH += $$PWD/../3rdparty/openssl/openssl-1.0.2o_vs2017_x64/include
    LIBS += $$PWD/../3rdparty/openssl/openssl-1.0.2o_vs2017_x64/lib/ssleay32.lib
    LIBS += $$PWD/../3rdparty/openssl/openssl-1.0.2o_vs2017_x64/lib/libeay32.lib
    LIBS+=user32.lib
    LIBS+=gdi32.lib
    LIBS+=advapi32.lib
}

linux{
    INCLUDEPATH += $$PWD/../3rdparty/openssl//openssl-1.0.2o_gcc_x64/include
    LIBS += $$PWD/../3rdparty/openssl/openssl-1.0.2o_gcc_x64/lib/libssl.a
    LIBS += $$PWD/../3rdparty/openssl/openssl-1.0.2o_gcc_x64/lib/libcrypto.a
    LIBS += -ldl
}
###########################################################################################
