#ifndef __SLIDE_H__
#define __SLIDE_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

#include <linux/input.h>		//输入子系统头文件

#define TS_DEV "/dev/input/event0"

int ts_x, ts_y;

int get_slide();

#endif