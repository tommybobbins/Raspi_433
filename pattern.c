#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;


// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

void singlet();
void doublet();
void triplet();
void quadruplet();
void quintuplet();
void intro_0();
void intro_1();

void singlet()
{
       int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(1250);
//	printf("1");
}
//1= 15 samples ON, 55 samples OFF
//2= 15 samples ON, 13 samples OFF, 15 samples ON, 55 samples OFF
//3 = 15 samples ON, 12 samples OFF, 15 samples ON, 12 samples OFF, 15 samples ON, 55 samples OFF

void doublet()
{
	int g;
        g=18;	
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(242);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(1250);
//	printf("2");
}

//3 = 15 samples ON, 12 samples OFF, 15 samples ON, 12 samples OFF, 15 samples ON, 55 samples OFF
void triplet()
{
  	int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(220);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(220);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(1250);
//	printf("3");
//Divide all numbers by
//1.23529411765
//10 = 227us = 184us
//15 = 340us = 275 us
//45 = 1020us = 826 us
//55 = 1247us = 1009 us
}

void quadruplet()
{
//3 = 15 samples ON, 10 samples OFF, 15 samples ON, 10 samples OFF, 15 samples ON, 15 samples ON, 10 samples OFF. 55 samples OFF
  	int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(184);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(184);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
	usleep(184);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(1250);
//	printf("4");
}


void quintuplet()
{
  	int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(184);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(184);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
	usleep(184);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
	usleep(184);
        GPIO_CLR = 1<<g;
        usleep(231);
        GPIO_SET = 1<<g;
        usleep(1250);
//	printf("5");
}

//Divide all numbers by
//1.23529411765
//10 = 227us = 184us
//15 = 340us = 275 us
//45 = 1020us = 826 us
//55 = 1247us = 1009 us
//0= 15 samples ON then 45 samples OFF
//1= 45 samples ON then 15 samples OFF

void intro_1()
{
       int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(847);
        GPIO_SET = 1<<g;
        usleep(300);
//	printf("1");
}
void intro_0()
{
       int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(253);
        GPIO_SET = 1<<g;
        usleep(970);
//	printf("0");
}

