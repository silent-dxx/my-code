/*
 * File Name:    font.h
 * Author:       Silent_DXX
 * Mail:         <silent_dxx@163.com>
 * Created Time: 2015年06月01日 星期一 22时28分51秒
 */

#ifndef _FONT_H
#define _FONT_H
#include "sheet.h"

extern unsigned char fontdata_sun12x22[];

//void putfont(char *vram, int xsize, int x1, int y1, int color, char *font);
void putfont(char *vram, int xsize, int x1, int y1, int color, char font);
void putstr(char *vram, int xsize, int x1, int y1, int color, char *s);
void putstr_f(struct SHEET *sht, int x, int y, int c, int b, char *s);

#endif
