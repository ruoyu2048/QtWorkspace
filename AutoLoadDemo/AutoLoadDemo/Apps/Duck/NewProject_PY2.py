import os

def mkdirProject(name):
    try:
        os.mkdir( "./plugin/"+name, 0755 ); 
    except IOError:
        print 'Project Existence'

def newpro(name):
    try:
        fobj=open("./plugin/"+name+'/'+name + '.pro',"a")
    except IOError:
        print 'New Project Error:'
    else:
        fobj.write('#------------------------------------------------- \n'
        '# \n'
        '# Project created by NewProject \n'
        '# \n'
        '#------------------------------------------------- \n\n\n'
        'QT       += widgets \n\n\n'
        'TARGET = '+name+'\n'
        'TEMPLATE = lib \n\n\n'
        'DEFINES += '+upperStr(name)+'_LIBRARY \n\n'
        'SOURCES += '+lowerStr(name)+'.cpp\\\n'
        '    form.cpp\n\n\n'
        'HEADERS += '+lowerStr(name) +'.h\\\n'
        '        '+lowerStr(name)+'_global.h \\\n'
        '    form.h\n\n\n')
        fobj.close()

def pluginJSON(name):
    try:
        fobj=open("./plugin/"+name+'/'+lowerStr(name)+'.json',"a")
    except IOError:
        print 'New global_h Erro:'
    else:
        fobj.write('{}')
        fobj.close()


def globalh(name):
    try:
        fobj=open("./plugin/"+name+'/'+lowerStr(name)+'_global.h',"a")
    except IOError:
        print 'New global_h Erro:'
    else:
        fobj.write('#ifndef '+upperStr(name)+'_GLOBAL_H \n'
        '#define  '+upperStr(name)+'_GLOBAL_H \n\n\n'
        '#include <QtCore/qglobal.h>\n\n\n'
        '#if defined('+upperStr(name)+'_LIBRARY)\n'
        '#  define '+upperStr(name)+'SHARED_EXPORT Q_DECL_EXPORT\n'
        '#else\n'
        '#  define '+upperStr(name)+'SHARED_EXPORT Q_DECL_IMPORT\n'
        '#endif\n\n\n'
        '#endif // '+upperStr(name)+'_GLOBAL_H')
        fobj.close()

def pluginH(name):
    try:
        fobj=open("./plugin/"+name+'/'+lowerStr(name)+'.h',"a")
    except IOError:
        print 'New plugin_h Error:'
    else:
        fobj.write('#ifndef '+upperStr(name)+'_H \n'
        '#define '+upperStr(name)+'_H \n\n\n'
        '#define BrushInterface_iid "org.XEQD.PlugAndPaint.BrushInterface"\n\n\n'
        '#include "'+lowerStr(name)+'_global.h"\n'
        '#include "../../src/plugininterface.h"\n'
        '#include <QObject>\n'
        '#include <QWidget>\n\n\n'
        'class '+upperStr(name)+'SHARED_EXPORT '+name+':public QObject,PluginInterFace\n'
        '{\n'
        '   Q_OBJECT\n'
        '   Q_PLUGIN_METADATA(IID BrushInterface_iid FILE "'+lowerStr(name)+'.json")\n'
        '   Q_INTERFACES(PluginInterFace)\n\n\n'
        'public:\n'
        '     QString name() const;\n'
        '     QString iconPath() const;\n'
        '     QWidget *widget() const;\n'
        '     DUCK_WIDGETTYPE duck_Widget() const;\n'
        '     Qt::DockWidgetArea duck_DockWidgetArea() const;\n'
        '};\n'
        '#endif // TESTPLUGIN_H')
        fobj.close()

def pluginCPP(name):
    try:
        fobj=open("./plugin/"+name+'/'+lowerStr(name)+'.cpp',"a")
    except IOError:
        print 'New plugin_cpp Error:'
    else:
        fobj.write('#include "'+lowerStr(name)+'.h" \n'
        '#include "form.h"\n\n\n\n'
        'QString '+name+'::name() const\n'
        '{\n'
        '    return tr("'+name+'");\n'
        '}\n\n'
        'QString '+name+'::iconPath() const\n'
        '{\n'
            'return "://'+name+'.png";\n'
        '}\n\n'
        'QWidget *'+name+'::widget() const\n'
        '{\n'
        '    return new Form();\n'
        '}\n\n'
        'DUCK_WIDGETTYPE '+name+'::duck_Widget() const\n'
        '{'
            'return DUCK_WIDGETTYPE::MDIAREA;\n'
        '}\n\n'
        'Qt::DockWidgetArea '+name+'::duck_DockWidgetArea() const\n'
        '{\n'
            'return Qt::LeftDockWidgetArea;\n'
        '}')
        fobj.close()

def formH(name):
    try:
        fobj=open("./plugin/"+name+'/form.h',"a")
    except IOError:
        print 'New form_h Error:'
    else:
        fobj.write('#ifndef FORM_H \n'
        '#define FORM_H\n\n\n'
        '#include <QWidget>\n\n\n'
        'class Form : public QWidget\n'
        '{\n'
        '    Q_OBJECT\n\n'
        'public:\n'
        '    explicit Form(QWidget *parent = 0);\n'
        '    ~Form();\n\n'
        '};\n\n'
        '#endif // FORM_H')
        fobj.close()

def formCPP(name):
    try:
        fobj=open("./plugin/"+name+'/form.cpp',"a")
    except IOError:
        print 'New form_CPP Error:'
    else:
        fobj.write('#include "form.h" \n'
        'Form::Form(QWidget *parent) :\n'
        '    QWidget(parent)\n'
        '{\n'
        '}\n\n'
        'Form::~Form()\n'
        '{\n'
        '}')
        fobj.close()


def upperStr(inStr):
    return inStr.upper()

def lowerStr(inStr):
    return inStr.lower()

if __name__=="__main__":
    
    fname=raw_input('Enter Project Name:')
    mkdirProject(fname)
    newpro(fname)
    pluginJSON(fname)
    globalh(fname)
    pluginH(fname)
    pluginCPP(fname)
    formH(fname)
    formCPP(fname)