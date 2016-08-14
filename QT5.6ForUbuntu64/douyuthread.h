#ifndef DOUYUTHREAD_H
#define DOUYUTHREAD_H

#include <QThread>

class DouyuThread : public QThread
{
    Q_OBJECT
public:
    DouyuThread();

    void setMessage(void* msg);
    void stop();
protected:
    void run();

private:
    void * msg;
    volatile bool stopped; //volatile variable
};

#endif // DOUYUTHREAD_H
