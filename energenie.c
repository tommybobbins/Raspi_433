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
//  Modified for Energenie Remotes 1-Jun-2014
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

void energenie_set1();
void energenie_plug1();
void energenie_plug2();
void energenie_plug3();
void energenie_plug4();
void energenie_master();
void energenie_on();
void energenie_off();
void output_0();
void output_1();

int main(int argc, char *argv[])
{ 
  int g,rep;
  int plug, set;
  int receiver;
  char *on_or_off = NULL;
 
  // Set up gpi pointer for direct register access
  void setup_io();

  set = atoi(argv[1]); 
  plug = atoi(argv[2]);
  on_or_off = argv[3];  

//test suitable
 if  ((set<=2) && (set>=0))
  { 
//    printf("set is %d, plug is %d switching %s\n",set, plug,on_or_off); 
  }
 else
  {
     printf("energenie [set] [plug] [on|off] where set is 0..2, plug is 1..4 and on or off\n");
     abort();
  }
   
 if ((plug<=9) && (plug>=1))
  {
//    printf("set is %d, plug is %d switching %s\n",set,plug,on_or_off); 
    
  }
 else
  {
     printf("energenie [set] [plug] [on|off] where set is 1..2, plug is 1..4 or 9 for master and on or off\n");
     abort();
  } 

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
 

  for (rep=0; rep<4; rep++)
  //recall INVERSION out a 1 to ge a zero!
  {
        g=18;
     {
        GPIO_SET = 1<<g;
        bcm2835_delay(10);
  //above is gap between burst

    	if (set == 0){
           energenie_set1();
	} else if (set == 1) {
           energenie_set1();
	} else {
           energenie_set1();
	}


        if (plug == 1){
            energenie_plug1();
        } else if (plug == 2){
            energenie_plug2();
        } else if (plug == 3){
            energenie_plug3();
        } else if (plug == 4){
            energenie_plug4();
        } else if (plug == 9){
            energenie_master();
        } else {
            printf ("Something has gone horribly wrong in the plug selection");
        }
 

//########################################################
	if (strcmp(on_or_off,"on")==0){	
          energenie_on();
/*        # It is on */

	} else if (strcmp(on_or_off,"off") == 0) {
          energenie_off();
/*        # It is off */
        }
// ##################################################################
} 
/*   printf ("loop done\n");*/
}
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
/*
11010011000010100000 111 10 	  1on   4+8+16   +2  = (28 + 2)
11010011000010100000 111 00       1off  4+8+16       = (28)
11010011000010100000 011 10       2on   4+8      +2  = (12)
11010011000010100000 011 00       2off  4+8          = (12)
11010011000010100000 101 10       3on   4+16     +2  =  20
11010011000010100000 101 00       3off  4+16         =  20
11010011000010100000 001 10       4on   4        +2  =  4
11010011000010100000 001 00       4off  4            =  4
11010011000010100000 110 10       master on 8+16   +2   24
11010011000010100000 110 00       master off 8+16       24 
*/
void energenie_set1 () {
output_1();
output_1();
output_0();
output_1();
output_0();
output_0();
output_1();
output_1();
output_0();
output_0();
output_0();
output_0();
output_1();
output_0();
output_1();
output_0();
output_0();
output_0();
output_0();
output_0();
}

void energenie_plug1 () {
output_1();
output_1();
output_1();
}

void energenie_plug2 () {
output_0();
output_1();
output_1();
}


void energenie_plug3 () {
output_1();
output_0();
output_1();
}
void energenie_plug4 () {
output_0();
output_0();
output_1();
}

void energenie_master () {
output_1();
output_1();
output_0();
}

void energenie_on(){
output_1();
output_0();
}

void energenie_off(){
output_0();
output_0();
}

void output_1()
{
/*28 high = 18.9 * 28 = 529us*/
/*8 low =  18.9 * 8 =    151us*/
       int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(531);
        GPIO_SET = 1<<g;
        usleep(150);
}

void output_0()
{
/*10 high = 18.9 * 10 = 189us*/
/*26 low =  18.9 * 26 = 491us*/

  	int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(189);
        GPIO_SET = 1<<g;
        usleep(492);
}
