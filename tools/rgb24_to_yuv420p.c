#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


static void rgb_to_yuv420(int w, int h, uint8_t *rgb, uint8_t *yuv)
{
    int pixsize;
    int pixIndex;
    uint8_t *y,*u,*v;
    int i,j;

    pixsize= w * h;                       //图像大小
    y = yuv;                              //yuv420P y得起始地址
    u = yuv + pixsize;                    //yuv420P u得起始地址
    v = u + pixsize/4;                    //yuv420P v得起始地址

    for (i = 0; i < h; i++) {

        for (j = 0; j < w; j++) {
            pixIndex = i * 3 * w + j * 3;
            int nr = rgb[pixIndex];                 //获取每个像素点得r,g,b值
            int ng = rgb[pixIndex+1];
            int nb = rgb[pixIndex+2];

            *y++ = (uint8_t)( ( 66 * nr + 129 * ng +  25 * nb + 128) >> 8) + 16 ;

            if ((i%2==1) && (j%2==1)) {
                *u++ = (uint8_t)( ( -38 * nr -  74 * ng + 112 * nb + 128) >> 8) + 128 ;
                *v++ = (uint8_t)( ( 112 * nr -  94 * ng -  18 * nb + 128) >> 8) + 128 ;
            }
        }
    }

    return;
 }

/*********************************************************************
/* Function: bgr24_to_yuv420p
/* Usage: colorspace convertation from BGR24 to YUV420P(I420,YU12)
/* Parameters:
/*  pBGR24  [In]        - source RGB data
/*  pYUV    [Out]       - output YUV data
/*  width   [In]        - image width
/*  height  [In]        - image height
/*
/* Modified: 2019.7.13
/*********************************************************************/
int bgr24_to_yuv420p(unsigned char *pBGR24, unsigned char *pYUV, int width, int height)
{
#define CLIP_COLOR(x) ((unsigned char)((x<0)?0:((x>255)?255:x)))
    int i,j,index;
    unsigned char *bufY, *bufU, *bufV, *bufRGB;
    unsigned char y, u, v, r, g, b;

    if(!pYUV || !pBGR24 || width < 1 || height < 1)

        return -1;

    memset(pYUV, 0 ,width*height*3/2*sizeof(unsigned char));

    bufY = pYUV;
    bufU = bufY + width*height;
    bufV = bufU + (width*height*1/4);


    for(j=0; j < height; j++)
    {
        bufRGB = pBGR24 + width*j*3;
        for(i=0; i < width; i++)
        {
            index = j*width+i;
            b = *(bufRGB++);
            g = *(bufRGB++);
            r = *(bufRGB++);

            y = (unsigned char)((66  * r + 129 * g + 25  * b + 128) >> 8) + 16;
            u = (unsigned char)((-38 * r - 74  * g + 112 * b + 128) >> 8) + 128;
            v = (unsigned char)((112 * r - 94  * g - 18  * b + 128) >> 8) + 128;

            *(bufY++) = CLIP_COLOR(y);
            if(j%2==0 && index%2==0)
            {
                *(bufU++) = CLIP_COLOR(u);
                *(bufV++) = CLIP_COLOR(v);
            }
        }
    }
    return 0;
}
#define PER_PIXEL_BYTES_BMP_24BIT             3
int pic_rgb24bit_file_to_yuv420p(const char *file_path, const char * output_path, int width, int heigh)
{
    FILE *fp;
    int rc;
    uint8_t *bmp_buf = NULL;
    uint8_t *buf = NULL;
    int flen = 0;
    int ret = -1;
    int total_length = 0;
    uint8_t *r_buff;
    uint32_t r_size;

    if(file_path == NULL)
    {
        printf("file_path Error,return");
        return -1;
    }
    printf("img path = %s\n", file_path);
    fp = fopen( file_path, "rb" );
    if(fp == NULL){
        printf("load cursor file open failed");
        return -1;
    }

    /* 求解文件长度 */
    fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);

    flen = ftell(fp);

    //bmp_buf = (uint8_t*)calloc(1,flen - 54);
    bmp_buf = (uint8_t*)calloc(1,flen);
    if(bmp_buf == NULL){
        printf("load > malloc bmp out of memory!\n");
        return -1;
    }

    /* 再移位到文件头部 */
    fseek(fp,0,SEEK_SET);


    total_length = width * heigh * PER_PIXEL_BYTES_BMP_24BIT;

    printf("file info:\n");
    printf("\t image size:\t%d\n", flen);               //图片大小


    //读取bmp数据
    buf = bmp_buf;
    while ((ret = fread(buf,1,total_length,fp)) >= 0) {
        if (ret == 0) {
            break;
        }
        printf("\t read bytes:\t%d\n", ret);
        buf = ((uint8_t*) buf) + ret;
        total_length = total_length - ret;
        if(total_length == 0)
            break;
    }

#if 1
    r_buff =  (uint8_t*)calloc(1, width*heigh*3/2*sizeof(unsigned char));

    bgr24_to_yuv420p(bmp_buf, r_buff, width, heigh);
    //rgb_to_yuv420(1920, 1080, bmp_buf, r_buff);

    printf("OK\n");
    FILE *yuvFile = fopen(output_path, "wb");
    fwrite(r_buff, width * heigh *3 / 2, 1, yuvFile);
    ////bmp倒叙转换到数组
    //cursor_bitmap_format_convert_for_bmp_24bit(r_buff, bmp_buf);

    //内存释放
    free(bmp_buf);
#endif
    fclose(fp);
    return 0;
}



int main(int argc, char *argv[])
{
    if(argc == 5)
    {
        pic_rgb24bit_file_to_yuv420p(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
    }else
    {
        printf("arg 1: rgb input  file\n");
        printf("arg 2: yuv output file\n");
        printf("arg 3: width\n");
        printf("arg 4: heigh\n");
    }
    //pic_rgb24bit_file_to_yuv420p("lt9211.1080p30.fpga.rgb24", "lt9211.fpga.1080p30.yuv", 1920, 1080);
    //pic_rgb24bit_file_to_yuv420p("lt9211.tp.rgb24", "lt9211.yuv420p", 1920, 1080);
    return 0;
}
