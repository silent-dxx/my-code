/*
 * File name : fbtools.h 
 */
#pragma noce
#ifndef _FBTOOLS_H_ 
#define _FBTOOLS_H_ 
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>

//a framebuffer device structure;
typedef struct fbdev {
	char           dev[20];                  //dev name
	int            fb;
	unsigned long  fb_mem_offset;
	unsigned long  fb_mem;                   //long mem add
	unsigned char* fb_ucmem;                 //char mem add
	int            bpp;                      //Save image digits(24 OR 32)
	int            line_length;              //The length of the screen real
	int            s_width, s_height;        //The screen size

	struct fb_fix_screeninfo fb_fix;
	struct fb_var_screeninfo fb_var;
} FBDEV, *PFBDEV;


//init a frame buffer
int fb_init(PFBDEV pFbdev);

//open a frame buffer
//to use this function,
//you must set FBDEV.dev="/dev/fb0"
//or "/dev/fbX"
//it's your frame buffer.
static int fb_open(PFBDEV pFbdev);

//close a frame buffer
int fb_close(PFBDEV pFbdev);

//get display depth
int get_display_depth(PFBDEV pFbdev);

//full screen clear
void fb_memset(void *addr, int c, size_t len);

#endif
//end
