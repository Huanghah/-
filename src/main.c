#include "menu.h"

int main(void)
{
    lcd_init();
	system("madplay ./music/RF.mp3 &");// &��̨���ű�������
    //����˵�
    menu();
    //�˳�����
    show_all_bmp("./photo/white.bmp", 0, 0);
	//�ر�����
	system("killall -STOP madplay"); //�رպ�̨����
    
    return 0;
}