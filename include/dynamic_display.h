#ifndef __DYNAMIC_DISPLAY_H__
#define __DYNAMIC_DISPLAY_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

int lcd;
unsigned int *lcdmap;

void lcd_init(void);
int Show_Bmp_Way(char *picname, int way);
void show_xbmp_leftpart(char *pic_name);

#endif