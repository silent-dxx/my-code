/* xfb.c
 * Copyright (C) 2015, XFB Group
 * Auther: Silent_DXX & XYHF
 * Synopsis: SFB font function
 * E-mail: silent_dxx@163.com
 */

#include <stdio.h>
#include <string.h>
#include "font.h"
#include "draw.h"

//extern unsigned char fontdata_sun12x22[];

void putfont(char *vram, int xsize, int x1, int y1, int color, char font)
{
	int i;
	unsigned char *s1, *s2;
	unsigned int * p;
    int f = 0;

	for (i = font * 44; i < (font + 1) * 44;)
	{
		p = (unsigned int *)(vram) + x1 + (y1 + f++) * xsize;
		s1 = &(fontdata_sun12x22[i]);
		s2 = &(fontdata_sun12x22[i + 1]);

		if ((*s1 & 0x80) != 0) { p[0] = color; }
		if ((*s1 & 0x40) != 0) { p[1] = color; }
		if ((*s1 & 0x20) != 0) { p[2] = color; }
		if ((*s1 & 0x10) != 0) { p[3] = color; }
		if ((*s1 & 0x08) != 0) { p[4] = color; }
		if ((*s1 & 0x04) != 0) { p[5] = color; }
		if ((*s1 & 0x02) != 0) { p[6] = color; }
		if ((*s1 & 0x01) != 0) { p[7] = color; }

        if ((*s2 & 0x80) != 0) { p[8] = color; }
		if ((*s2 & 0x40) != 0) { p[9] = color; }
		if ((*s2 & 0x20) != 0) { p[10] = color; }
		if ((*s2 & 0x10) != 0) { p[11] = color; }

		i+=2;
	}
}

void putstr(char *vram, int xsize, int x1, int y1, int color, char *s)
{
	for (; *s != 0x00; s++) {
		putfont(vram, xsize, x1, y1, color, *s);
		x1 += 12;
	}
	return;
}

void putstr_f(struct SHEET *sht, int x, int y, int c, int b, char *s)
{
    int len = strlen(s);
    rectangle((char *)sht->buf, sht->bxsize, x, y, len * 12, 22, b);
    putstr((char *)sht->buf, sht->bxsize, x, y, c, s);
    sheet_refresh(sht, x, y, x + len * 12, 22);
}
