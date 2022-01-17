#include "test_font.h"
#include "lcddevice.h"
void  show_bitmap(struct LcdDevice*lcd, int x, int y,   bitmap* mp)
{
	unsigned int *ptr = lcd->mptr + y*lcd->width+x;
	unsigned char *mapptr = mp->map;
	
	for(int i=0; i<mp->height; i++)
	{
		for(int j=0; j<mp->width; j++)		
		{
			memcpy(ptr+j, mapptr+j*mp->byteperpixel, mp->byteperpixel);
		}
		ptr+=lcd->width;
		mapptr += mp->width*mp->byteperpixel;
	}
}


void test_font(int font_i)
{
	
	struct LcdDevice* lcd = Lcd_init("/dev/fb0");
	font *f = fontLoad("./STXINWEI.TTF");
  
    bitmap *titlemp = createBitmapWithInit(600,50, 4, 0x00000000);
    fontSetSize(f, 48);
    fontPrint(f, titlemp, 12,0,"黄敏弘和梁汝柔在一起已经",0xffffffff,0);
    show_bitmap(lcd, 100, 0,  titlemp);

    if(font_i == 1)
    {
        titlemp = createBitmapWithInit(500,72, 4, 0x00000000);
        fontSetSize(f, 72);
        fontPrint(f, titlemp, 242,0,"830",0xffffffff,0);
        show_bitmap(lcd, 100, 400,  titlemp);
    }
	
    else if(font_i == 2)
    {
        titlemp = createBitmapWithInit(500,60, 4, 0x00000000);
        fontSetSize(f, 60);
        fontPrint(f, titlemp, 42,0,"2年3个月08天",0xffffffff,0);
        show_bitmap(lcd, 180, 400,  titlemp);	
    }

    else if(font_i == 3)
    {
        titlemp = createBitmapWithInit(500,60, 4, 0x00000000);
        fontSetSize(f, 60);
        fontPrint(f, titlemp, 42,0,"27个月08天",0xffffffff,0);
        show_bitmap(lcd, 200, 400,  titlemp);
    }

    else if(font_i == 4)
    {
        titlemp = createBitmapWithInit(500,72, 4, 0x00000000);
        fontSetSize(f, 72);
        fontPrint(f, titlemp, 42,0,"118周4天",0xffffffff,0);
        show_bitmap(lcd, 210, 400,  titlemp);
    }

    titlemp = createBitmapWithInit(300,24, 4, 0x00000000);
    fontSetSize(f, 24);
    fontPrint(f, titlemp, 42,0,"起始日:2019-10-24 星期四",0xffffffff,0);
    show_bitmap(lcd, 220, 460,  titlemp);	

	lcd_destroy(lcd);
	fontUnload(f);
}