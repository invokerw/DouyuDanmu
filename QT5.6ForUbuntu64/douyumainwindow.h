#ifndef DOUYUMAINWINDOW_H
#define DOUYUMAINWINDOW_H

#include <QMainWindow>
#include "dy_barrage_client.h"
#include "douyuthread.h"
#include "dy_show_thread.h"
#include <QMutex>
class QStandardItemModel;
namespace Ui {
class DouyuMainWindow;
}
#include "ui_douyumainwindow.h"
class DouyuMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DouyuMainWindow(QWidget *parent = 0);
    ~DouyuMainWindow();
    QStandardItemModel *danmu_model;
private slots:
    void on_lineEdit_textChanged();
    void on_okButton_clicked();
private:
    void DouYuDanMuShow();
private:
    Ui::DouyuMainWindow *ui;
    dy_barrage_client dy_bclient;
    DouyuThread dyThread;
    dy_show_thread dyshowThread;
    QMutex mutex;
public slots:
    void addLine(QString name, QString say);
};

#endif // DOUYUMAINWINDOW_H
