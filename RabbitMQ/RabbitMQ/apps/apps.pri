#目标文件生成路径
DESTDIR = $$OUT_PWD/../../bin
CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../../bind
}

#Qt核心依赖库
QT += network

#================================================================#
#依赖库路径宏定义
THIRD_INCLUDEPATH  = $${PWD}/../libs
#添加依赖库路径
INCLUDEPATH += $${THIRD_INCLUDEPATH}
#================================================================#
#////////////////////////////////////////////////////////////////#
#================================================================#
#动态库路径宏定义
QAMQP_LIBS = -lqamqp
QFTP_LIBS = -lqftp
QLOG4_LIBS = -llog4qt
CONFIG(debug, debug|release){
    QAMQP_LIBS = -lqamqpd
    QFTP_LIBS  = -lqftpd
    QLOG4_LIBS = -llog4qtd
}

#添加动态库路径
LIBS += -L$${DESTDIR} $${QAMQP_LIBS}
LIBS += -L$${DESTDIR} $${QFTP_LIBS}
LIBS += -L$${DESTDIR} $${QLOG4_LIBS}
#================================================================#


