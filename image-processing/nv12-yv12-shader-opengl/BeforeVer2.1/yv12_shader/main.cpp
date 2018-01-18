#include <QApplication>

#include "yuv_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Yuv_window window(320, 180, "test_yuv420p_320x180.yuv");//最好是绝对路径
    window.resize(600, 600);
    window.show();
    return app.exec();
}
