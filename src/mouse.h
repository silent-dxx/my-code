/*
 * File Name:    mouse.h
 * Author:       Silent_DXX
 * Mail:         <silent_dxx@163.com>
 * Created Time: 2015年06月02日 星期二 22时16分37秒
 */

#ifndef _MOUSE_H
#define _MOUSE_H

struct MOUSE_INFO {
    char *vram;
    int xsize;
    int x, y, btn;
    int status;
};
typedef struct MOUSE_INFO MOUSE_INFO;

void *test(unsigned char *vram, int xsize, int a);
void *mymouse(MOUSE_INFO *mouse);
void putblock(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, int *buf, int bxsize);
void init_mouse_cursor(int *mouse, int bc);

#endif
