/* xfb.c
 * Copyright (C) 2015, XFB Group
 * Auther: Silent_DXX & XYHF
 * Synopsis: SFB mian function
 * E-mail: silent_dxx@163.com
 */

#include <sys/time.h>
#include "types.h"
#include "xfb.h"
#include "draw.h"
#include "jpeg.h"
#include "class.h"
#include "fbtools.h"
#include "font.h"
#include "mouse.h"
#include "keyboard.h"
#include "sheet.h"

int fps = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void prompt_info(int signe)
{
    //write(STDERR_FILENO, msg, len);
    //printf("run time: %d s\n",fps);
    char fps_fp[1000];
    //rectangle(0, 580, 350, 130, 0, 0, 0, 0);
    //pclear();
    sprintf(fps_fp, "%d", fps);
    //print_f(fps_fp, 0, 580, 0);
    fps = 0;
}

void init_sigaction(void)
{
    struct sigaction tach;
    tach.sa_handler = prompt_info;
    tach.sa_flags = 0;

    sigemptyset(&tach.sa_mask);
    sigaction(SIGALRM, &tach, NULL);
}

void init_time()
{
    struct itimerval value;

    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;

    value.it_interval = value.it_value;
    
    setitimer(ITIMER_REAL, &value, NULL);
}

void init_screen(int *vram, int x, int y)
{
    //rectangle((char *)vram, x,  0,     0,      x - 1,  y - 28, 0xff008484);
    
    //rectangle((char *)vram, x,  0,     y - 28, x - 1 , 28,     0xffc6c6c6);
    //rectangle((char *)vram, x,  0,     y - 27, x - 1 , 27,     0xffffffff);
    //rectangle((char *)vram, x,  0,     y - 26, x - 1 , 26,     0xffc6c6c6);
#if 0
    rectangle((char *)vram, x,  3,     y - 24, 59,      1,     0xffffffff);
    rectangle((char *)vram, x,  2,     y - 24,  1,     23,     0xffffffff);
    rectangle((char *)vram, x,  3,     y -  4, 59,      1,     0xff848484);
    rectangle((char *)vram, x, 61,     y - 23,  1,     20,     0xff848484);
    rectangle((char *)vram, x,  2,     y - 3,  60,      1,     0xff000000);
    rectangle((char *)vram, x, 62,     y - 23,  1,     21,     0xff000000);
#else 
    rectangle((char *)vram, x,  0,     0,      x - 1,  y - 28, 0x00000001);
    
    rectangle((char *)vram, x,  0,     y - 28, x - 1 , 28,     0xffc6c6c6);
    rectangle((char *)vram, x,  0,     y - 27, x - 1 , 27,     0xffffffff);
    rectangle((char *)vram, x,  0,     y - 26, x - 1 , 26,     0xffc6c6c6);

    rectangle((char *)vram, x,  3,     y - 24, 59,      1,     0xffffffff);
    rectangle((char *)vram, x,  2,     y - 24,  1,     23,     0xffffffff);
    rectangle((char *)vram, x,  3,     y -  4, 59,      1,     0xff848484);
    rectangle((char *)vram, x, 61,     y - 23,  1,     20,     0xff848484);
    rectangle((char *)vram, x,  2,     y - 3,  60,      1,     0xff000000);
    rectangle((char *)vram, x, 62,     y - 23,  1,     21,     0xff000000);
#endif
}

