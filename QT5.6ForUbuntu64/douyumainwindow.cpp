//#include <QtGui>
#include "douyumainwindow.h"
#include "ui_douyumainwindow.h"
#include <QMessageBox>
#include "dy_barrage_client.h"
#include <unistd.h>
#include <QDebug>
#include "douyuthread.h"
#include "dy_show_thread.h"
#include <QMutex>

#ifndef _MSC_VER
#	include <netdb.h>
#	include <sys/socket.h>
#else
#   include <winsock.h>
#   pragma comment(lib, "ws2_32")
#endif


DouyuMainWindow::DouyuMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DouyuMainWindow)
{
    ui->setupUi(this);

    QRegExp regExp("[1-9][0-9]{0,6}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
    ui->okButton->setEnabled(false);

    //set table view.
    danmu_model = new QStandardItemModel;
    danmu_model->setHorizontalHeaderItem(0,new QStandardItem(tr("Name")));
    danmu_model->setHorizontalHeaderItem(1,new QStandardItem(tr("Say")));
    ui->tableView->setModel(danmu_model);
    //set horizontal  fixed
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    //set column wideth
    ui->tableView->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,315);

    //Header
    ui->tableView->verticalHeader()->hide();

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //no edit
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //context menu
    //ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

      //for eg:
//    danmu_model->setItem(0, 0, new QStandardItem("张三"));
//    danmu_model->setItem(0, 1, new QStandardItem("20120202"));
//    danmu_model->item(0, 0)->setTextAlignment(Qt::AlignCenter);
//    int x = danmu_model->rowCount();
//    qDebug("columu count:%d",x);
//    danmu_model->setItem(1, 0, new QStandardItem("张三"));
//    danmu_model->setItem(1, 1, new QStandardItem("20120202"));
//    danmu_model->item(1, 0)->setTextAlignment(Qt::AlignCenter);
//    x = danmu_model->rowCount();
//    qDebug("columu count:%d",x);

}

DouyuMainWindow::~DouyuMainWindow()
{
    delete ui;
}
void DouyuMainWindow::on_lineEdit_textChanged()
{
    ui->okButton->setEnabled(ui->lineEdit->hasAcceptableInput());
}
void DouyuMainWindow::on_okButton_clicked()
{
     //QMessageBox::warning(this,tr("From Douyu"),tr("You enter the room id "),
     //                    QMessageBox::Yes);
     //424559
     DouYuDanMuShow();
     qDebug("over");
}
void DouyuMainWindow::DouYuDanMuShow()
{
    int room_id = ui->lineEdit->text().toInt(NULL,10);
    char host_name[] = "openbarrage.douyutv.com";
    int32_t port = 8601;

    int ret = dy_bclient.connect_dy_server(host_name,port);
    if(ret != 0)
    {
        QMessageBox::warning(this,tr("From Douyu"),
                             tr("please chech the address and restart the program"),
                             QMessageBox::Yes);
        return;
    }
    ret = dy_bclient.login_dy_room();
    if(ret != 0)
        return ;
    ret = dy_bclient.join_dy_room_group(room_id,-9999);
    if(ret != 0)
        return;
////////////////////////new


    dyThread.setMessage(&dy_bclient);
    dyThread.start();

    dyshowThread.setMessage(&dy_bclient,this);
    dyshowThread.start();
////////////////////////


    return ;
}
void DouyuMainWindow::addLine(QString name,QString say)
{
    qDebug()<<name+":"+say;
    mutex.lock();

    int x = danmu_model->rowCount();
    danmu_model->setItem(x, 0, new QStandardItem(name));
    danmu_model->setItem(x, 1, new QStandardItem(say));
    danmu_model->item(x, 0)->setTextAlignment(Qt::AlignCenter);
    ui->tableView->scrollToBottom();
    mutex.unlock();
}

