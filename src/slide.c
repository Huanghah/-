#include "slide.h"
/*
*	@功能：获取一次划动的方向
*	@参数：无
*	@返回值: 1 == 上划;	 2==下划;	3==左划;	4:右划;		0:不划
*	@说明：无
*/
int get_slide()
{
	int x1, x2, y1, y2;
	int flag = 0;		//记录当前获取到的坐标的个数 : 0 --> 无;  1-->横坐标;	2-->横纵坐标
	struct input_event xy;
	
	//1, 打开触摸屏文件
	int fd = open(TS_DEV, O_RDWR);
	if(-1 == fd)
	{
		perror("open ts failed");
		return -1;
	}
	
	//2, 读取分析触摸屏数据 -->获取起点坐标
	while(1)	
	{
		read(fd, &xy, sizeof(xy));		//如果触摸屏文件中没有数据-->程序在此阻塞!
		if(xy.type == EV_ABS && xy.code == ABS_X && flag == 0)
		{
			flag = 1;
			x1 = xy.value *800/1024;		//对获取的坐标进行等比例换算
		}
		if(xy.type == EV_ABS && xy.code == ABS_Y && flag == 1)
		{
			flag = 2;
			y1 = xy.value *480/600;
		}	
		if(flag == 2)		//获取的第一个坐标就是起点坐标
		{
			flag = 0;
			printf("start : (%d, %d)\n", x1, y1);
			break;		
		}			
	}
	x2 = x1;	y2 = y1;
	//获取终点坐标 (x2, y2)
	while(1)	
	{
		read(fd, &xy, sizeof(xy));		//如果触摸屏文件中没有数据-->程序在此阻塞!
		if(xy.type == EV_ABS && xy.code == ABS_X )
		{
			x2 = xy.value *800/1024;		//对获取的坐标进行等比例换算
		}
		if(xy.type == EV_ABS && xy.code == ABS_Y )
		{
			y2 = xy.value *480/600;
		}	
		if(xy.type == EV_KEY && xy.code == BTN_TOUCH && xy.value == 0)//压力值为0的时候此时获取终点坐标
		{
			printf("end : (%d, %d)\n", x2, y2);
			break;	
		}			
	}
	close(fd);	
	//判断终点位置相对起点的位置

	if(x1<10 && x2>10)
	{
		printf("return !\n");
		return 5;
	}
	if(x2>x1 && (y2-y1)*(y2-y1) < (x2-x1)*(x2-x1))	//右划
	{
		printf("right slide -->\n");
		return 4;
	}
	if(x2<x1 && (y2-y1)*(y2-y1) < (x2-x1)*(x2-x1))	//左划
	{
		printf("left slide <--\n");
		return 3;
	}
	if(y2>y1 && (y2-y1)*(y2-y1) > (x2-x1)*(x2-x1))	//下划
	{
		printf("down slide v\n");
		return 2;
	}	
	if(y2<y1 && (y2-y1)*(y2-y1) > (x2-x1)*(x2-x1))	//上划
	{
		printf("up slide ^\n");
		return 1;
	}	
	if(x2 >250 && x2 < 315 && y2 > 224 && y2 < 295) 	//不划
	{
		printf("左边 !\n");
		//ts_x = x1;
		//ts_y = y1;
		return 0;
	}	
	if(x2 >484 && x2 < 550 && y2 > 220 && y2 < 290)	
	{
		printf("右边 !\n");
		//ts_x = x1;
		//ts_y = y1;
		return -1;		
	}
	if(x2 >350 && x2 < 450 && y2 > 145 && y2 < 300)
	{
		printf("吉\n");
		return 8;
	}
	if(x1 > 0 && y1 > 0)
		return 6;
}




