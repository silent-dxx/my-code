#pragma noce
#ifndef _DRAW_H_
#define _DRAW_H_
#include "fbtools.h"

extern inline int image(int x, int y);
extern inline int RGB(int r, int g, int b, int v);
extern inline void putpixel(unsigned char *vram, int xsize, int x1, int y1, int color);
extern void line(PFBDEV pFbdev, int x1, int y1, int x2, int y2);
extern void rectangle(unsigned char *vram, int xsize, int x0, int y0, int width, int height, int color);
extern void drawRect(unsigned char *vram, int xsize, int bpp, int x0, int y0, int width, int height, int color);
extern void show(char *vram, int xsize, int x0, int y0, char *filename);

#endif
