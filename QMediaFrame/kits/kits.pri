#=============================================================#
#引用libs.pri文件(include(../libs.pri))时，务必将引用语句放在*.pro文
#件中TARGET变量的下方，否目标文件生成类型目标文件生成类型(调试版本或发布版
#本)的条件编译将不起作用。
#=============================================================#
#目标文件生成路径

CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../../bind/kits
}else{
    DESTDIR = $$OUT_PWD/../../bin/kits
}

#目标文件生成类型(调试版本或发布版本)
build_pass:CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}