void make_window(int *buf, int xsize, int ysize, char *title)
{
	static char closebtn[14][16] = {
		"OOOOOOOOOOOOOOO#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQ@@QQQQ@@QQ$#",
		"OQQQQ@@QQ@@QQQ$#",
		"OQQQQQ@@@@QQQQ$#",
		"OQQQQQQ@@QQQQQ$#",
		"OQQQQQ@@@@QQQQ$#",
		"OQQQQ@@QQ@@QQQ$#",
		"OQQQ@@QQQQ@@QQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"O$$$$$$$$$$$$$$#",
		"################"
	};
	static char minbtn[14][16] = {
		"OOOOOOOOOOOOOOO#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQ@@@@@@QQQQ$#",
		"OQQQ@@@@@@QQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQQQQQQQQQQQ$#",
		"O$$$$$$$$$$$$$$#",
		"################"
	};
	static char maxbtn[14][16] = {
		"OOOOOOOOOOOOOOO#",
		"OQQQQQQQQQQQQQ$#",
		"OQQQ@@@@@@@@QQ$#",
		"OQQQ@@@@@@@@QQ$#",
		"OQQQ@QQQQQQ@QQ$#",
		"OQQQ@QQQQQQ@QQ$#",
		"OQQQ@QQQQQQ@QQ$#",
		"OQQQ@QQQQQQ@QQ$#",
		"OQQQ@QQQQQQ@QQ$#",
		"OQQQ@QQQQQQ@QQ$#",
		"OQQQ@@@@@@@@QQ$#",
		"OQQQQQQQQQQQQQ$#",
		"O$$$$$$$$$$$$$$#",
		"################"
	};
	int x, y;
	int c;
#if 1
	rectangle((char *)buf, xsize,         0,         0, xsize,     ysize    , 0x00D4D0C8);

	rectangle((char *)buf, xsize,         1,         1, xsize - 2, 1        , 0x00FFFFFF);  //Top
	rectangle((char *)buf, xsize,         1,         1,         1, ysize - 2, 0x00FFFFFF);  //Left

	rectangle((char *)buf, xsize,         1, ysize - 2, xsize - 2,         1, 0x00808080);  //Down
	rectangle((char *)buf, xsize,         1, ysize - 1, xsize - 2,         1, 0x00404040);

	rectangle((char *)buf, xsize, xsize - 2,         1,         1, ysize - 1, 0x00808080);  //Right
	rectangle((char *)buf, xsize, xsize - 1,         1,         1, ysize - 1, 0x00404040);

	rectangle((char *)buf, xsize,         4,         4, xsize - 8,        20, 0x00000084);  //Title
#else
	rectangle((char *)buf, xsize,  0,         0,         xsize - 1, 0        , 0xffc6c6c6);
	rectangle((char *)buf, xsize,  1,         1,         xsize - 2, 1        , 0xffFFFFFF);
	rectangle((char *)buf, xsize,  0,         0,         0,         ysize - 1, 0xffC6C6C6);
	rectangle((char *)buf, xsize,  1,         1,         1,         ysize - 2, 0xffFFFFFF);
	rectangle((char *)buf, xsize,  xsize - 2, 1,         xsize - 2, ysize - 2, 0xff848484);
	rectangle((char *)buf, xsize,  xsize - 1, 0,         xsize - 1, ysize - 1, 0xff00FFFF);
	rectangle((char *)buf, xsize,  2,         2,         xsize - 3, ysize - 3, 0xffC6C6C6);
	rectangle((char *)buf, xsize,  3,         3,         xsize - 5, 20       , 0xff000084);
	//rectangle((char *)buf, xsize,  1,         ysize - 2, xsize - 2, ysize - 2, 0xff848484);
	//rectangle((char *)buf, xsize,  0,         ysize - 1, xsize - 1, ysize - 1, 0xff000000);
#endif
	putstr((char *)buf, xsize, 10, 5, 0xffffffff, title);
	for (y = 0; y < 14; y++) {
		for (x = 0; x < 16; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = 0x00000000;
			} else if (c == '$') {
				c = 0x00808080;
			} else if (c == 'Q') {
				c = 0x00D4D0C8;
			} else if (c == '#') {
				c = 0x00404040;
			} else {
				c = 0x00FFFFFF;
			}
			//buf[(5 + y) * xsize + (xsize - 21 + x)] = c;
			buf[(7 + y) * xsize + (xsize - 22 + x)] = c;
		}
	}

	for (y = 0; y < 14; y++) {
		for (x = 0; x < 16; x++) {
			c = minbtn[y][x];
			if (c == '@') {
				c = 0x00000000;
			} else if (c == '$') {
				c = 0x00808080;
			} else if (c == 'Q') {
				c = 0x00D4D0C8;
			} else if (c == '#') {
				c = 0x00404040;
			} else {
				c = 0x00FFFFFF;
			}
			buf[(7 + y) * xsize + (xsize - 56 + x)] = c;
		}
	}

	for (y = 0; y < 14; y++) {
		for (x = 0; x < 16; x++) {
			c = maxbtn[y][x];
			if (c == '@') {
				c = 0x00000000;
			} else if (c == '$') {
				c = 0x00808080;
			} else if (c == 'Q') {
				c = 0x00D4D0C8;
			} else if (c == '#') {
				c = 0x00404040;
			} else {
				c = 0x00FFFFFF;
			}
			buf[(7 + y) * xsize + (xsize - 40 + x)] = c;
		}
	}

	return;
}

