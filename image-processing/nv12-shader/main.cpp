#include <QApplication>

#include "yuv_window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Yuv_window window(1920, 1080, "/mnt/hgfs/share/videotestsrc_1920x1080.nv12");
    window.resize(600, 600);
    window.show();
    return app.exec();
}
