#include <libv4l2.h>
#include <linux/videodev2.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/ioctl.h>

#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QPaintEvent>
#include <QtGui/QColor>

#define RESX 640
#define RESY 480

QWidget* widget;

struct buffer {
    void   *start;
    size_t  length;
};
struct buffer *     BUFFERS   = NULL;
static unsigned int n_buffers = 0;

struct v4l2_buffer V4L2_BUF;

class MyWidget : public QWidget {
public:
    QImage* _p_image;

    MyWidget() {
        _p_image = NULL;
        setAutoFillBackground(false);
    }

    void paintEvent(QPaintEvent* event) {
        if (!_p_image)
            _p_image = new QImage(RESX,RESY, QImage::Format_RGB888);

        const unsigned char* ptr = ((const unsigned char*)BUFFERS[V4L2_BUF.index].start);

        int x = 0;
        int y = 0;
        while (y<RESY) {
            int y1;
            int pb;
            int pr;
            int r;
            int g;
            int b;

            int Y1;
            int Cb;
            int Cr;

            Y1 = *ptr;
            Cb = *(ptr+1);
            Cr = *(ptr+3);
            y1 = (255 * (Y1 -  16)) / 219;
            pb = (255 * (Cb - 128)) / 224;
            pr = (255 * (Cr - 128)) / 224;
            r = y1 + (0    * pb + 1402 * pr)/1000;
            g = y1 + (-344 * pb - 714  * pr)/1000;
            b = y1 + (1772 * pb + 0    * pr)/1000;
            r = r>255?255:r;
            r = r<0?0:r;
            g = g>255?255:g;
            g = g<0?0:g;
            b = b>255?255:b;
            b = b<0?0:b;
            _p_image->setPixel(x,y, r<<16 | g<<8 | b);

            Y1 = *ptr+2;
            Cb = *(ptr+1);
            Cr = *(ptr+3);
            y1 = (255 * (Y1 -  16)) / 219;
            pb = (255 * (Cb - 128)) / 224;
            pr = (255 * (Cr - 128)) / 224;
            r = y1 + (0    * pb + 1402 * pr)/1000;
            g = y1 + (-344 * pb - 714  * pr)/1000;
            b = y1 + (1772 * pb + 0    * pr)/1000;
            r = r>255?255:r;
            r = r<0?0:r;
            g = g>255?255:g;
            g = g<0?0:g;
            b = b>255?255:b;
            b = b<0?0:b;
            _p_image->setPixel(x+1,y, r<<16 | g<<8 | b);

            x += 2;
            if (x > RESX-1) {
                x = 0;
                y++;
            }
            ptr += 4;
        }

        QPainter p(this);
        p.drawImage(QPoint(0,0),*_p_image);
        p.end();

        QWidget::paintEvent(event);
    }

};

void set_input(int fd) {
    int index = 0;
    if (-1 == ioctl(fd, VIDIOC_S_INPUT, &index)) {
        perror("VIDIOC_S_INPUT");
        exit(EXIT_FAILURE);
    }

    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;

    if (-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            fprintf(stderr, "Device is no V4L2 device\n");
            exit(EXIT_FAILURE);
        } else {
            perror("VIDIOC_QUERYCAP");
            exit(EXIT_FAILURE);
        }
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "Device is no video capture device\n");
        exit(EXIT_FAILURE);
    }

    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf(stderr, "Device does not support streaming i/o\n");
        exit(EXIT_FAILURE);
    }

    memset(&cropcap, 0, sizeof(cropcap));
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == ioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if(-1 == ioctl(fd, VIDIOC_S_CROP, &crop)) {
            switch (errno) {
            case EINVAL:
                /* Cropping not supported. */
                break;
            default:
                /* Errors ignored. */
                break;
            }
        }
    } else {
        /* Errors ignored. */
    }

    memset(&fmt, 0, sizeof(fmt));

    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = RESX;
    fmt.fmt.pix.height      = RESY;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

    if (-1 == ioctl(fd, VIDIOC_S_FMT, &fmt)) {
        perror("VIDIOC_S_FMT");
        exit(EXIT_FAILURE);
    }

    /* Note VIDIOC_S_FMT may change width and height. */

    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;

    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;

    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    printf("%d %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
    printf("%d\n",fmt.fmt.pix.sizeimage);

    // Init mmap
    struct v4l2_requestbuffers req;

    memset(&req, 0, sizeof(req));

    req.count  = 2;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl(fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf(stderr, "Device does not support memory mapping\n");
            exit(EXIT_FAILURE);
        } else {
            perror("VIDIOC_REQBUFS");
            exit(EXIT_FAILURE);
        }
    }

    if (req.count < 2) {
        fprintf(stderr, "Insufficient buffer memory on device\n");
        exit(EXIT_FAILURE);
    }

    BUFFERS = (struct buffer*)calloc(req.count, sizeof(*BUFFERS));

    if (!BUFFERS) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));

        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = n_buffers;

        if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)) {
            perror("VIDIOC_QUERYBUF");
            exit(EXIT_FAILURE);
        }

        BUFFERS[n_buffers].length = buf.length;
        BUFFERS[n_buffers].start =
            mmap(NULL /* start anywhere */,
                  buf.length,
                  PROT_READ | PROT_WRITE /* required */,
                  MAP_SHARED /* recommended */,
                  fd, buf.m.offset);

        if (MAP_FAILED == BUFFERS[n_buffers].start) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }
}

