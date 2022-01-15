#include "menu.h"

void menu(void)
{
    int rt=0;
    char *photo[12] = {"./photo/1.bmp", "./photo/2.bmp", "./photo/3.bmp", 
                                "./photo/4.bmp", "./photo/5.bmp", "./photo/6.bmp",
                                "./photo/7.bmp", "./photo/8.bmp", "./photo/9.bmp", 
                                 "./photo/10.bmp", "./photo/11.bmp", "./photo/12.bmp"};
    char *small_photo[12] = {"./small_photo/1.bmp", "./small_photo/2.bmp", "./small_photo/3.bmp", 
                             "./small_photo/4.bmp", "./small_photo/5.bmp", "./small_photo/6.bmp",
                             "./small_photo/7.bmp", "./small_photo/8.bmp", "./small_photo/9.bmp", 
                             "./small_photo/10.bmp", "./small_photo/11.bmp", "./small_photo/12.bmp"};
    //视频进入

    //显示主界面
    show_all_bmp("./photo/main.bmp", 0, 0);
    printf("显示主界面\n");
    //双触摸解锁，从中间往两边打开，然后先显示一张白色背景图片，再显示2021年图片
    while(1)
    {
        if(get_slide() == 0)
        {
            while(get_slide() != -1);
            break;
        }
        if(get_slide() == -1)
        {
            while(get_slide() != 0);
            break;           
        }
    }

    int i= 11;
    while(1)
    {
        Show_Bmp_Way("./photo/1.bmp", 3);
        rt = get_slide();
        if(rt == 5)
            break;
        //显示图片，上滑显示上半年，下滑显示下半年，左滑循环显示上一年，右滑显示下一年
        while(1)
        {
            switch(rt)
            {
                case 1:
                        for(int i=0;i<6;i++)
                        {
                            Show_Bmp_Way(photo[i], 5-i);
                            sleep(1);
                        }
                        break;
                case 2:
                        for(int i=6;i<12;i++)
                        {
                            Show_Bmp_Way(photo[i], i%6);
                            sleep(1);
                        }
                        break;
                case 3:
                        show_all_bmp("./photo/black.bmp", 0, 0);
                        while(1)
                        {
                            //显示左半边图片
                            show_xbmp_leftpart(small_photo[i%12]);
                            //显示中间图片
                            show_all_bmp(small_photo[(i+1)%12], 200, 120);
                            //显示右半边图片
                            show_all_bmp(small_photo[(i+2)%12], 600, 120);
                            rt = get_slide();    
                            if(rt == 5)
                            {
                                break;
                            } 
                            else if(rt == 4) //右滑
                            {
                                i--;
                                if(i == -1)
                                    i=11;
                            }
                            else if(rt == 3)//左滑
                            {
                                i++;
                            }
                            else if(rt == 6)//单击
                            {
                                Show_Bmp_Way(photo[(i+1)%12], 6);
                                get_slide();  
                                show_all_bmp("./photo/black.bmp", 0, 0);
                            }
                        }
                        break;
                case 4:break;
            }
            rt = get_slide();
            if(rt == 5)
                break;
        }
    }

}