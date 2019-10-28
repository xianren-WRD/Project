#ifndef _SHOWBMP_H_
#define _SHOWBMP_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <unistd.h>
#include <pthread.h>
#include "ts.h"
#include "yuvcamera.h"


#define WELCOME 1
#define LOGIN   2
#define MAIN    3
#define PHOTO   4
#define MUSIC   5
#define VIDEO   6
#define CAMERA  7

int *pcam;
camera_t *camera;

static char pic[5][15]={
	"/root/1.bmp",
	"/root/2.bmp",
	"/root/3.bmp",
	"/root/4.bmp",
	"/root/5.bmp"
};

static char picture[9][20]={
	"/root/IMG00000.bmp",
	"/root/IMG00001.bmp",
	"/root/IMG00002.bmp",
	"/root/IMG00003.bmp",
	"/root/IMG00004.bmp",
	"/root/IMG00005.bmp",
	"/root/IMG00006.bmp",
	"/root/IMG00007.bmp",
	"/root/IMG00008.bmp"
};

void show_bmp(char const *argv);
void show_welcome(void);
void show_login(void);
void show_main(void);
void show_photo(void);
void show_auto(void);
void show_hand(void);
void *monitor (void *argv);
void show_camera(void);
#endif