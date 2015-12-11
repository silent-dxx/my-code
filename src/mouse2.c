#include <stdio.h>
#include <stdlib.h> 
#include <linux/input.h> 
#include <fcntl.h> 
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <pthread.h>
#include "mouse.h"
#include "draw.h"
#include "font.h"

struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};

void *mymouse(MOUSE_INFO *mouse)
{
    //struct MOUSE_INFO *mouse = (struct MOUSE_INFO *)arg;
	int fd, retval; 
	signed char buf[6];
 	fd_set readfds;
 	struct timeval tv;

	int m_x = mouse->x, m_y = mouse->y;     //鼠标的初始位置
	int o_x = mouse->x, o_y = mouse->y;

	char s[40];

	int mcursor[256];
	init_mouse_cursor(mcursor, 0xff008484);

 	// 打开鼠标设备
 	// 判断是否打开成功
	if((fd = open("/dev/input/mice", O_RDONLY)) < 0) {
		//printf("Failed to open \"/dev/input/mice\".\n");
 		exit(1);
 	} else {
 		//printf("open \"/dev/input/mice\" successfuly.\n");
		putblock(mouse->vram, mouse->xsize, 16, 16, m_x, m_y, mcursor, 16);
 	}
	while (1) {
		// 设置最长等待时间 
		tv.tv_sec = 10; 
		tv.tv_usec = 0; 
		FD_ZERO(&readfds); 
		FD_SET(fd, &readfds); 
		retval = select(fd + 1, &readfds, NULL, NULL, &tv); 

		if (retval == 0) { 
			//printf( "Time out!\n" ); 
			//rectangle(0,0,10,10,0,255,0,255);
		}
 		if(FD_ISSET(fd, &readfds)) {
			// 读取鼠标设备中的数据 
			if(read(fd, buf, 6) <= 0) { 
				continue;
 			} 
			// 打印出从鼠标设备中读取到的数据
 			//printf("Button type = %d, X = %d, Y = %d, Z = %d\n", (buf[0] & 0x07), buf[1], buf[2], buf[3]);
			m_x += buf[1]; m_y -= buf[2];
            if (m_x < 0) m_x = 0;
            if (m_y < 0) m_y = 0;
            if (m_x > 1366 - 16) m_x = 1366 - 16;
            if (m_y > 768 - 16) m_y = 768 - 16;

			if((buf[0] & 0x07) == 1) {  //左键

			    //sprintf(s, "(%d, %d)", m_x, m_y);
			    rectangle(mouse->vram, mouse->xsize, 300, 30, 20, 22, 0xff008484);
			    putstr(mouse->vram, mouse->xsize, 300, 30, 0xff000084, "L");
			}
			if((buf[0] & 0x07) == 2) {  //右键

			    rectangle(mouse->vram, mouse->xsize, 300, 30, 20, 22, 0xff008484);
			    putstr(mouse->vram, mouse->xsize, 300, 30, 0xff000084, "R");
			}
			if((buf[0] & 0x07) == 4) {  //中键

			    rectangle(mouse->vram, mouse->xsize, 300, 30, 20, 22, 0xff008484);
			    putstr(mouse->vram, mouse->xsize, 300, 30, 0xff000084, "C");
			}

			sprintf(s, "(%d, %d)", m_x, m_y);
			rectangle(mouse->vram, mouse->xsize, 20, 30, 200, 22, 0xff008484);
			putstr(mouse->vram, mouse->xsize, 20, 30, 0xff000084, s);



			rectangle(mouse->vram, mouse->xsize, o_x, o_y, 16, 16, 0xff008484);
			putblock(mouse->vram, mouse->xsize, 16, 16, m_x, m_y, mcursor, 16);

			o_x = m_x; o_y = m_y;
 		} 
	} 

	close(fd);

	pthread_exit((void *)1);
}

void init_mouse_cursor(int *mouse, int bc)
{
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = 0xff000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = 0xffffffff;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, int *buf, int bxsize)
{
	int *dest = (int *) (vram) + px0 + py0 * vxsize;
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			dest[x] = buf[y * bxsize + x];
		}
		dest += vxsize;
	}
	return;
}
