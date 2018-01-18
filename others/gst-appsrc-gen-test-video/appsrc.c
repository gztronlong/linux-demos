#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <gst/gst.h>
#include <glib.h>
#include <string.h>
#include <sys/time.h>

GMainLoop *g_gst_loop;

static void sigint_handler(int signo);
static GstElement *create_pipeline(const char *name, GMainLoop *loop);
static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data);
static void cb_need_data (GstElement *appsrc, guint unused_size, gpointer user_data);
static inline void RGB24_PIXEL_SET(uint8_t *pixels, uint32_t offset, uint8_t r, uint8_t g, uint8_t b);
static void gen_black_block(uint8_t *buf, int w, int h);
static void gen_white_block(uint8_t *buf, int w, int h);
static void gen_rgb_3block(uint8_t *buf, int w, int h);

GstBuffer *buffer_rgb;
GstBuffer *buffer_black;
GstBuffer *buffer_white;
int video_w = 720;
int video_h = 576;

int main (int   argc, char *argv[]) {
    gst_init (&argc, &argv);
    g_gst_loop = g_main_loop_new (NULL, FALSE);

    // Handle the SIGINT signal, user may use ctrl+c to terminate the program.
    signal(SIGINT, sigint_handler);

    buffer_rgb   = gst_buffer_new_and_alloc(video_w* video_h* 3);
    buffer_black = gst_buffer_new_and_alloc(video_w* video_h* 3);
    buffer_white = gst_buffer_new_and_alloc(video_w* video_h* 3);
    gen_rgb_3block((uint8_t*)GST_BUFFER_DATA(buffer_rgb), video_w, video_h);
    gen_black_block((uint8_t*)GST_BUFFER_DATA(buffer_black), video_w, video_h);
    gen_white_block((uint8_t*)GST_BUFFER_DATA(buffer_white), video_w, video_h);

    GstElement *pipeline = create_pipeline("appsrc", g_gst_loop);
    assert(pipeline);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    guint bus_watch_id = gst_bus_add_watch (bus, bus_call, g_gst_loop);
    gst_object_unref (bus);

    g_print ("Running...\n");
    g_main_loop_run (g_gst_loop);

    g_print ("Stopping playback\n");
    gst_element_set_state (pipeline, GST_STATE_NULL);

    gst_object_unref (GST_OBJECT (pipeline));
    g_source_remove (bus_watch_id);
    g_main_loop_unref (g_gst_loop);

    return 0;
}

/*
 * Signal handler of ctrl+c.
 */
void sigint_handler(int signo) {
     assert(signo == SIGINT);
     // Quit the main loop.
    g_main_loop_quit(g_gst_loop);
}

