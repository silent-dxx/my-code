#pragma noce
#ifndef _JPEG_H_
#define _JPEG_H_
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
unsigned char * read_jpeg_file(char *filename, int *w, int *h);
#endif
