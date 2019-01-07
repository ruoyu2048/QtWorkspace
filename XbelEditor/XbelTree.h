
#ifndef XBELTREE_H
#define XBELTREE_H

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>

class XbelTree : public QTreeWidget
{
    Q_OBJECT

public:
    XbelTree( QWidget *parent = nullptr );

    /**
     * @brief read 读取XML配置文件
     * @param filePath XML配置文件路径
     * @return  读取成功返回true，否则返回false
     */
    bool read(QString filePath);

    /**
     * @brief write 更新XML配置文件参数
     * @param filePath XML配置文件路径
     * @return 更新成功返回true,否则返回false
     */
    bool write(QString filePath);

    /**
     * @brief save 保存配置文件
     * @return 成功返回true，否则返回false
     */
    bool save();

private:
    bool read(QIODevice *device);
    bool write(QIODevice *device);

protected slots:
    /**
     * @brief updateDomElement 更新XML元素信息
     * @param item 元素关联的节点指针对象
     * @param column 更新元素所在的列
     */
    void updateDomElement(QTreeWidgetItem *item, int column);

protected:

    virtual void parseUnitElement(const QDomElement &element,QTreeWidgetItem *parentItem = nullptr );

    void parseFolderElement(const QDomElement &element,QTreeWidgetItem *parentItem = nullptr );

    QTreeWidgetItem *createItem(const QDomElement &element,QTreeWidgetItem *parentItem = nullptr );

private:
    QString m_strCfgFilePath;
    QDomDocument m_domDocument;
    QHash<QTreeWidgetItem*, QDomElement> m_domElementForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif
