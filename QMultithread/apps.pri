#目标文件生成路径
CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../bind
}else{
    DESTDIR = $$OUT_PWD/../bin
}
