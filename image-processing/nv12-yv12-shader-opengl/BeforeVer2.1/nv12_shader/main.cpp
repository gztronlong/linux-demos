#include <QApplication>

#include "yuv_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Yuv_window window(1920, 1080, "videotestsrc_1920x1080.nv12");//最好是绝对路径
    window.resize(960, 540);
    window.show();
    return app.exec();
}
