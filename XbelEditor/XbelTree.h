
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
    XbelTree(QWidget *parent = 0);

    bool read(QString filePath);
    bool write(QString filePath);
    bool save();
private:
    bool read(QIODevice *device);
    bool write(QIODevice *device);

private slots:
    void updateDomElement(QTreeWidgetItem *item, int column);

private:
    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);

    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);
private:
    QString m_strFilePath;
    QDomDocument m_domDocument;
    QHash<QTreeWidgetItem*, QDomElement> m_domElementForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif
