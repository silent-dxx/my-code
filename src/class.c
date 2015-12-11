/*************************************************************************
	> File Name: class.c
	> Author: 
	> Mail: 
	> Created Time: 2015年05月17日 星期日 22时26分40秒
 ************************************************************************/

#include "jpeg.h"
#include "draw.h"
#include "xfb.h"
#include "class.h"

/*
struct Picture
{
    char *filename;
    char *old_name;
    int width, height;
	int hide;
    unsigned char *pic_buf;
    //unsigned char * (*open)(char *, int *, int *);
    void (*open)(struct Picture *pic);
    void (*show)(struct Picture *pic);
};

typedef struct Picture Picture;
*/

void picopen(struct Picture *pic)
{
    pic->pic_buf = read_jpeg_file(pic->filename, &pic->width, &pic->height);
    pic->old_name = pic->filename;
}

void picxs(struct Picture *pic)
{
    int y;
    //printf("%d %d\n", pic.width, pic.height);
	if (pic->filename != NULL) {
        if (pic->filename != pic->old_name)
        {
            pic->open(pic);
        }
		if (pic->hide == 0) {
    	    for(y = 0; y < pic->height - 1; y++)
	        {
		        //draw_hline((s_width - pic->width) / 2, y, pic->width, pic->pic_buf + y * 3 * pic->width);
	        }
		}
	}
}

void pic_init(Picture *pic)
{
	pic->hide = 0;
    pic->open = picopen;
    pic->show = picxs;
    pic->old_name = pic->filename;
}

