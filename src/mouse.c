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

void *mymouse(MOUSE_INFO *mouse)
{
	int fd, retval; 
	signed char buf[6];
 	fd_set readfds;
 	struct timeval tv;
    
    extern pthread_mutex_t mtx;

    int m_x = mouse->x, m_y = mouse->y;     //mouse x and y
	int o_x = mouse->x, o_y = mouse->y;

 	// open mouse dev
 	// check mouse dev
	if((fd = open("/dev/input/mice", O_RDONLY)) < 0) {
		//printf("Failed to open \"/dev/input/mice\".\n");
 		exit(1);
 	}

	while (1) {
        //setting time
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
            // read mouse dev 
            if(read(fd, buf, 6) <= 0) { 
                continue;
            }
 			//printf("Button type = %d, X = %d, Y = %d, Z = %d\n", (buf[0] & 0x07), buf[1], buf[2], buf[3]);
            m_x += buf[1]; m_y -= buf[2];
            if (m_x < 0) m_x = 0;
            if (m_y < 0) m_y = 0;
            if (m_x > 1366 - 1) m_x = 1366 - 1;
            if (m_y > 768 - 1) m_y = 768 - 1;

            mouse->status = 1;
            mouse->x = m_x;
            mouse->y = m_y;
            mouse->btn = buf[0] & 0x07;
 		} 
	} 

	close(fd);
	pthread_exit((void *)1);
}

void init_mouse_cursor(int *mouse, int bc)
{
    static char cursor2[16][16] = {
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
    static char cursor[21][12] = {
        "*...........",
	    "**..........",
	    "*O*.........",
	    "*OO*........",
	    "*OOO*.......",
	    "*OOOO*......",
	    "*OOOOO*.....",
	    "*OOOOOO*....",
	    "*OOOOOOO*...",
	    "*OOOOOOOO*..",
	    "*OOOOOOOOO*.",
	    "*OOOOOO*****",
	    "*OOO*OO*....",
	    "*OO**OO*....",
	    "*O*..*OO*...",
	    "**...*OO*...",
	    "*.....*OO*..",
	    "......*OO*..",
	    ".......*OO*.",
	    ".......*OO*.",
	    "........**..",
    };
    int x, y;

    for (y = 0; y < 21; y++) {
        for (x = 0; x < 12; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 12 + x] = 0xff000000;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 12 + x] = 0xffffffff;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 12 + x] = bc;
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
