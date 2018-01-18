#include <QApplication>

#include "yuv_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Yuv_window window(320, 180, "/home/flypei/study/qt/yu12_shader/test_yuv420p_320x180.yuv");
    window.resize(960, 540);
    window.show();
    return app.exec();
}
