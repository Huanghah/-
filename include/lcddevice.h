#ifndef _LCDDEVICE_H_
#define _LCDDEVICE_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <string.h>
#include "font.h"

#pragma pack(1)  //设置1字节对齐
struct BmpHeader
{
	unsigned short bfType;//文件类型BM
	unsigned int bfSize;//文件大小
	unsigned int bfReserved;//保留设置为0
	unsigned int bfoffBits;//偏移

	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biX;
	unsigned int biY;
	unsigned int biClrUsed;
	unsigned int biCrlImportant;
};
#pragma pack(0)

//设计一个lcd结构体
struct LcdDevice {
	int fd;//文件描述符
	unsigned int *mptr;//保存映射首地址
	int width, height;
	int screenSize;//屏幕占用的内存空间

};

struct LcdDevice *Lcd_init(const char *lcdname);
bool lcd_destroy(struct LcdDevice* lcd);
void draw_rect(struct LcdDevice* lcd, int x, int y, int w, int h, unsigned int color);
void draw_bmp(struct LcdDevice* lcd, int x, int y, const char *pictrue);

#endif//_LCDDEVICE_H_
