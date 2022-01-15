#include "show_bmp.h"
#include "dynamic_display.h"
int show_all_bmp(char *pic_name, unsigned int x_s, unsigned int y_s)
{
	char A, R, G, B;
	int bmp_fd;
	int i, j,k;
	unsigned int wight, high;	//存放图片宽度和高度
	unsigned char head[54];		//存放bmp头信息
	
	//打开bmp图片，读取数据
	bmp_fd = open(pic_name, O_RDWR);
	if(-1 == bmp_fd)
	{
		perror("open bmp failed");
		return -1;
	}
		
	char *p = NULL;
	p = (char *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd,0);
	if(p == (void *)-1)
		printf("mmap error!\n");
	//lseek(bmp_fd, 54, SEEK_SET);
	read(bmp_fd, head, sizeof(head));	//把54字节头信息读取出来
	wight = head[21]<<24 | head[20]<<16 | head[19] << 8 | head[18];			//解析图片的宽度信息
	high =  head[25]<<24 | head[24]<<16 | head[23] << 8 | head[22];			//解析图片的高度信息
	printf("%s picture wight:%u  high: %u\n", pic_name, wight, high);

	unsigned char bmp_buf[wight*high*3];
	unsigned int buf[wight*high];

	read(bmp_fd, bmp_buf, sizeof(bmp_buf));		//读取bmp图片中的RGB像素数据

	//把读取的bmp图片数据进行解码得到 ARGB数据
	for (i = 0; i < wight*high; ++i)
	{
		A = 0x00;
		B = bmp_buf[3*i];
		G = bmp_buf[3*i+1];
		R = bmp_buf[3*i+2];
		buf[i] = A<<24 | R<<16 | G<<8 | B;
	}

	/* 到目前为止,show_buf可以直接搞到lcd上，就可以显示的了 */
	for (i = y_s; i < high+y_s && i < 480; ++i)
	{
		for (j = x_s; j < wight+x_s && j < 800; ++j)
		{
			lcdmap[800*i+j] = buf[wight*(high+y_s-i-1) + j-x_s];
		}
	}
	
	// 撤销映射，关闭文件
	close(bmp_fd);
	
	return 0;
}
