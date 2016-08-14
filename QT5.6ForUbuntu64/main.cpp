#include "douyumainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DouyuMainWindow w;
    w.show();

    return a.exec();
}
