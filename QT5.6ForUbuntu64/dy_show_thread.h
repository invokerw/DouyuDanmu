#ifndef DY_SHOW_THREAD_H
#define DY_SHOW_THREAD_H

#include <QThread>

class dy_show_thread:public QThread
{
     Q_OBJECT
public:
    dy_show_thread();
    void setMessage(void* msg,QObject *parent = NULL);
    void stop();
protected:
    void run();

private:
    void * msg;
    volatile bool stopped; //volatile variable
    QObject* window;
};

#endif // DY_SHOW_THREAD_H
