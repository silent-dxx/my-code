/*************************************************************************
	> File Name: class.h
	> Author: 
	> Mail: 
	> Created Time: 2015年05月17日 星期日 22时30分37秒
 ************************************************************************/

#ifndef _CLASS_H
#define _CLASS_H

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

extern void pic_init(Picture *pic);
#endif
