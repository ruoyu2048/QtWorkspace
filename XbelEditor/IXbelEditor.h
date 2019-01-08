#ifndef IXBELEDITOR_H
#define IXBELEDITOR_H

#include <QWidget>

class IXbelTree;
class IXbelEditor : public QWidget
{
    Q_OBJECT
public:
    explicit IXbelEditor(QWidget *parent = nullptr);

    virtual ~IXbelEditor();

protected:
    /**
     * @brief initMainWindow 主界面初始化函数
     * @param strCfgFilePath 界面初始化配置文件路径
     */
    virtual void initMainWindow( QString strCfgFilePath ) = 0;

signals:

public slots:

protected:
    /**
     * @brief m_pXbelTree 主界面参数显示树指针对象
     */
    IXbelTree* m_pXbelTree;
};

#endif // IXBELEDITOR_H
