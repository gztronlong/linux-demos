#include <stdio.h>
#include <stdlib.h>

int nv12_border(char *pic, int pic_w, int pic_h, int rect_x, int rect_y, int rect_w, int rect_h, int R, int G, int B)
{
    /* Set up the rectangle border size */
    const int border = 5;

    /* RGB convert YUV */
    int Y, U, V;
    Y =  0.299  * R + 0.587  * G + 0.114  * B;
    U = -0.1687 * R + 0.3313 * G + 0.5    * B + 128;
    V =  0.5    * R - 0.4187 * G - 0.0813 * B + 128;

    printf("%d %d %d\n", Y, U ,V);

    /* Locking the scope of rectangle border range */
    int j, k;
    for(j = rect_y; j < rect_y + rect_h; j++){
        for(k = rect_x; k < rect_x + rect_w; k++){
            if (k < (rect_x + border) || k > (rect_x + rect_w - border) ||\
                j < (rect_y + border) || j > (rect_y + rect_h - border)){

                /* Components of YUV's storage address index */
                int y_index = j * pic_w + k;
                int u_index = (y_index / 2 - pic_w / 2 * ((j + 1) / 2)) * 2 + pic_w * pic_h;
                int v_index = u_index + 1;

                /* set up YUV's conponents value of rectangle border */
                pic[y_index] =  Y ;
                pic[u_index] =  U ;
                pic[v_index] =  V ;
            }
        }
    }

    return 0;
}

int main()
{
    /* Set up the nv12's pixel size */
    const int pic_w  = 1920;
    const int pic_h  = 1080;

    char *input  = "videotestsrc_1920x1080.nv12";
    char *output = "outputFile.nv12";

    FILE *fin  = fopen(input,  "rb+");
    FILE *fout = fopen(output, "wb+");

    /* Allocate memory for nv12 */
    unsigned char *buf = (unsigned char *)malloc(pic_w * pic_h * 3 / 2);

    /* Read file data to buffer */
    fread(buf, 1, pic_w * pic_h * 3 / 2, fin);

    /* Draw rectangle border to nv12 */
    nv12_border(buf, pic_w, pic_h, 500, 500, 300, 400, 0, 0, 255);

    /* Write data of buf to fout */
    fwrite(buf, 1, pic_w * pic_h * 3 / 2, fout);

    /* Free the allocation memory */
    free(buf);

    /* Close the file */
    fclose(fin);
    fclose(fout);

    return 0;
}
