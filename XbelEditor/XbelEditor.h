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
    XbelEditor(QWidget *parent = 0);
    ~XbelEditor();

private:
    void initMainWindow();

private slots:
    void onBtnSave();

private:
    QGridLayout* m_pMainLayout;

    XbelTree* m_pXbelTree;

    QPushButton* m_pBtnSave;
};

#endif // XBELEDITOR_H
