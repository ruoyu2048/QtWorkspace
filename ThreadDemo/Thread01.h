#ifndef THREAD01_H
#define THREAD01_H

#include <QThread>

class Thread01 : public QThread
{
    Q_OBJECT
public:
    Thread01();

protected:
    virtual void run();

signals:

public slots:

};

#endif // THREAD01_H
