#目标文件生成路径
DESTDIR = $$OUT_PWD/../../bin
CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../../bind
}

#Qt核心依赖库
QT += network

#================================================================#
#依赖库路径宏定义
QAMQP_INCLUDEPATH = $${PWD}/../libs/qamqp
#添加依赖库路径
INCLUDEPATH += $${QAMQP_INCLUDEPATH}
#================================================================#
#////////////////////////////////////////////////////////////////#
#================================================================#
#动态库路径宏定义
QAMQP_LIBS = -lqamqp
CONFIG(debug, debug|release){
    QAMQP_LIBS = -lqamqpd
}

#添加动态库路径
LIBS += -L$${DESTDIR} $${QAMQP_LIBS}
#================================================================#


