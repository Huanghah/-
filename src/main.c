#include "menu.h"

int main(void)
{
    //lcd��ʼ��
    lcd_init();
    //������Ƶ
	system("mplayer ./video/begin_video.avi -quiet -zoom -x 800 -y 480 ");
    //����˵�
    menu();

    return 0;
}