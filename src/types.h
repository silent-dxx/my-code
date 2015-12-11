#pragma noce
#ifndef TYPES_H
#define TYPES_H

#define TRUE        1
#define FALSE       0
#define MAX(x,y)        ((x)>(y)?(x):(y))
#define MIN(x,y)        ((x)<(y)?(x):(x))
typedef unsigned char *POINTER;
typedef unsigned char UCHAR;
typedef short Bool;

struct point
{
	int x, y;
};

struct image
{
	int width, height;
	unsigned char * buf;
};

#endif
