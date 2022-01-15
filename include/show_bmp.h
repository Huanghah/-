#ifndef _SHOW_BMP_H_
#define _SHOW_BMP_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int show_all_bmp(char *pic_name, unsigned int x_s, unsigned int y_s);

#endif