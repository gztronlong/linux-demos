#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char uint8_t;

#define WIDTH  (800)
#define HEIGHT (480)
#define COMPONENTS  (3)  // bytes per pixel

static int convert_yuv422_to_rgb24_pixel(int y, int u, int v);
static int convert_yuv422_to_rgb24_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

static int rgb24_to_bmp(unsigned char *rgb24_buffer, int width,int height,const char *bmppath);

int main(int argc, char *argv[]) {
    printf("Program start...\n");

    //FILE *fp_rgb = NULL;
    //if((fp_rgb = fopen("800x480.rgba","wb")) == NULL) {
    //    printf("Error: Cannot open rgba file.\n");
    //    return -1;
    //}
    //char *rgb24buf = (char *)malloc(WIDTH * HEIGHT * 4);
    //fseek(fp_rgb, WIDTH * HEIGHT * COMPONENTS * 20, 0);
    //fread(rgb24buf, 1, WIDTH * HEIGHT * COMPONENTS, fp_rgb);

    //char rgb24buf[WIDTH * HEIGHT * COMPONENTS];
    //memset(rgb24buf, 0xaa, WIDTH * HEIGHT * COMPONENTS);
    unsigned char yuv422buf[WIDTH * HEIGHT * 2];
    memset(yuv422buf, 0xaa, WIDTH * HEIGHT * 2);
    unsigned char rgb24buf[WIDTH * HEIGHT * 3];

    convert_yuv422_to_rgb24_buffer(yuv422buf, rgb24buf, WIDTH, HEIGHT);
    rgb24_to_bmp(rgb24buf, WIDTH, HEIGHT, "sample.bmp");

    printf("Program exit\n");
    return 0;
}

int convert_yuv422_to_rgb24_pixel(int y, int u, int v)
{
    unsigned int pixel32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel32;
    int r, g, b;
    r = y + (1.370705 * (v-128));
    g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
    b = y + (1.732446 * (u-128));
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;
    pixel[0] = r ;
    pixel[1] = g ;
    pixel[2] = b ;
    return pixel32;
}

// YUYV
int convert_yuv422_to_rgb24_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
    unsigned int  in, out = 0;
    unsigned int  pixel_16;
    unsigned char pixel_24[3];
    unsigned int  pixel32;
    int y0, u, y1, v;

    for(in = 0; in < width * height * 2; in += 4) {
        pixel_16 = yuv[in + 3] << 24 | yuv[in + 2] << 16 | yuv[in + 1] <<  8 | yuv[in + 0];
        y0 = (pixel_16 & 0x000000ff);
        u  = (pixel_16 & 0x0000ff00) >>  8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v  = (pixel_16 & 0xff000000) >> 24;

        pixel32 = convert_yuv422_to_rgb24_pixel(y0, u, v);

        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];

        pixel32 = convert_yuv422_to_rgb24_pixel(y1, u, v);

        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
    }
    return 0;
}

int rgb24_to_bmp(unsigned char *rgb24_buffer, int width,int height,const char *bmppath){
    typedef struct
    {
        long imageSize;
        long blank;
        long startPosition;
    }BmpHead;

    typedef struct
    {
        long  Length;
        long  width;
        long  height;
        unsigned short  colorPlane;
        unsigned short  bitColor;
        long  zipFormat;
        long  realSize;
        long  xPels;
        long  yPels;
        long  colorUse;
        long  colorImportant;
    }InfoHead;

    int i=0,j=0;
    BmpHead m_BMPHeader={0};
    InfoHead  m_BMPInfoHeader={0};
    char bfType[2]={'B','M'};
    int header_size=sizeof(bfType)+sizeof(BmpHead)+sizeof(InfoHead);
    FILE *fp_bmp=NULL;

    if((fp_bmp=fopen(bmppath,"wb"))==NULL){
        printf("Error: Cannot open output BMP file.\n");
        return -1;
    }

    m_BMPHeader.imageSize=COMPONENTS * width*height+header_size;
    m_BMPHeader.startPosition=header_size;

    m_BMPInfoHeader.Length=sizeof(InfoHead);
    m_BMPInfoHeader.width=width;
    //BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
    m_BMPInfoHeader.height=-height;
    m_BMPInfoHeader.colorPlane=1;
    m_BMPInfoHeader.bitColor=24;
    //m_BMPInfoHeader.bitColor=32;
    m_BMPInfoHeader.realSize=COMPONENTS*width*height;

    fwrite(bfType,1,sizeof(bfType),fp_bmp);
    fwrite(&m_BMPHeader,1,sizeof(m_BMPHeader),fp_bmp);
    fwrite(&m_BMPInfoHeader,1,sizeof(m_BMPInfoHeader),fp_bmp);

    //BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
    //It saves pixel data in Little Endian
    //So we change 'R' and 'B'
    for(j =0;j<height;j++){
        for(i=0;i<width;i++){
            char temp=rgb24_buffer[(j*width+i)*COMPONENTS+2];
            rgb24_buffer[(j*width+i)*COMPONENTS+2]=rgb24_buffer[(j*width+i)*COMPONENTS+0];
            rgb24_buffer[(j*width+i)*COMPONENTS+0]=temp;

            // for rgb32
            /*
            char temp=rgb24_buffer[(j*width+i)*COMPONENTS+3];
            rgb24_buffer[(j*width+i)*COMPONENTS+3]=rgb24_buffer[(j*width+i)*COMPONENTS+0];
            rgb24_buffer[(j*width+i)*COMPONENTS+0]=temp;

            temp=rgb24_buffer[(j*width+i)*COMPONENTS+2];
            rgb24_buffer[(j*width+i)*COMPONENTS+2]=rgb24_buffer[(j*width+i)*COMPONENTS+1];
            rgb24_buffer[(j*width+i)*COMPONENTS+1]=temp;
            */
        }
    }
    fwrite(rgb24_buffer,COMPONENTS*width*height,1,fp_bmp);
    fclose(fp_bmp);
    printf("Finish generate %s!\n",bmppath);
    return 0;
}
