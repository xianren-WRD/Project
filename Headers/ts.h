#ifndef _TS_H_
#define _TS_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/input.h>
#include <unistd.h>

int ts_fd;

void ts_open(void);
void ts_close(void);
int get_xy(int *ts_x, int *ts_y);
#endif