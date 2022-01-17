#include "menu.h"

int main(void)
{
    //lcd初始化
    lcd_init();
    //播放视频
	system("mplayer ./video/begin_video.avi -quiet -zoom -x 800 -y 480 ");
    //进入菜单
    menu();

    return 0;
}