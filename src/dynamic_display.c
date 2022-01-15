#include "dynamic_display.h"

void lcd_init(void)
{
	// 打开LCD文件
	lcd = open("/dev/fb0", O_RDWR);
	if(-1 == lcd)
	{
		perror("open lcd failed");
		return ;
	}
	lcdmap = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
	if(lcdmap == MAP_FAILED)
	{
		perror("mmap failed");
		return ;
	}
}

void lcd_uninit(void)
{
	munmap(lcdmap, 800*480*4);
	
	close(lcd);
}

/*
*	@功能：在LCD上动态显示一张800*480的bmp图片
*	@参数：
		@picname:图片的路径名
		@way:显示的方法 1：从上往下;	2：从左往右;	3:中间往两边;	4:百叶窗	5：圆形扩散   其他:直接显示
*	@返回值: 成功返回0, 失败返回-1
*	@说明：无 
*/
int Show_Bmp_Way(char *picname, int way)
{	
	int i, j;
	unsigned int tmp;
	unsigned char A, R, G, B;
	unsigned int bmp_buf[800*480];		//存放转码之后的数据  ARGB数据
	unsigned char head[54];
	unsigned char buf[800*480*3];
	
	FILE *fp = fopen(picname, "r");
	if(NULL == fp)
	{
		perror("fopen failed");
		return -1;
	}
	fread(head, 54, 1, fp);		//获取54字节头信息
	fread(buf, 3, 800*480, fp);	//获取800*480个像素点的RGB数据 	
	for(i=0; i<800*480; i++)	//对第i个像素点进行转码
	{
		A = 0x00;
		R = buf[i*3+2];
		G = buf[i*3+1];
		B = buf[i*3+0];
		// ARGB = A R G B		1234 = 1*1000+ 2*100 +3*10 +4
		bmp_buf[i] = A<<24 | R<<16 | G<<8 | B;
	}
	for(i=0; i<240; i++)	//像素点行数
	{
		for(j=0; j<800; j++)	//像素点列数
		{
			//第i行第j列的点 == 第479-i行第j列的点
			tmp = bmp_buf[800*i+j];
			bmp_buf[800*i+j] = bmp_buf[800*(479-i)+j];
			bmp_buf[800*(479-i)+j] = tmp;
		}
	}
	
	//2) 把读取的图片数据写入LCD	==> 实现将write 改为内存映射方式显示图片！


	//显示方法
	switch(way)
	{
		case 0:		//从上往下
			for(i=0; i<480; i++)
			{
				for(j=0; j<800; j++)
					lcdmap[800*i+j] = bmp_buf[800*i+j];
				usleep(2*1000);
			}
			break;		
		
		case 2:		//从左往右
			for(j=0; j<800; j++)
			{
				for(i=0; i<480; i++)
					lcdmap[800*i+j] = bmp_buf[800*i+j];
				usleep(2*1000);
			}
			break;
		case 3:		//中间往2边
			for(j=0; j<400; j++)
			{
				for(i=0; i<480; i++)
				{
					lcdmap[800*i+400+j] = bmp_buf[800*i+400+j];
					lcdmap[800*i+399-j] = bmp_buf[800*i+399-j];
				}
				usleep(2*1000);
			}
			break;
		
		case 4:		//百叶窗 --> 水平方向百叶窗(6扇窗)
			for(i=0; i<80; i++)
			{
				for(j=0; j<800; j++)
				{
					for(int k=0; k<6; k++)	//显示6扇窗
					{
						lcdmap[800*(i+80*k)+j] = bmp_buf[800*(i+80*k)+j];
					}	
				}
				usleep(5*1000);
			}
			break;
		
		case 1:		//圆形扩散	--> 显示圆球范围里面的图片像素点数据 --> 半径逐渐增加
			for(int r=1; r<=467; r++)
			{
				for(i=(240-r)>0?(240-r):0; i<=240+r&&i<480;	i++ )	//i纵坐标
				{
					for(j=(400-r)>0?(400-r):0; j<400+r&&j<800; j++ )
					{
						if( (i-240)*(i-240)+(j-400)*(j-400) < r*r )	//判断点(j,i)是否在圆球内部
						{
							lcdmap[800*i+j] = bmp_buf[800*i+j];		//显示图片的第(j, i)的点
						}	
					}
				}
			}
			break;

		case 5:		//从下往上
			for(i=479; i>=0; i--)
			{
				for(j=0; j<800; j++)
					lcdmap[800*i+j] = bmp_buf[800*i+j];
				usleep(2*1000);
			}
			break;	
		default:	//直接显示
			for(i=0; i<800*480; i++)
				lcdmap[i] = bmp_buf[i];
			break;
		
	}
	fclose(fp);
}

/*
**	@函数名： show_xbmp
**	@功能 ： LCD左边以正常方式显示任意大小图片的右半边
**	@参数 ： char *pic_name:文件路径及名称
**  @返回值： 无
*/
void show_xbmp_leftpart(char *pic_name)
{
	char A, R, G, B;
	int bmp_fd;
	int i, j;
	unsigned int wight, high;	//存放图片宽度和高度
	unsigned char head[54];		//存放bmp头信息
	printf("%s\n",pic_name);
	//1，打开bmp图片，读取数据
	bmp_fd = open(pic_name, O_RDWR);
	if(-1 == bmp_fd)
	{
		perror("open bmp failed");
		return;
	}

	//lseek(bmp_fd, 54, SEEK_SET);
	read(bmp_fd, head, sizeof(head));	//把54字节头信息读取出来
	wight = head[21]<<24 | head[20]<<16 | head[19] << 8 | head[18];			//解析图片的宽度信息
	high =  head[25]<<24 | head[24]<<16 | head[23] << 8 | head[22];			//解析图片的高度信息
	printf("%s picture wight:%u  high: %u\n", pic_name, wight, high);

	unsigned char bmp_buf[wight*high*3];
	unsigned int buf[wight*high];

	read(bmp_fd, bmp_buf, sizeof(bmp_buf));		//读取bmp图片中的RGB像素数据

	//2，把读取的bmp图片数据进行解码得到 ARGB数据
	for (i = 0; i < wight*high; ++i)
	{
		A = 0x00;
		B = bmp_buf[3*i];
		G = bmp_buf[3*i+1];
		R = bmp_buf[3*i+2];
		buf[i] = A<<24 | R<<16 | G<<8 | B;
	}

	//将LCD映射到内存
	unsigned int *mapaddr = mmap(NULL,					//系统内核找一片大小合适的空间进行映射
					800*480*4,				//映射的空间大小 (LCD屏幕大小)
					PROT_READ | PROT_WRITE,	//读写权限
					MAP_SHARED,				//共享方式映射  (映射内存中的数据实时映射到设备文件)
					lcd,					//文件描述符
					0);						//偏移量
	if(MAP_FAILED == mapaddr)
	{
		perror("mmap failed");
		return;
	}
	//3，把解码后的图片数据写入映射内存空间
	for (i = 120; i < 360; ++i)
	{
		for (j = 0; j < 200; ++j)
		{
			mapaddr[800*i+j] = buf[400*(239-i+120)+j+200];
		}
	}
	close(bmp_fd);	
	return;

}