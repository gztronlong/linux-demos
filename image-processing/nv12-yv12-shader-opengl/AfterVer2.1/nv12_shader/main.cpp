#include "playwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CPlayWidget w(1920, 1080, "/home/flypei/study/qt/PlayWidget/videotestsrc_1920x1080.nv12");
    w.show();

    return a.exec();
}
