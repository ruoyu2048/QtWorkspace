#ifndef FILETRANSDLG_H
#define FILETRANSDLG_H

#include <QDialog>
#include <QStringList>

class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QGridLayout;
class FileTransDlg : public QDialog
{
    Q_OBJECT
public:
    explicit FileTransDlg(QDialog *parent = 0);
    //初始化主界面
    void InitMainLayout();
    //初始化信号连接
    void InitConnections();
    //遍历文件夹
    void TraverseFolder();
    //设置文件下发目的路径
    void SetFileDstPaths();
public:
    qint64 m_nSendFlag;
    QString m_strDstPath;
    QString m_strDirPath;
    //记录文件目的路径
    QStringList m_lsDstPaths;
    //记录源文件路径
    QStringList m_lsFilePaths;
private:
    //文件发送类型标签
    QLabel* m_pLBSendType;
    //源文件路径标签
    QLabel* m_pLBFilePath;
    //目的路径标签
    QLabel* m_pLBDstPath;
    //单选按钮 文件
    QRadioButton* m_pRBtnFile;
    //单选按钮 文件夹
    QRadioButton* m_pRBtnFolder;
    //发送文件源路径
    QLineEdit* m_pLEFilePath;
    //发送文件源路径
    QLineEdit* m_pLEDstPath;
    //浏览按钮
    QPushButton* m_pBtnScan;
    //发送按钮
    QPushButton* m_pBtnSend;
    //取消按钮
    QPushButton* m_pBtnCancel;
    //主界面布局
    QGridLayout* m_pMainLayout;
signals:

public slots:
    void SetSendFlag();
    void ScanFile();
    void SendFile();
    void CancelSend();
};

#endif // FILETRANSDLG_H
