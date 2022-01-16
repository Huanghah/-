#include "menu.h"

int main(void)
{
    lcd_init();
	//system("mplayer ./video/begin_video.mp4 -zoom -x 800 -y 480");
    //进入菜单
    menu();
    //退出界面
    show_all_bmp("./photo/white.bmp", 0, 0);
	//关闭音乐
	system("killall -STOP madplay"); //关闭后台音乐
    
    return 0;
}