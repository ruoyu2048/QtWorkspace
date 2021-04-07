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

#================================================================#
#///////////////////////////第三方依赖库///////////////////////////#
#================================================================#

#添加依赖库路径
###########################################################################################
#OpenSSL configure
win32-msvc{
    #MSVC2017_x64
    INCLUDEPATH += $$PWD/../thirdLib/openssl/openssl-1.0.2o_vs2017_x64/include
    LIBS += $$PWD/../thirdLib/openssl/openssl-1.0.2o_vs2017_x64/lib/ssleay32.lib
    LIBS += $$PWD/../thirdLib/openssl/openssl-1.0.2o_vs2017_x64/lib/libeay32.lib
    LIBS+=user32.lib
    LIBS+=gdi32.lib
    LIBS+=advapi32.lib
}

linux{
    INCLUDEPATH += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_gcc_x64/include
    DEPENDPATH += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_gcc_x64/include
    LIBS += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_gcc_x64/lib/libssl.a
    LIBS += $$PWD/../../thirdLib/openssl/openssl-1.0.2o_gcc_x64/lib/libcrypto.a
    LIBS += -ldl
}
###########################################################################################

INCLUDEPATH += $$PWD/../libs/QRtsp/include
DEPENDPATH += $$PWD/../libs/QRtsp/include

win32-msvc{
    #MSVC2017_x64
    LIBS+=WS2_32.lib
    LIBS+=Iphlpapi.lib
    LIBS+=shlwapi.lib
    LIBS+=kernel32.lib
    LIBS+=user32.lib
    LIBS+=gdi32.lib
    LIBS+=winspool.lib
    LIBS+=shell32.lib
    LIBS+=ole32.lib
    LIBS+=oleaut32.lib
    LIBS+=uuid.lib
    LIBS+=comdlg32.lib
    LIBS+=advapi32.lib
    CONFIG(debug, debug|release){
        LIBS += $$OUT_PWD/../../libs_static_d/QRtspd.lib
    }else{
        LIBS += $$OUT_PWD/../../libs_static/QRtsp.lib
    }
}

linux{
    CONFIG(debug, debug|release){
        LIBS += $$OUT_PWD/../../libs_static_d/libQRtspd.a
    }else{
        LIBS += $$OUT_PWD/../../libs_static/libQRtspd.a
    }
}