int main(int argc, char argv[])
{
	atexit(exit_handle);
	signal(SIGALRM, sig_handle);	/* 定时器 */
	signal(SIGHUP, sig_handle);	    /* 注销时 */
	signal(SIGINT, sig_handle);	    /* Ctrl+C */
	signal(SIGQUIT, sig_handle);	/* Ctrl+\ */
	signal(SIGTSTP, sig_handle);	/* Ctrl+Z */
	signal(SIGTERM, sig_handle);	/* 被结束时 */

/*
    init_sigaction();               // timer 
    init_time();
*/


    int resys;
    //resys = system("cp /dev/fb0 ./x");
    printf("\033[?25l");             //hide
	//printf("\033[?25h");           //show 

    struct SHTCTL *shtctl;
    struct SHEET *sht_back, *sht_task, *sht_mouse, *sht_win, *sht_win2, *sht_win3;
    unsigned int *buf_back, *buf_task;
    unsigned int *buf_win, *buf_win2, *buf_win3;
    int buf_mouse[252];
	static char keytable[0x54] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   '\\', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.'
	};

    int xx = 30;
    int cursor_x = 0;

	FBDEV fbdev;

	fb_init(&fbdev);                 //Initialize.....

    resys = system("cp /dev/fb0 ./x");

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);

    MOUSE_INFO mouse;
    mouse.vram = fbdev.fb_ucmem;
    mouse.xsize = fbdev.s_width + 10;
    mouse.x = 200;
    mouse.y = 300;
    mouse.status = 0;

    KEY_INFOV key;
    key.status = 0;
    
    shtctl = shtctl_init(fbdev.fb_ucmem, fbdev.s_width + 10, fbdev.s_height);

    sht_back = sheet_alloc(shtctl);
    sht_task = sheet_alloc(shtctl);
    sht_mouse = sheet_alloc(shtctl);
    sht_win = sheet_alloc(shtctl);
    sht_win2 = sheet_alloc(shtctl);
    sht_win3 = sheet_alloc(shtctl);

    buf_back = (unsigned int *) malloc(1376 * 768 * 4);
    buf_task = (unsigned int *) malloc(1376 * 768 * 4);
    buf_win = (unsigned int *) malloc(480 * 320 * 4);
    buf_win2 = (unsigned int *) malloc(200 * 100 * 4);
    buf_win3 = (unsigned int *) malloc(200 * 200 * 4);

    sheet_setbuf(sht_back, (char *)buf_back, 1376, 768, 1);
    sheet_setbuf(sht_task, (char *)buf_task, 1376, 768, 1);
    sheet_setbuf(sht_mouse, (char *)buf_mouse, 12, 21, 99);
    sheet_setbuf(sht_win, (char *)buf_win, 480, 320, 1);
    sheet_setbuf(sht_win2, (char *)buf_win2, 200, 100, 1);
    sheet_setbuf(sht_win3, (char *)buf_win3, 200, 200, 1);

    init_screen(buf_task, 1376, 768);
	init_mouse_cursor(buf_mouse, 99);

    make_window(buf_win, 480, 320, "windows");
    make_window(buf_win2, 200, 100, "move");
    make_window(buf_win3, 200, 200, "^-^");

    sheet_slide(sht_back, 0, 0);
    sheet_slide(sht_task, 0, 0);
    sheet_slide(sht_mouse, mouse.x, mouse.y);
    sheet_slide(sht_win, 100, 150);
    sheet_slide(sht_win2, 30, 250);
    sheet_slide(sht_win3, 300, 400);

    sheet_updown(sht_back, 0);
    sheet_updown(sht_task, 250);
    sheet_updown(sht_mouse, 255);
    sheet_updown(sht_win, 1);
    sheet_updown(sht_win2, 2);
    sheet_updown(sht_win3, 3);
    

	putstr((char *)buf_task, fbdev.s_width + 10, 1300, 747, 0x00000084, "12:24");
	putstr((char *)buf_task, fbdev.s_width + 10, 2, 746, 0x00000084, "start");

	putstr((char *)buf_win2, sht_win2->bxsize, 40, 60, 0x00000084, "move test");

    show(sht_back->buf, 1376, 0, 0, "img/img_2.jpg");
    show(sht_win3->buf, 200, 4, 26, "img/2222.jpg");
    //sheet_refresh(sht_win3, 0, 0, 1376, 768);
	putstr((char *)buf_back, fbdev.s_width + 10, fbdev.s_width / 2 - 100, fbdev.s_height / 2, \
            0xffffffff, "Hello XFB-OS");

    sheet_refresh(sht_back, 0, 0, 1376, 768);
    sheet_refresh(sht_task, 0, 0, 1376, 768);
    sheet_refresh(sht_win3, 0, 0, 200, 200);

    int        ret = 0;
    pthread_t  id1, id2;
    void       *tret;

    if ((ret = pthread_create(&id1, NULL, (void*)mymouse, &mouse))) {
        printf("Create pthread error!\n");
    }

    if ((ret = pthread_create(&id2, NULL, (void*)keyboard, &key))) {
        printf("Create pthread error!\n");
    }

    char s[40];
    int o_x = mouse.x, o_y = mouse.y;
    int count = 0;

    unsigned int x = 0;
    int k = 0;

    while (1) {
        count++;
        sprintf(s, "[%d]", count);
        rectangle((char *)buf_win, 480 , 30, 30, 119, 22, 0xffc6c6c6);
        //putstr((char *)buf_win, 480, 30, 30, 0xff000084, s);
        putstr((char *)buf_win, 480, 30, 30, RGB(255, 127, 0, 0), s);
		sheet_refresh(sht_win, 30, 30, 119, 22);

        if (count%300 == 0) {
            if (k == 0) {
                sheet_slide(sht_win2, x+=1, 300);
                if (x >= 1365 - 200) k = 1;
            } else {
                sheet_slide(sht_win2, x-=1, 300);
                if (x <= 0) k = 0;
            }

        }

        pthread_mutex_lock(&mtx);
        if (key.status == 1) {
            key.status = 0;

            sprintf(s, "[key code: %02d %d]", key.code, key.value);

            rectangle((char *)buf_win, sht_win->bxsize, 30, 80, 300, 22, 0xffc6c6c6);
            putstr((char *)buf_win, sht_win->bxsize, 30, 80, 0xff000084, s);
            sheet_refresh(sht_win, 30, 80, 300, 22);

            if (key.value == 1) {
                s[0] = keytable[key.code];
                s[1] = '\0';
                if (s[0] >= 32 && s[0] <= 126) {
                    putstr_f(sht_win, cursor_x + xx, 240, 0x00000084, 0x00D4D0C8, s);
                    //rectangle((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 12, 22, 0xffc6c6c6);
                    //putstr((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 0xff000084, s);
                    //sheet_refresh(sht_win, 30, 240, 300, 22);  
                    cursor_x += 12;
                }
                if (key.code == 14 && cursor_x > 8) {
                    cursor_x -= 12;
                    putstr_f(sht_win, cursor_x + xx, 240, 0x00000084, 0x00D4D0C8, "  ");
                    rectangle((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 7, 22, 0xffc6c6c6);
                    sheet_refresh(sht_win, cursor_x + xx, 240, 7, 22);  
                    //rectangle((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 12, 22, 0xffc6c6c6);
                    //putstr((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 0xff000084, " ");
                    //sheet_refresh(sht_win, 30, 240, 300, 22);  
                    //cursor_x -= 12;
                }
            }
        }

            //rectangle((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 7, 22, 0x00ffffff);
            //sheet_refresh(sht_win, cursor_x + xx, 240, 7, 22);  
        if (count % 40000 == 0) {
            rectangle((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 7, 22, 0x00D4D0C8);
            sheet_refresh(sht_win, cursor_x + xx, 240, 7, 22);  
        } else if (count % 80001 == 0) {
            rectangle((char *)buf_win, sht_win->bxsize, cursor_x + xx, 240, 7, 22, 0x00ffffff);
            sheet_refresh(sht_win, cursor_x + xx, 240, 7, 22);  
        }

        if (mouse.status == 1) {
            mouse.status = 0;

            sprintf(s, "[lrc %d %d]", mouse.x, mouse.y);
            if ((mouse.btn & 0x01) != 0) {
                s[1] = 'L';
                sheet_slide(sht_win3, mouse.x, mouse.y);
            }
            if ((mouse.btn & 0x02) != 0) {
                s[3] = 'C';
            }
            if ((mouse.btn & 0x04) != 0) {
                s[2] = 'R';
            }
            rectangle((char *)buf_win, sht_win->bxsize, 30, 120, 300, 22, 0xffc6c6c6);
            putstr((char *)buf_win, sht_win->bxsize, 30, 120, 0xff000084, s);

            sheet_refresh(sht_win, 30, 120, 300, 22);
            sheet_slide(sht_mouse, mouse.x, mouse.y);

            o_x = mouse.x; o_y = mouse.y;
        }
        pthread_mutex_unlock(&mtx);
    }

    fb_close(&fbdev);
	endwin();
    pthread_join(id1, &tret);
    pthread_join(id2, &tret);
	return 0;
}


void loop()
{
    //int iy;

	Picture pic1;
	pic_init(&pic1);
	pic1.filename = "1.jpg";
    pic1.open(&pic1);
 
	Picture pic2;
	pic_init(&pic2);
	pic2.filename = "123.jpg";
    pic2.open(&pic2);   

	while (1) {
		//pclear();
		//demo();
        //show("img/1.jpg");
		//pic1.hide = 1;
        //rectangle(0, 0, 500, 300, 100, 255, 255, 0);

        pic1.show(&pic1);
        //pic2.show(&pic2);
        fps++;
        //print_f("123",0, 0, 0);
        //usleep(100000000);
        //pic1.filename = "img/img_6.jpg";
        //pic1.show(&pic1);
        //usleep(1000);
        pic1.filename = "img/img_8.jpg";
	    //while(1);
	}
}

static void exit_handle(void)
{
    int ret;
    printf("\033[?25h");          //show 
	endwin();
    ret = system("cp ./x /dev/fb0");
	printf(">> xfb As Exit...\n");
}

static void sig_handle(int sig)
{
    /* Exit */
	exit(EXIT_SUCCESS);
}

