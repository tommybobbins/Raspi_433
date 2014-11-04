//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//  Played with by Paschal noisepower 30 Oct 2012
//  29th Nov 2012 modified by out as a silly string the ON command
//  will be compiled to an exe called ON 
//  Modified tng@chegwin.org 30-Jul-2013
//  Sniffed the remote codes and replicated inside the C.


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
void singlet();
void doublet();
void triplet();
void quadruplet();
void intro_0();
void intro_1();
void chapter_1();
void chapter_2();

int main(int argc, char *argv[])
{ 
  int g,rep,loop1,loop2;
  int plug;
  int receiver;
  char *on_or_off = NULL;
 
  // Set up gpi pointer for direct register access
  setup_io();

  
//  plug = atoi(argv[1]);
//  on_or_off = argv[2];  

   
  // Set up gpi pointer for direct register access
  setup_io();

  // Switch GPIO 7..11 to output mode

 /************************************************************************\
  * You are about to change the GPIO settings of your computer.          *
  * Mess this up and it will stop working!                               *
  * It might be a good idea to 'sync' before running this program        *
  * so at least you still have your code changes written to the SD-card! *
 \************************************************************************/

  // Set GPIO pin 18 to output
      INP_GPIO(18);
      OUT_GPIO(18);
	g=18;		
        GPIO_SET = 1<<g;
        bcm2835_delay(1000);
	GPIO_CLR = 1<<g;
        bcm2835_delay(1000);
        GPIO_SET = 1<<g;
        bcm2835_delay(10);
 				
 

  for (rep=0; rep<7; rep++)
  //recall INVERSION out a 1 to ge a zero!
  {
        g=18;
        GPIO_SET = 1<<g;
        bcm2835_delay(10);
	usleep(4000);
       
	for (loop1=0; loop1<7; loop1++)
	{
        usleep(3510);
        intro_1();
        intro_1();
        intro_1();
        intro_0();
        intro_0();
        intro_1();
        intro_0();
        intro_0();
        intro_1();
        intro_0();
        intro_1();
        intro_1();
        intro_1();
        intro_1();
        intro_0();
        intro_1();
        intro_1();
        intro_0();
        intro_0();
        intro_0();
        intro_0();
        intro_1();
        intro_0();
        intro_1();
        intro_1();
        intro_0();
        intro_0();
        intro_1();
        intro_0();
//	printf("\n");
	}
/////////////////////////////End of Intro
//120 samples
//        222 222 222 13212 312 223 213 132 213 2122
//480 samples
//        1 15 samples long
//120 samples
//        222 222 222 13212 312 223 213 132 213 2122
//480 samples
//        1 15 samples long
//120 samples
//        222 222 222 13212 312 223 213 132 213 2122
//480 samples
//        1 15 samples long
//400 samples
//        114 111 411 123 113 313 222 221 321 13112
//400 samples
//        114 111 411 123 113 313 222 221 321 13112
//400 samples
//        114 111 411 123 113 313 222 221 321 13112
//
	usleep(2000);
	chapter_1();
	usleep(8000);

	singlet();
//	printf("\n");
	usleep(2000);
	chapter_1();
	usleep(8000);
	singlet();
//	printf("\n");
	usleep(2000);
        chapter_1();
	usleep(8000);
	singlet();
//	printf("\n");
	usleep(8000);


	chapter_2();
	usleep(8800);

	chapter_2();
	usleep(8000);

	chapter_2();
	usleep(8000);

//0100000100010101000101010
	for (loop2=0; loop2<5; loop2++){
		intro_0();	
		intro_1();	
		intro_0();	
		intro_0();	
		intro_0();	
		intro_0();	
		intro_0();	
		intro_1();	
		intro_0();	
		intro_0();	
		intro_0();	
		intro_1();	
		intro_0();	
		intro_1();	
		intro_0();	
		intro_1();	
		intro_0();	
		intro_0();	
		intro_0();	
		intro_1();	
		intro_0();	
		intro_1();	
		intro_0();	
		intro_1();	
		intro_0();	
	        usleep(8000);
//		printf("\n");

	}	

//480 samples
//       1 15 samples long
//400 samples
//	114 111 411 123 113 313 222 221 321 13112
//400 samples
//        114 111 411 123 113 313 222 221 321 13112
//400 samples
//        114 111 411 123 113 313 222 221 321 13112
//440 samples
//5 * 0100000100010101000101010


	
  }

  return 0;

} // main


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

void chapter_1()
{
        doublet();
        doublet();
        doublet();

        doublet();
        doublet();
        doublet();

        doublet();
        doublet();
        doublet();

        singlet();
        triplet();
        doublet();
        singlet();
        doublet();

        triplet();
        singlet();
        doublet();

        doublet();
        doublet();
        triplet();

        doublet();
        singlet();
        triplet();

        singlet();
        triplet();
        doublet();

        doublet();
        singlet();
        triplet();

        doublet();
        singlet();
        doublet();
        doublet();
//	printf("\n");
}

void chapter_2(){

	singlet();
	singlet();
	quadruplet();

	singlet();
	singlet();
	singlet();

	quadruplet();
	singlet();
	singlet();

	singlet();
	doublet();
	triplet();

	singlet();
	singlet();
	triplet();

	triplet();
	singlet();
	triplet();

	doublet();
	doublet();
	doublet();

	doublet();
	doublet();
	singlet();

	triplet();
	doublet();
	singlet();

	singlet();
	triplet();
	singlet();
	singlet();
	doublet();
//	printf("\n");
}	
