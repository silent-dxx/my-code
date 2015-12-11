#include <stdlib.h>
#include <linux/fb.h>
#include <unistd.h>
#include "draw.h"
#include "jpeg.h"
#include "xfb.h"
#include "types.h"

inline int RGB(int r, int g, int b, int v)
{
    return v << 24 | r << 16 | g << 8 | b;
}

inline void putpixel(unsigned char *vram, int xsize, int x1, int y1, int color)
{

	//unsigned int *start = (unsigned int *)(vram + x1 * 4 + y1 * xsize);
	//*(start) = color;

	//*((unsigned int *)(vram + x1 * 4 + y1 * xsize * 4)) = color;
	*((unsigned int *)(vram) + x1 + y1 * xsize) = color;

}

void line(PFBDEV pFbdev, int x1, int y1, int x2, int y2)
{
 
	int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1; 
	int err = (dx > dy ? dx : -dy) / 2, e2;
 
	for(;;) {
        //draw_pixel_img(x1, y1);

        if (x1==x2 && y1==y2) break;

        e2 = err;

        if (e2 >-dx) { err -= dy; x1 += sx; }
		if (e2 < dy) { err += dx; y1 += sy; }
    }
}

void rectangle(unsigned char *vram, int xsize, int x0, int y0, int width, int height, int color)
{
    //const int bytesPerPixel = 4;

	int *dest = (int *) (vram + x0 * 4 + y0 * xsize * 4);

    int x, y;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            dest[x] = color;
        }
        dest += xsize;
    }
}

//画大小为width*height的同色矩阵，8alpha+8reds+8greens+8blues
void drawRect_rgb32(unsigned char *vram, int xsize, int x0, int y0, int width, int height, int color)
{
    const int bytesPerPixel = 4;
    //const int stride = pFbdev->fb_fix.line_length / bytesPerPixel;

	//unsigned char *start = (unsigned char *)(pFbdev->fb_ucmem + x1 * pFbdev->bpp + y1 * pFbdev->line_length);
    //int *dest = (int *) (pFbdev->fb_ucmem) + (y0 + pFbdev->fb_var.yoffset) * stride + (x0 + pFbdev->fb_var.xoffset);
	int *dest = (int *) (vram + x0 * 4 + y0 * xsize * 4);

    int x, y;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            dest[x] = color;
        }
        dest += xsize;
    }
}

//画大小为width*height的同色矩阵，5reds+6greens+5blues
void drawRect_rgb16(unsigned char *vram, int xsize, int x0, int y0, int width, int height, int color)
{
    const int bytesPerPixel = 2;
    //const int stride = finfo.line_length / bytesPerPixel;
    const int red = (color & 0xff0000) >> (16 + 3);
    const int green = (color & 0xff00) >> (8 + 2);
    const int blue = (color & 0xff) >> 3;
    const short color16 = blue | (green << 5) | (red << (5 + 6));

    //short *dest = (short *) (pFbdev->fb_ucmem) + (y0 + pFbdev->fb_var.yoffset) * stride + (x0 + pFbdev->fb_var.xoffset);
	short *dest = (short *) (vram + x0 * 4 + y0 * xsize * 4);

    int x, y;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            dest[x] = color16;
        }
        dest += xsize;
    }
}

//画大小为width*height的同色矩阵，5reds+5greens+5blues
void drawRect_rgb15(unsigned char *vram, int xsize, int x0, int y0, int width, int height, int color)
{
	const int bytesPerPixel = 2;
	//const int stride = finfo.line_length / bytesPerPixel;
    const int red = (color & 0xff0000) >> (16 + 3);
    const int green = (color & 0xff00) >> (8 + 3);
    const int blue = (color & 0xff) >> 3;
    const short color15 = blue | (green << 5) | (red << (5 + 5)) | 0x8000;

    //short *dest = (short *) (pFbdev->fb_ucmem) + (y0 + pFbdev->fb_var.yoffset) * stride + (x0 + pFbdev->fb_var.xoffset);
	short *dest = (short *) (vram + x0 * 4 + y0 * xsize * 4);

    int x, y;
    for (y = 0; y < height; ++y) {
        for (x = 0; x < width; ++x) {
            dest[x] = color15;
        }
        dest += xsize;
    }
}

void drawRect(unsigned char *vram, int xsize, int bpp, int x0, int y0, int width, int height, int color)
{
    switch (bpp) {
    case 32:
        drawRect_rgb32(vram, xsize, x0, y0, width, height, color);
        break;
    case 16:
        drawRect_rgb16(vram, xsize, x0, y0, width, height, color);
        break;
    case 15:
        drawRect_rgb15(vram, xsize, x0, y0, width, height, color);
        break;
    default:
        printf ("Warning: drawRect() not implemented for color depth %i\n", vinfo.bits_per_pixel);
        break;
    }
}

void show(char *vram, int xsize, int x0, int y0, char *filename)                       //快速显示图片
{
    int            img_w, img_h;            //picture info
    unsigned char  *img_buf;                //picture buf
	int x, y;
    int x1, y1;
    unsigned int *buf;

    img_buf = read_jpeg_file(filename, &img_w, &img_h);

	char *dest;
	//int *dest = (int *) (vram + x0 * 4 + y0 * xsize * 4);
    //int *buf = (int *)img_buf;
    int color;
    if (img_w + x0 > xsize) 
        y1 = img_w + x0;
    else
        y1 = img_w + x0;

    for (y = y0; y < img_h + y0; y++) {
        for (x = x0; x < y1; x++) {
            //dest = img_buf + x * 3 + 2 * y * img_w;
            color = RGB(img_buf[0], img_buf[1], img_buf[2], 255);
            //color = RGB(dest[0], dest[1], dest[2], 255);
            if (x < xsize) putpixel(vram, xsize, x, y, color);
            img_buf+=3;
        }
        //dest += xsize;
    }
}

