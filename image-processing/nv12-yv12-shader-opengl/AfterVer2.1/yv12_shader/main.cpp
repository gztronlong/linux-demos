#include "playwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CPlayWidget w(0);
    w.show();

    return a.exec();
}
