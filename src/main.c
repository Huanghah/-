#include "menu.h"

int main(void)
{
    lcd_init();
	system("madplay ./music/RF.mp3 &");// &后台播放背景音乐
    //进入菜单
    menu();
    //退出界面
    show_all_bmp("./photo/white.bmp", 0, 0);
	//关闭音乐
	system("killall -STOP madplay"); //关闭后台音乐
    
    return 0;
}