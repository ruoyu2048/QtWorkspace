#ifndef XBELEDITOR_H
#define XBELEDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class XbelTree;
class XbelEditor : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief XbelEditor XML编辑器对象
     * @param strCfgFilePath 界面初始化配置文件路径
     * @param parent 父对象指针
     */
    XbelEditor(QString strCfgFilePath,QWidget *parent = nullptr);

    ~XbelEditor();

private:
    /**
     * @brief initMainWindow 主界面初始化函数
     * @param strCfgFilePath 界面初始化配置文件路径
     */
    void initMainWindow( QString strCfgFilePath );

private slots:
    /**
     * @brief onBtnSave 保存界面配置槽函数
     */
    void onBtnSave();

private:
    /**
     * @brief m_strCfgFilePath 界面初始化配置文件路径
     */
    QString m_strCfgFilePath;

    /**
     * @brief m_pMainLayout 主界面布局指针对象
     */
    QGridLayout* m_pMainLayout;

    /**
     * @brief m_pXbelTree 主界面参数显示树指针对象
     */
    XbelTree* m_pXbelTree;

    /**
     * @brief m_pBtnSave 保存界面参数按钮指针对象
     */
    QPushButton* m_pBtnSave;
};

#endif // XBELEDITOR_H
