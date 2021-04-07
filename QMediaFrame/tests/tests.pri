#目标文件生成路径
CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../../bind
}else{
    DESTDIR = $$OUT_PWD/../../bin
}

#目标文件生成类型(调试版本或发布版本)
build_pass:CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}
