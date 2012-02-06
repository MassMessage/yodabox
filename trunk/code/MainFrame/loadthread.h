#ifndef LOADTHREAD_H
#define LOADTHREAD_H

#include <QObject>
#include <QThread>

class LoadThread: public QThread
{
   Q_OBJECT
public:
    LoadThread();
    void run();
};

#endif // LOADTHREAD_H
