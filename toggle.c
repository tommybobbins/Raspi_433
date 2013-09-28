//  www.noisepower.co.uk
//  This is NOT copyright - offered as is with no warranty at all
//  Care is needed - this turns ON or OFF wireless mains receiver for home automation
//  DO not work on live mains or use this program to provide safety isolation
//  This code is written for a TXDver1.2  home automation board.
//  Other boards are possible- it drives the ARM GPIO pin 18 on /off to do so.
//  The code for controlling the GPIO is based upon an Example program dated 15-January-2012
//  written by Dom and Gert and posted on raspberry pi foundation forum  (appeared to invite copying)
//  note to compile this code need to install the library  bcm2835.h
//  We use the gcc compile supplied with debian with command line
//  gcc -o toggle -l rt /home/pi/hello_code/toggle.c -l bcm2835
//  toggle is the executable mafe toggle.c is this file (and hello_code is directory it is in)
//  need to do this in the root directory, with bcm2835.h there, as root (ie after sudi -i)
//  this is written for the DB receivers noisepower sell.
//  The receiver cannot "learn".  Each is numbered 1 to 5 (stamped on front)  There are 16 possible sets of 1 to 5   
//  thus the total possible codes is 80
//  The program can be called up toggle n k  (where n is 0 to 15, k is 1 to 5)
//  There is a python script on the noisepower website downloads, which calls up this program with all possible 
//  80 combinations.   This can be used if you have lost what set your receivers are in.
//  it is possible to toggle x 3 (eg if you have nmuber 3 ,but do not know which set), or toggle 3, x
//  
//  This code is only offered as an examples which worked in the testing we did.
//
//  24th Feb 2013 tested working.  
//  TO change timings of 1s and zeros you need to edit output_1() and output_0().   
//  You are invited to improve on this code, or use as a staring point for another task
//  and post on forum or back to noisepower.   
//  Please contact sales@noisepower.co.uk for details of where to purchase the board TXDver1.2 



// Access from ARM Running Linux

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

void setup_io();
void output_1();
void output_0();

int main(int argc, char **argv)
{ 
  int g,rep,x,y,loop,fixed;
  int set, mask, receiver_bin;
  int receiver;
  
  // Set up gpi pointer for direct register access
  setup_io();
  
  set = atoi(argv[1]);
  receiver = atoi(argv[2]);  

   
//test suitable
 if (((set<=15) && (set>=0)) && ((receiver>=0)&&(receiver <=5)))
  {
//    printf("set is %d  receiver is %d\n",set,receiver); 
    
  }
 else
  {
     printf("invalid arguments - toggle [n] [k] where n is 0..15 and k is 1..5\n");
     printf(" n represents the set the receiver is in, k is the number stamped on receiver\n");
  } 


  // Set GPIO pin 18 to output
      INP_GPIO(18);
      OUT_GPIO(18);
   
  //need to generate x from set and receiver 

     set=set<<21;                                                                                                                        
     receiver_bin=3<<(2*receiver-1);
 //  printf("set now equals %d\n",set);
 //  printf("receiver now equals %d\n",receiver_bin);
     fixed=0b110101010100000000000;
 //  printf("fixed is %d\n",fixed);
     x=(set|fixed|receiver_bin);
 //  printf("x is %d\n",x); 
 
  for (rep=0; rep<16; rep++)
  //recall INVERSION out a 1 to get a zero!
  { 
        g=18;
        mask=16777216;
        GPIO_SET = 1<<g;
        bcm2835_delay(5);

  //above is gap between burst
  //for DBtech this is 5ms
        
       for (loop=0; loop<25; loop++)
   { 
    if (x & mask)
    {
     output_1();
    }
    else
    {
     output_0(); 
    }
    mask=mask>>1;
   }  //end of 25 loop 

  }   // end of 16 loop


  return 0;

} // main

void output_1()
{
       int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(540);
        GPIO_SET = 1<<g;
        usleep(40);
}

void output_0()
{
  	int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(160);
        GPIO_SET = 1<<g;
        usleep(370);
}



//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io

