#include "../Headers/ts.h"

void ts_open(void)
{
	ts_fd = open("/dev/input/event0", O_RDWR | O_NONBLOCK);
	if(ts_fd == -1)
	{
		printf("打开触摸屏失败!\n");
		return;
	}
}

void ts_close(void)
{
	close(ts_fd);
}

int get_xy(int *ts_x, int *ts_y)
{
	struct input_event buf;
	int count = 0;
	while(1)
	{
		read(ts_fd, &buf, sizeof(buf));
		if(buf.type == EV_ABS)
		{
			if(buf.code == ABS_X)
			{
				*ts_x = (int)(buf.value/1024.0 * 800);
				count++;
			}
			if(buf.code == ABS_Y)
			{
				*ts_y = (int)(buf.value/600.0 * 480);
				count++;
			}
			if(count == 2)
				break;
		}
	}

	return 0;
}