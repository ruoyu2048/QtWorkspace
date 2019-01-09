#ifndef IXBELTREE_H
#define IXBELTREE_H

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>

class IXbelTree : public QTreeWidget
{
    Q_OBJECT

public:
    IXbelTree( QWidget *parent = nullptr );

    virtual ~IXbelTree();

    QDomDocument getDocument();

    QDomElement getDomElementForItem(QTreeWidgetItem* pItem);

    /**
     * @brief setHeaders 设置表头名称
     * @param headerList 表头名称列表
     */
    void setHeaders(QStringList headerList);

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

protected:
    /**
     * @brief setRootElement 设置DomElement的根对象
     * @param rooltElement DomElement的根对象
     */
    virtual void setRootElement( const QDomElement& rooltElement ) = 0;

    /**
     * @brief parseUnitElement 解析元素单元
     * @param element 元素单元根Dom对象
     * @param parentItem 指针对象
     */
    virtual void parseUnitElement(const QDomElement &element,QTreeWidgetItem *parentItem = nullptr ) = 0;

    /**
     * @brief createItem 创建Item对象，并建立索引
     * @param element Dom对象
     * @param parentItem Item对象
     * @return
     */
    QTreeWidgetItem *createItem(const QDomElement &element,QTreeWidgetItem *parentItem = nullptr );

private:
    bool read(QIODevice *device);
    bool write(QIODevice *device);

protected slots:
    /**
     * @brief updateDomElement 更新XML元素信息
     * @param pItem 元素关联的节点指针对象
     * @param column 更新元素所在的列
     */
    virtual void updateDomElement(QTreeWidgetItem* pItem, int column) = 0;

public:
    QIcon m_folderIcon;
    QIcon m_bookmarkIcon;

protected:
    QString m_strName;  //名称
    QString m_strId;    //ID

private:
    QString m_strCfgFilePath;
    QDomDocument m_domDocument;
    QHash<QTreeWidgetItem*, QDomElement> m_domElementForItem;
};

#endif // IXBELTREE_H
