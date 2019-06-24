#ifndef SUBCONFIGDISPALYTREE_H
#define SUBCONFIGDISPALYTREE_H

#include <QTreeWidget>
#include <QXmlStreamWriter>

struct SubCfgInfo{
    QString strName;//分机名称
    QString strSubId;//分机ID
    QString strBaseName;//文件名(无后缀)
    QString strAbsFilePath;//分机配置文件绝对路径
};

typedef struct _WidgetTreeItem{
    QWidget* pWidget;
    QTreeWidgetItem* pItem;
    _WidgetTreeItem(){
        pWidget=nullptr;
        pItem=nullptr;
    }
}WidgetTreeItem;

class SubConfigDispalyTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SubConfigDispalyTree(SubCfgInfo subCfgInfo);
    QString getSubDevId();
    QString getSubDevName();
    void exportSubCofig(QXmlStreamWriter* pWriter);
    void importSubConfig(QMap<QString,QString>paraValueMap);

private:
    void initSubConfigDispalyTree(QString strSubCfgPath);
    int  readEquipmentInfo(QString strPath);

signals:

public slots:
private slots:
    void lineTextEdited(QString strText);

private:
    QString m_subDevId;
    QString m_subDevName;
    QMap<QWidget*,QTreeWidgetItem*> m_widgetMap;
    QMap<QString,WidgetTreeItem>m_IdLinkedMap;
};

#endif // SUBCONFIGDISPALYTREE_H
