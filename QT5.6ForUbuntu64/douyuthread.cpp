#include <QThread>
#include "douyuthread.h"

#include "dy_barrage_client.h"
#include <unistd.h>

DouyuThread::DouyuThread()
{
    stopped = false;
    msg = NULL;
}

// zzzz
void DouyuThread::run()
{
    if(msg == NULL)
        return ;
    qDebug("msg != NULL");
    dy_barrage_client *dyc = (dy_barrage_client *)msg;
    while(!stopped)
    {
        //to do .
        dyc->keep_alive();
        qDebug("in thread. sleep 45s");
        #ifdef _MSC_VER
            Sleep(45000);
        #else
            sleep(45);      //keep live once per 45 seconds
        #endif

    }
}
void DouyuThread::setMessage(void *msg)
{
    this->msg = msg;
}
void DouyuThread::stop()
{
    stopped = true;
}


