#include "lcddevice.h"
#include "linux/fb.h"
#include <sys/ioctl.h>
#include <string.h>

#define ARM
/*

//设计一个lcd结构体
struct LcdDevice {
	int fd;//文件描述符
	unsigned int *mptr;//保存映射首地址
	int width, height;
	int screenSize;//屏幕占用的内存空间
};
*/


struct LcdDevice *Lcd_init(const char *lcdname)
{
	//申请堆空间
	struct LcdDevice *lcd = (struct LcdDevice*)malloc(sizeof(struct LcdDevice));
	if(lcd == NULL) goto err;

	//打开lcd设备
	lcd->fd = open(lcdname, O_RDWR);
	if(lcd->fd < 0)
	{
		perror("open lcd error");
		goto err;
	}

	//从驱动获取lcd信息  ioctl
	struct fb_var_screeninfo info;
	int ret  = ioctl(lcd->fd, FBIOGET_VSCREENINFO,  &info);
	if(ret < 0)
	{
		perror("get info error");
		goto err;
	}

#ifdef  ARM
	lcd->width = info.xres;
	lcd->height = info.yres;
#else //ubuntu
	lcd->width = info.xres_virtual;
	lcd->height = info.yres_virtual;
#endif
	printf("screen:(%d:%d)\n", lcd->width, lcd->height);

	//屏幕空间计算
	lcd->screenSize = lcd->width*lcd->height*4;

	//映射
	lcd->mptr = (unsigned int*)mmap(NULL,lcd->screenSize, PROT_READ|PROT_WRITE, MAP_SHARED,lcd->fd, 0);
	if(lcd->mptr == (void*)-1)
	{
		perror("mmap error");
		goto err;
	}
	return lcd;

err:
	if(lcd!=NULL) free(lcd);
	return NULL;
}



bool lcd_destroy(struct LcdDevice* lcd)
{
	if(lcd == NULL)return true;
	//释放映射
	if(lcd->mptr != (void*)-1)
		munmap(lcd->mptr, lcd->screenSize);

	//关闭文件
	if(lcd->fd > 0)
		close(lcd->fd);

	//释放堆空间
	free(lcd);
	return true;
}


void draw_rect(struct LcdDevice* lcd, int x, int y, int w, int h, unsigned int color)
{
	unsigned int *p = lcd->mptr + y*lcd->width+x;  //用p指向绘图开始位置

	for(int i=0; i<h; i++)
	{
		for(int j=0; j<w; j++)
		{
			p[j] = color;
		}
		//把p移动到下一行
		p+=lcd->width;
	}
}


void draw_bmp(struct LcdDevice* lcd, int x, int y, const char *pictrue)
{
	//打开图片文件
	FILE *file = fopen(pictrue, "r");
	if(file == NULL)
	{
		perror("fopen fail");
		return ;
	}
	//读取图片头
	struct BmpHeader header;
	int ret = fread(&header, 54, 1, file);
	if(ret < 0)
	{
		perror("read header fail");
		return ;
	}
	
	//读取像素数据
	unsigned char data[header.biSizeImage];
	ret = fread(data, sizeof(data), 1, file);
	if(ret < 0)
	{
		perror("fread error");
		return ;
	}

	int twidth = header.biWidth;
	int theight = header.biHeight;
	if(x+header.biWidth > lcd->width)
	{
		twidth = lcd->width - x;
	}
	if(y+header.biHeight > lcd->height)
	{
		theight = lcd->height - y;
	}
	//如bmp图片行占用的字节数不是4的倍数，要补齐tmpbyte字节
	int tmpbyte =0; 
	if(header.biWidth*3 % 4 != 0)
	{
		tmpbyte = 4-header.biWidth*3 % 4;
	}
	

	//p保存图片绘制的开始位置
	unsigned int *p = lcd->mptr+y*lcd->width+x + theight*lcd->width;
	unsigned char *dp = data;
	for(int i=0; i<theight; i++)
	{
		for(int j=0; j<twidth; j++)
		{
			memcpy(p+j, dp+j*3, 3);//绘制一行
		}
		//指针p，dp偏移到下一行
		p-=lcd->width;
		dp+=header.biWidth*3+tmpbyte;
	}

	if(file) fclose(file);
	return ;
}

