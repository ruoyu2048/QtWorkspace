#include "IXbelTree.h"

#include <QFile>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>

IXbelTree::IXbelTree(QWidget *parent):QTreeWidget(parent)
{
    header()->setSectionResizeMode(QHeaderView::Stretch);

    m_folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
    m_folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),QIcon::Normal, QIcon::On);
    m_bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
}

IXbelTree::~IXbelTree(){

}

QDomDocument IXbelTree::getDocument(){
    return m_domDocument;
}

QDomElement IXbelTree::getDomElementForItem(QTreeWidgetItem* pItem){
    return  m_domElementForItem.value(pItem);
}

void IXbelTree::setHeaders(QStringList headerList){
    setHeaderLabels(headerList);
}

bool IXbelTree::read(QString filePath)
{
    m_strCfgFilePath = filePath;
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("提示"),QString(tr("无法加载文件 %1:\n%2.")).arg(filePath).arg(file.errorString()));
        return false;
    }
    file.close();

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!m_domDocument.setContent(&file, true, &errorStr, &errorLine,&errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = m_domDocument.documentElement();

    clear();

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    setRootElement(root);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    return true;
}

bool IXbelTree::write(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("提示"),QString(tr("无法加载文件 %1:\n%2.")).arg(filePath).arg(file.errorString()));
        return false;
    }
    const int IndentSize = 4;
    QTextStream out(&file);
    m_domDocument.save(out, IndentSize);
    file.close();
    return true;
}

bool IXbelTree::save(){
    return write(m_strCfgFilePath);
}

QTreeWidgetItem *IXbelTree::createItem(const QDomElement &element,QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    }
    else {
        item = new QTreeWidgetItem(this);
    }
    m_domElementForItem.insert(item, element);
    return item;
}

bool IXbelTree::read(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!m_domDocument.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = m_domDocument.documentElement();
    if (root.tagName() != "xbel") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL version 1.0 "
                                    "file."));
        return false;
    }

    clear();

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    QDomElement child = root.firstChildElement("folder");
    while (!child.isNull()) {
        parseUnitElement(child);
        child = child.nextSiblingElement("folder");
    }

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    return true;
}

bool IXbelTree::write(QIODevice *device)
{
    const int IndentSize = 4;

    QTextStream out(device);
    m_domDocument.save(out, IndentSize);
    return true;
}