void get_info(int fd) {
    struct v4l2_input input;
    int index;

    if (-1 == ioctl(fd, VIDIOC_G_INPUT, &index)) {
        perror("VIDIOC_G_INPUT");
        exit(EXIT_FAILURE);
    }

    memset(&input, 0, sizeof(input));
    input.index = index;

    if (-1 == ioctl(fd, VIDIOC_ENUMINPUT, &input)) {
        perror("VIDIOC_ENUMINPUT");
        exit(EXIT_FAILURE);
    }

    printf("Current input: %s\n", input.name);
}

void get_video_info(int fd) {
    struct v4l2_input input;
    struct v4l2_fmtdesc formats;

    memset(&input, 0, sizeof(input));

    if (-1 == ioctl(fd, VIDIOC_G_INPUT, &input.index)) {
        perror("VIDIOC_G_INPUT");
        exit(EXIT_FAILURE);
    }

    printf("Current input %s supports:\n", input.name);

    memset(&formats, 0, sizeof(formats));
    formats.index = 0;
    formats.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while (0 == ioctl(fd, VIDIOC_ENUM_FMT, &formats)) {
        printf("%s\n", formats.description);
        formats.index++;
    }

    if (errno != EINVAL || formats.index == 0) {
        perror("VIDIOC_ENUMFMT");
        exit(EXIT_FAILURE);
    }
}

void start_capturing(int fd)
{
    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < n_buffers; ++i) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;

        if (-1 == ioctl(fd, VIDIOC_QBUF, &buf)) {
            perror("VIDIOC_QBUF");
            exit(EXIT_FAILURE);
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == ioctl(fd, VIDIOC_STREAMON, &type)) {
        perror("VIDIOC_STREAMON");
        exit(EXIT_FAILURE);
    }
}


int read_frame(int fd)
{
    memset(&V4L2_BUF, 0, sizeof(V4L2_BUF));

    V4L2_BUF.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    V4L2_BUF.memory = V4L2_MEMORY_MMAP;

    if (-1 == ioctl(fd, VIDIOC_DQBUF, &V4L2_BUF)) {
        switch(errno) {
        case EAGAIN:
            return 0;
        default:
            perror("VIDIOC_DQBUF");
            exit(EXIT_FAILURE);
        }
    }

    assert(V4L2_BUF.index < n_buffers);
    //printf("%d %d: ", V4L2_BUF.index, V4L2_BUF.bytesused);

    widget->update();
    QApplication::processEvents();

    if (-1 == ioctl(fd, VIDIOC_QBUF, &V4L2_BUF)) {
        perror("VIDIOC_QBUF");
        exit(EXIT_FAILURE);
    }

    return 1;
}

static void mainloop(int fd)
{
    unsigned int count = 1000000;

    while (count-- > 0 && !widget->isHidden()) {
        for (;;) {
            fd_set fds;
            struct timeval tv;
            int r;

            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* Timeout. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);

            if (-1 == r) {
                if (EINTR == errno)
                    continue;
                perror("select");
            }

            if (0 == r) {
                fprintf(stderr, "select timeout\n");
                exit(EXIT_FAILURE);
            }

            if (read_frame(fd))
                break;
        }
    }
}

int main(int argc, char** argv) {
    QApplication qapp(argc,argv);
    widget = new MyWidget();
    widget->resize(RESX,RESY);
    widget->show();

    int fd = open("/dev/video0", O_RDWR);
    if (!fd) {
        perror("Error opening device");
        exit(EXIT_FAILURE);
    }
    set_input(fd);
    get_info(fd);
    get_video_info(fd);
    start_capturing(fd);
    mainloop(fd);
    close(fd);
}
