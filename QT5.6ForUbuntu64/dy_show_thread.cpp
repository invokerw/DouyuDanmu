#include "dy_show_thread.h"


#include "dy_barrage_client.h"
#include <unistd.h>
#include "douyumainwindow.h"
dy_show_thread::dy_show_thread()
{
    stopped = false;
    msg = NULL;
    window = NULL;

}

// zzzz
void dy_show_thread::run()
{
    if(msg == NULL || window == NULL)
        return ;
    qDebug("msg and window in dy_show thread != NULL");
    dy_barrage_client *dyc = (dy_barrage_client *)msg;
    DouyuMainWindow* w = (DouyuMainWindow *)window;
    QString name = "";
    QString say = "";
    while(!stopped)
    {
        //to do .
        dyc->get_dy_server_msg(name,say);
        //qDebug()<<name +"[in thread]:"+say;
        if(name != "" || say != "")
            QMetaObject::invokeMethod(w,"addLine",Q_ARG(QString,name),
                                    Q_ARG(QString,say));
    }
}
void dy_show_thread::setMessage(void *msg, QObject *parent)
{
    this->msg = msg;
    this->window = parent;
}
void dy_show_thread::stop()
{
    stopped = true;
}


