#include "../Headers/showbmp.h"

extern int status;

void show_bmp(char const *argv)
{
	int lcd_fd;
	lcd_fd =open("/dev/fb0",O_RDWR);
	
	if(lcd_fd < 0)
	{
		printf("打开显示屏失败\n");
		return;
	}
	
	int * plcd = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(plcd == MAP_FAILED)
	{
		perror("lcd mmap fail");
		close(lcd_fd);
		return;
	}

	int pic_fd;
	pic_fd = open(argv,O_RDWR);
	char * ppic = mmap(NULL,800*480*3+54,PROT_READ | PROT_WRITE,MAP_SHARED,pic_fd,0);
	if(ppic == MAP_FAILED)
	{
		perror("pic mmap fail");
		close(pic_fd);
		close(lcd_fd);
		return;
	}
	
	ppic += 54;
	int x,y;
	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
			*(plcd +y*800 + x ) = (*(ppic+2)<<16)  | (*(ppic+1)<<8)  |  (*(ppic+0)<<0);
			ppic += 3;	
		}
	}

	int t;
	int i = 0, j = 479;
	while(i < j)
	{
		for(x=0;x<800;x++)
		{
			t = *(plcd + i*800 + x);
			*(plcd + i*800 + x) = *(plcd + j*800 + x);
			*(plcd + j*800 + x) = t;
		}
		i++;
		j--;
	}
	
	munmap(plcd,800*480*4);
	ppic -= (800*480*3 + 54);
	munmap(ppic,800*480*3 + 54);
	
	close(pic_fd);
	close(lcd_fd);	
}

void show_welcome(void)
{
	int i;
	for (i = 0; i < 9; ++i)
	{
		show_bmp(picture[i]);
		usleep(400000);
	}
}

void show_login()
{
	char usrname[15]={0};
	char passwd[15]={0};
	int ts_x, ts_y;
	show_bmp("/root/login.bmp");
	while(1)
	{
		get_xy(&ts_x, &ts_y);
		if(ts_x > 168 && ts_x < 600 && ts_y > 191 && ts_y < 238)
		{
			printf("请输入用户名：\n");
			scanf("%s", usrname);
		}
		if(ts_x > 168 && ts_x < 600 && ts_y > 256 && ts_y < 302)
		{
			printf("请输入密码：\n");
			scanf("%s", passwd);
		}
		if(ts_x > 232 && ts_x < 319 && ts_y > 323 && ts_y < 371)
		{
			if(strcmp("haoxin", usrname) == 0 && strcmp("123456", passwd) == 0)
			{
				status = MAIN;
				break;
			}
			else
			{
				printf("您输入的用户名或密码有误！\n");
			}
		}		
	}
}

void show_main()
{
	show_bmp("/root/main.bmp");
	int ts_x, ts_y;
	while(1)
	{
		get_xy(&ts_x, &ts_y);

		if(ts_x > 440 && ts_x < 554 && ts_y > 170 && ts_y < 288)//相册
		{
			status = PHOTO;
			break;
		}
		// if(ts_x > 56 && ts_x < 187 && ts_y > 165 && ts_y < 294)//音乐
		// {
		// 	status = 5;
		// 	break;
		// }
		// if(ts_x > 625 && ts_x < 744 && ts_y > 171 && ts_y < 286)//视频
		// {
		// 	status = 6;
		// 	break;
		// }
		if(ts_x > 245 && ts_x < 374 && ts_y > 164 && ts_y < 293)//摄像头
		{
			status = CAMERA;
			break;
		}	
	}
}

void show_photo()
{
	show_bmp("/root/photo.bmp");
	int ts_x, ts_y;
	while(1)
	{
		get_xy(&ts_x, &ts_y);
		if(ts_x > 8 && ts_x < 99 && ts_y > 414 && ts_y < 467)//自动播放
		{
			show_auto();
			break;
		}
		if(ts_x > 350 && ts_x < 450 && ts_y > 414 && ts_y < 467)//手动播放
		{
			show_hand();
			break;
		}
		if(ts_x > 672 && ts_x < 794 && ts_y > 414 && ts_y < 467)//返回主界面
		{
			status = MAIN;
			break;
		}
	}
}

void show_auto()
{
	int i;
	for (i = 0; i < 5; ++i)
	{
		show_bmp(pic[i]);
		sleep(1);
	}

	printf("自动播放完毕！\n");
	
	int ts_x, ts_y;
	while(1)
	{
		get_xy(&ts_x, &ts_y);
		if(ts_x > 569 && ts_x < 799 && ts_y > 308 && ts_y < 479)
		{
			status = PHOTO;
			break;
		}
	}
}

void show_hand()
{
	int ts_x, ts_y;
	int i = 0;
	show_bmp(pic[i%5]);
	while(1)
	{
		get_xy(&ts_x, &ts_y);
		if(ts_x > 0 && ts_x < 230 && ts_y > 129 && ts_y < 283)
		{
			if(i == 0)
				i = 5;

			show_bmp(pic[(--i)%5]);
		}
		if(ts_x > 569 && ts_x < 799 && ts_y > 129 && ts_y < 283)
		{
			show_bmp(pic[(++i)%5]);
		}
		if(ts_x > 569 && ts_x < 799 && ts_y > 308 && ts_y < 479)
		{
			status = PHOTO;
			break;
		}
	}
}

void *monitor (void *argv)
{
	char rgbbuf[camera->width*camera->height*3];
	while(1)
	{
		yuvcamera_getrgb(camera, rgbbuf);
		yuvcamera_show(camera, pcam, rgbbuf);
	}
}

void show_camera()
{
	show_bmp("/root/camera.bmp");

	pthread_t id1;
	int ts_x, ts_y;
	while(1)
	{
		get_xy(&ts_x, &ts_y);
		if(ts_x > 0 && ts_x < 80 && ts_y > 0 && ts_y < 480)//启动摄像头
		{
			pthread_create(&id1, NULL, monitor,NULL);
		}
		if(ts_x > 719 && ts_x < 800 && ts_y > 0 && ts_y < 480)//返回主界面
		{
			pthread_cancel(id1);
			usleep(400000);
			status = MAIN;
			break;
		}
	}
}