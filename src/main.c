#include "menu.h"

int main(void)
{
    lcd_init();
	//system("mplayer ./video/begin_video.mp4 -zoom -x 800 -y 480");
    //����˵�
    menu();
    //�˳�����
    show_all_bmp("./photo/white.bmp", 0, 0);
	//�ر�����
	system("killall -STOP madplay"); //�رպ�̨����
    
    return 0;
}