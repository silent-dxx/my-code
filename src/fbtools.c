/*
    File name : fbtools.c 
*/
#pragma noce
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "types.h"
#include "fbtools.h"


//init a frame buffer
int fb_init(PFBDEV pFbdev)
{
 	memset(pFbdev, 0, sizeof(FBDEV));
	strcpy(pFbdev->dev, "/dev/fb0");
	if (fb_open(pFbdev) == 0) {
		perror("open frame buffer error");
		return FALSE;
	}
	fb_memset((void *)(pFbdev->fb_mem + pFbdev->fb_mem_offset), 0, pFbdev->fb_fix.smem_len);
	return TRUE;
}

//open a frame buffer
int fb_open(PFBDEV pFbdev)
{
    unsigned long page_mask;
    pFbdev->fb = open(pFbdev->dev, O_RDWR);
    if (pFbdev->fb < 0) {
        fprintf(stderr, "Error opening %s: %m. Check kernel config\n", pFbdev->dev);
        return FALSE;
    }
    if (-1 == ioctl(pFbdev->fb,FBIOGET_VSCREENINFO, &(pFbdev->fb_var))) { 
        fprintf(stderr, "ioctl FBIOGET_VSCREENINFO\n");
        return FALSE;
    } 
    if (-1 == ioctl(pFbdev->fb,FBIOGET_FSCREENINFO, &(pFbdev->fb_fix))) {
        fprintf(stderr, "ioctl FBIOGET_FSCREENINFO\n");
        return FALSE;
    }
    //map physics address to virtual address
    //pFbdev->fb_mem_offset = (unsigned long)(pFbdev->fb_fix.smem_start) & (~PAGE_MASK);
    page_mask = getpagesize() - 1;
    pFbdev->fb_mem_offset = (unsigned long)(pFbdev->fb_fix.smem_start) & page_mask;
    pFbdev->fb_mem = (unsigned long int)mmap(NULL, pFbdev->fb_fix.smem_len + pFbdev->fb_mem_offset, \
                        PROT_READ | PROT_WRITE, MAP_SHARED, pFbdev->fb, 0); 
	pFbdev->fb_ucmem = (unsigned char *)pFbdev->fb_mem;
    if (-1L == (long) pFbdev->fb_mem) {
        fprintf(stderr, "mmap error! mem:%d offset:%d\n", (int)pFbdev->fb_mem, (int)pFbdev->fb_mem_offset);
        return FALSE;
    }
	pFbdev->bpp = pFbdev->fb_var.bits_per_pixel / 8;            //Save image digits(24 OR 32)
	pFbdev->line_length = pFbdev->fb_fix.line_length;       //The length of the screen real
	pFbdev->s_width = pFbdev->fb_var.xres;                  //The screen size
	pFbdev->s_height = pFbdev->fb_var.yres;

	printf("bpp = %d\n", pFbdev->bpp);
	printf("line_length = %d\n", pFbdev->line_length);
	printf("s_width = %d\n", pFbdev->s_width);
	printf("s_height = %d\n", pFbdev->s_height);
    return TRUE;
}

//close frame buffer
int fb_close(PFBDEV pFbdev)
{
	munmap(&(pFbdev->fb_mem), pFbdev->fb_fix.smem_len);
    close(pFbdev->fb);
    pFbdev->fb=-1;
}

//get display depth
int get_display_depth(PFBDEV pFbdev)
{
        if(pFbdev->fb<=0) {
              fprintf(stderr, "fb device not open, open it first\n");
              return FALSE;
        }
        return pFbdev->fb_var.bits_per_pixel;
}

//full screen clear
void fb_memset (void *addr, int c, size_t len)
{
#if 1
    unsigned int i, *p;
    i = (c & 0xff) << 8;
    i |= i << 16;
    len >>= 2;
    for (p = addr; len--; p++)
        *p = i;
#else
    memset(addr, c, len);
#endif
}