static GstElement *create_pipeline(const char *name, GMainLoop *loop) {
    GstElement *pipeline = gst_pipeline_new (name);

    GstElement *videosrc         = gst_element_factory_make("appsrc", "appsrc");
    GstElement *capsfilter       = gst_element_factory_make("capsfilter", "capsfilter");
    GstElement *ffmpegcolorspace = gst_element_factory_make("ffmpegcolorspace", "ffmpegcolorspace");

    //GstElement *omx_h264enc      = gst_element_factory_make("omx_h264enc", "omx_h264enc");
    //GstElement *h264parse        = gst_element_factory_make("h264parse", "h264parse");
    //GstElement *mp4mux           = gst_element_factory_make("mp4mux", "mp4mux");
    //GstElement *filesink         = gst_element_factory_make("filesink", "filesink");

    GstElement *omx_ctrl         = gst_element_factory_make("omx_ctrl", "omx_ctrl");
    GstElement *omx_videosink    = gst_element_factory_make("omx_videosink", "omx_videosink");

    if (!pipeline || !videosrc || !ffmpegcolorspace || !omx_ctrl || !omx_videosink) {
    //if (!pipeline || !videosrc || !ffmpegcolorspace || !omx_h264enc || !h264parse || !mp4mux || !filesink) {
        g_printerr ("One element could not be created. Exit.\n");
        return NULL;
    }

    g_object_set(G_OBJECT(videosrc), "caps",
            gst_caps_from_string("video/x-raw-rgb, bpp=(int)24, endianness=(int)4321, "
                "depth=(int)24, red_mask=(int)16711680, green_mask=(int)65280, blue_mask=(int)255, "
                "width=(int)720, height=(int)576, framerate=(fraction)30/1"), NULL);
    g_signal_connect(videosrc, "need-data", G_CALLBACK(cb_need_data), loop);
    //g_object_set(G_OBJECT(videosrc), "num-buffers", 1810, NULL);

    g_object_set(G_OBJECT(omx_ctrl), "display-mode", "OMX_DC_MODE_1080I_60", NULL);
    //g_object_set(G_OBJECT(h264parse), "output-format", 0, NULL);
    //g_object_set(G_OBJECT(filesink), "location", "./appsrc-video-720x576-60s.mp4", NULL);

    gst_bin_add_many(GST_BIN (pipeline), videosrc, capsfilter, ffmpegcolorspace, omx_ctrl, omx_videosink, NULL);
    gst_element_link_many(videosrc, capsfilter, ffmpegcolorspace, omx_ctrl, omx_videosink, NULL);
    //gst_bin_add_many(GST_BIN (pipeline), videosrc, capsfilter, ffmpegcolorspace,
    //                 omx_h264enc, h264parse, mp4mux, filesink, NULL);
    //gst_element_link_many(videosrc, capsfilter, ffmpegcolorspace,
    //                      omx_h264enc, h264parse, mp4mux, filesink, NULL);

    return pipeline;
}

void cb_need_data (GstElement *appsrc, guint unused_size, gpointer user_data) {
    //static int frame_count = 0;
    static int color_count = 0;

    //GstBuffer *buffer = gst_buffer_new_and_alloc(w * h * 3);
    GstBuffer *buffer;

    color_count++;
    if (color_count % 3 == 0)
        buffer = buffer_rgb;
    else if (color_count % 3 == 1)
        buffer = buffer_black;
    else
        buffer = buffer_white;

   // frame_count++;
   // if (frame_count >= 30) {
   //     color_count++;
   //     if (color_count >= 3)
   //         color_count = 0;
   //     frame_count = 0;
   // }

    GstFlowReturn ret;
    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);
    //gst_buffer_unref(buffer);
}

void RGB24_PIXEL_SET(uint8_t *pixels, uint32_t offset, uint8_t r, uint8_t g, uint8_t b) {
    *(pixels + offset + 0) = r;
    *(pixels + offset + 1) = g;
    *(pixels + offset + 2) = b;
}

void gen_black_block(uint8_t *buf, int w, int h) {
    int i;
    for (i = 0; i < w * h * 3; i += 3)
        RGB24_PIXEL_SET(buf, i, 0, 0, 0);
}

void gen_white_block(uint8_t *buf, int w, int h) {
    int i;
    for (i = 0; i < w * h * 3; i += 3)
        RGB24_PIXEL_SET(buf, i, 0xff, 0xff, 0xff);
}

void gen_rgb_3block(uint8_t *buf, int w, int h) {
    int i;
    for (i = 0; i < w * 3 * h / 3; i += 3)
        RGB24_PIXEL_SET(buf, i, 0xff, 0, 0);

    for (i = 0; i < w * h; i+=3)
        RGB24_PIXEL_SET(buf + w * h * 1, i, 0, 0xff, 0);

    for (i = 0; i < w * h; i+=3)
        RGB24_PIXEL_SET(buf + w * h * 2, i, 0, 0, 0xff);
}

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data) {
GMainLoop *loop = (GMainLoop*)data;

switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:  // 达到阀值
        g_print ("End of stream\n");
        g_main_loop_quit (loop);
        break;

    case GST_MESSAGE_ERROR: { // 出现错误
        gchar  *debug;
        GError *error;

        gst_message_parse_error (msg, &error, &debug);
        g_free (debug);

        g_printerr ("Error: %s\n", error->message);
        g_error_free (error);

        g_main_loop_quit (loop);
        break;
        }
    default:
        break;
    }

    return TRUE;
}
