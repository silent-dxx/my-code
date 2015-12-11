#pragma noce
#ifndef _XFB_H_
#define _XFB_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>	
#include <ncurses.h>
#include <signal.h>
#include <pthread.h>

extern int            img_w, img_h;            //picture info
extern unsigned char  *img_buf;                //picture buf

extern int            fbfd;			           //Device file
extern long int       screensize;              //Tne screen size
extern int            bpp;                     //Save image digits(24 OR 32)
extern int            line_length;             //The length of the screen real
extern int            s_width, s_height;       //The screen size
extern unsigned char  *fbp;		               //The origin position

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

static void loop();

static void exit_handle(void);
static void sig_handle(int sid);

#endif
