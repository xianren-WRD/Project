#include <stdio.h>
#include "showbmp.h"
#include "ts.h"

int status = WELCOME;

int main(int argc, char const *argv[])
{
	camera = malloc(sizeof(camera_t));
	yuvcamera_init(camera, "/dev/video7");

	int lcd_fd;
	lcd_fd =open("/dev/fb0",O_RDWR);
	pcam = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);

	ts_open();
	while(1)
	{
		switch(status)
		{
			case WELCOME:
				show_welcome();
				status++;
				break;
			case LOGIN:
				show_login();
				break;
			case MAIN:
				show_main();
				break;
			case PHOTO:
				show_photo();
				break;
			// case 5:
			// 	show_music();
			// 	break;
			// case 6:
			// 	show_video();
			// 	break;
			case CAMERA:
				show_camera();
				break;
		}
	}
	return 0;
}