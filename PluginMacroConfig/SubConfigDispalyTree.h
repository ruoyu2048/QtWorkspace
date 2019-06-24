#ifndef SUBCONFIGDISPALYTREE_H
#define SUBCONFIGDISPALYTREE_H

#include <QTreeWidget>
#include <QXmlStreamWriter>

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
    explicit SubConfigDispalyTree(QString strSubCfgPath);
    void exportSubCofig(QXmlStreamWriter* pWriter);

private:
    void initSubConfigDispalyTree(QString strSubCfgPath);
    int  readEquipmentInfo(QString strPath);

signals:

public slots:
private slots:
    void lineTextEdited(QString strText);

private:

    QMap<QWidget*,QTreeWidgetItem*> m_widgetMap;
    QMap<QString,WidgetTreeItem>m_IdLinkedMap;
};

#endif // SUBCONFIGDISPALYTREE_H
