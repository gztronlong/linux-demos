#include <stdio.h>
#include <stdlib.h>

#define uchar unsigned char

typedef struct rectangle{
    int x;
    int y;
    int w;
    int h;
    uchar thin;
    uchar YUV[3];
}RECT;

void RgbToYuv(uchar RGB[], uchar YUV[]);
void FindAxis(RECT rect[], int n, int rect_all_axis[]);
void DrawWidthLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[]);
void DrawHeightLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[]);
void DrawPoint(uchar* pic, int pic_w, int pic_h, int point_axis[], int count, uchar YUV[]);
void NV12MarkRect(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int point_axis[], int count);

int main()
{
    /*****************************************************/
    /****                                             ****/
    /****             Configure Parameter             ****/
    /****                                             ****/
    /*****************************************************/

    /* Config the nv12's pixel size */
    const int pict_w  = 1920;
    const int pict_h  = 1080;
    int picSize = pict_w * pict_h * 1.5;

    /* Allocate memory for nv12 */
    uchar* picBuf = (unsigned char *)malloc(picSize);

    /* RGB convert to YUV */
    uchar RGBblue[3] = {255, 0, 0};
    uchar YUVblue[3];
    RgbToYuv(RGBblue, YUVblue);

    /* Config rectangle's quanlity and parameter */
    int n = 2;
    uchar t = 1;
    RECT Rect[] = {
        {
            .x = 0,
            .y = 0,
            .w = 96,
            .h = 96,
            .thin = t,
            .YUV[0] = YUVblue[0],
            .YUV[1] = YUVblue[1],
            .YUV[2] = YUVblue[2] },
        {
            .x = 144,
            .y = 144,
            .w = 128,
            .h = 128,
            .thin = t,
            .YUV[0] = YUVblue[0],
            .YUV[1] = YUVblue[1],
            .YUV[2] = YUVblue[2] }
    };

    /* Config point axis's count and draw the point at NV12 */
    int c = 30;
    int point[] = { 12,233, 132,15, 142,22, 312,34, 112,45, 212,58, 127,62, 132,72, 212,84, 129,91,
                    112,66, 187,38, 155,13, 145,56, 134,23, 165,56, 176,67, 189,87, 174,52, 121,99,
                    212,66, 287,38, 255,13, 245,56, 234,23, 265,56, 276,67, 289,87, 274,52, 221,99 };

    /* Config input and output path */
    char input[255]  = "videotestsrc_1920x1080.nv12";
    char output[255] = "outputFile.nv12";

    /******************************************************/

    FILE *fin  = fopen(input,  "rb");
    FILE *fout = fopen(output, "wb+");

    /* Read file data to buffer */
    fread(picBuf, 1, picSize, fin);

    /* Draw rectangle and point to nv12 */
    NV12MarkRect(picBuf, pict_w, pict_h, Rect, n, point, c);

    /* Write data of buf to fout */
    fwrite(picBuf, 1, picSize, fout);

    /* Free the allocation memory */
    free(picBuf);

    /* Close the file */
    fclose(fin);
    fclose(fout);

    return 0;
}

void RgbToYuv(uchar RGB[3], uchar YUV[3])
{
    /* RGB convert YUV */
    YUV[0] =  0.299  * RGB[0] + 0.587  * RGB[1] + 0.114  * RGB[2];
    YUV[1] = -0.1687 * RGB[0] + 0.3313 * RGB[1] + 0.5    * RGB[2] + 128;
    YUV[2] =  0.5    * RGB[0] - 0.4187 * RGB[1] - 0.0813 * RGB[2] + 128;
}

void FindAxis(RECT rect[], int n, int axis[])
{
    axis[0] = axis[2] = axis[4] = rect[n].x;
    axis[6] = rect[n].x + rect[n].w - rect[n].thin;
    axis[1] = rect[n].y;
    axis[3] = rect[n].y + rect[n].h - rect[n].thin;
    axis[5] = axis[7] = rect[n].y + rect[n].thin;
}

void DrawWidthLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[])
{
    int i, j, k;
    int y_index, u_index, v_index;
    for (i = 0; i < 4; i += 2){
        for (j = axis[i+1]+rect[n].thin-1; j >= axis[i+1]; j--){
            for (k = axis[i]+rect[n].w-1; k >= axis[i]; k--){

                y_index = j * pic_w + k;
                u_index = ((j >> 1) + pic_h) * pic_w + k - (k & 1);
                //u_index = (((y_index >> 1) - (pic_w >> 1) * ((j + 1) >> 1)) << 1) + pic_w * pic_h;
                v_index = u_index + 1;

                pic[y_index] = rect[n].YUV[0];
                pic[u_index] = rect[n].YUV[1];
                pic[v_index] = rect[n].YUV[2];

            }
        }
    }
}

void DrawHeightLine(uchar* pic, int pic_w, int pic_h, RECT rect[], int n, int axis[])
{
    int i, j, k;
    int y_index, u_index, v_index;
    for (i = 4; i < 8; i += 2){
        for (k = axis[i]+rect[n].thin-1; k >= axis[i]; k--){
            for (j = axis[i+1]+rect[n].h-(2*rect[n].thin)-1; j >= axis[i+1]-1; j--){
                y_index = j * pic_w + k;
                u_index = ((j >> 1) + pic_h) * pic_w + k - (k & 1);
                //u_index = (((y_index >> 1) - (pic_w >> 1) * ((j + 1) >> 1)) << 1) + pic_w * pic_h;
                v_index = u_index + 1;

                pic[y_index] = rect[n].YUV[0];
                pic[u_index] = rect[n].YUV[1];
                pic[v_index] = rect[n].YUV[2];

            }
        }
    }
}

void DrawPoint(uchar* pic, int pic_w, int pic_h, int point_axis[], int count, uchar YUV[])
{
    /* Draw points at NV12 */
    int a, y_point, u_point, v_point;
    for (a = (count-1)*2; a >= 0; a -= 2){

            y_point = point_axis[a+1] * pic_w + point_axis[a];
            u_point = ((point_axis[a+1] >> 1) + pic_h) * pic_w + point_axis[a] - (point_axis[a] & 1);
            //u_point = (((y_point >> 1) - (pic_w >> 1) * ((point_axis[a+1] + 1) >> 1)) << 1) + pic_w * pic_h;
            v_point = u_point + 1;

            pic[y_point] = YUV[0];
            pic[u_point] = YUV[1];
            pic[v_point] = YUV[2];
    }

}

void NV12MarkRect(uchar* pic, int pic_w, int pic_h, RECT rect[], int num, int point_axis[], int count)
{
    int i;
    int axis[8];

    for (i = num-1; i >= 0; i--){

        FindAxis(rect, i, axis);
        DrawWidthLine(pic, pic_w, pic_h, rect, i, axis);
        DrawHeightLine(pic, pic_w, pic_h, rect, i, axis);
    }

    DrawPoint(pic, pic_w, pic_h, point_axis, count, rect[0].YUV);
}
