#include "font.h"

//初始化Lcd
struct LcdDevice *init_lcd(const char *device)
{
	//申请空间
	struct LcdDevice* lcd = malloc(sizeof(struct LcdDevice));
	if(lcd == NULL)
	{
		return NULL;
	} 

	//1打开设备
	lcd->fd = open(device, O_RDWR);
	if(lcd->fd < 0)
	{
		perror("open lcd fail");
		free(lcd);
		return NULL;
	}
	
	//映射
	lcd->mp = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd->fd,0);

	return lcd;
}

int main()
{
	
    //初始化Lcd
	struct LcdDevice* lcd = init_lcd("/dev/fb0");
			
	//打开字体	
	font *f = fontLoad("./simfang.ttf");
	if(f==NULL) perror("fontloag");

	//字体大小的设置
	fontSetSize(f,72);
	
	
	//创建一个画板（点阵图）
	bitmap *bm = createBitmapWithInit(200,200,4,getColor(0,255,255,255)); //也可使用createBitmapWithInit函数，改变画板颜色
	//bitmap *bm = createBitmap(288, 100, 4);
	
	char buf[] = "晚安";
	
	//将字体写到点阵图上
	fontPrint(f,bm,0,0,buf,getColor(0,255,0,0),0);
	
	
	
	//把字体框输出到LCD屏幕上

	//把字体框输出到LCD屏幕上

	
	//关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
	
}
