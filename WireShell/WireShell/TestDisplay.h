#ifndef TESTDISPLAY_H
#define TESTDISPLAY_H

#include <QTableWidget>

class TestDisplay : public QTableWidget
{
    Q_OBJECT
public:
    explicit TestDisplay();
    void displayItems(QString strFile);

private:
    void initMainWindow();

signals:

public slots:
};

#endif // TESTDISPLAY_H
