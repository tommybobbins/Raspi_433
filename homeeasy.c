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
void chapter1_set1();
void chapter1_set2();

int main(int argc, char *argv[])
{ 
  int g,rep;
  int plug, set;
  int receiver;
  char *on_or_off = NULL;
 
  // Set up gpi pointer for direct register access
  setup_io();

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
     printf("homeeasy [set] [plug] [on|off] where set is 1..2, plug is 1..4 and on or off\n");
     abort();
  }
   
 if ((plug<=4) && (plug>=1))
  {
    printf("set is %d, plug is %d switching %s\n",set,plug,on_or_off); 
    
  }
 else
  {
     printf("homeeasy [set] [plug] [on|off] where set is 1..2, plug is 1..4 and on or off\n");
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
 

  for (rep=0; rep<12; rep++)
  //recall INVERSION out a 1 to ge a zero!
  {
        g=18;
     {
        GPIO_SET = 1<<g;
        bcm2835_delay(10);
  //above is gap between burst

	if (set == 1){
	chapter1_set1();
	} else if (set ==2) {
	chapter1_set2();
	} else {
	chapter1_set1();

	}

 

// 213 is ON ########################################################
// 222 is OFF
	if (strcmp(on_or_off,"on")==0){	
	doublet();
	singlet();
	triplet();

	} else if (strcmp(on_or_off,"off")==0) {
	doublet();
	doublet();
	doublet();
	} else if (strcmp(on_or_off,"mon")==0) {
	singlet();
	doublet();
	triplet();
	} else if (strcmp(on_or_off,"moff")==0) {
	singlet();
	triplet();
	doublet();
	} else  {
	printf("No on or off given. Exiting\n");
	abort();
	}
// ##################################################################
//1ON =  1gap22213131232221312231313123 213 2221
//1OFF = 1gap22213131232221312231313123 222 2221
//2ON =  1gap22213131232221312231313123 213 2212
//2OFF = 1gap22213131232221312231313123 222 2212
//3ON =  1gap22213131232221312231313123 213 2131
//3OFF = 1gap22213131232221312231313123 222 2131
//MASTER ON =                           123 2221
//MASTER OFF =				132 2221
if (plug == 1){
	doublet();
	doublet();
	doublet();
	singlet();
} else if (plug == 2) {
	doublet();
	doublet();
	singlet();
	doublet();
} else if (plug ==3) {
	doublet();
	singlet();
	triplet();
	singlet();
} else {
	printf("Can't find plug number\n");
	abort();
	}

   }
//	printf("\n");
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


void chapter1_set1 () {
	singlet();
        usleep(1200);
//	printf("1long"); 
//1 long sleep 
	doublet();
	doublet();
	doublet();
	singlet();
	triplet();
	singlet();
	triplet();
	singlet();
	doublet();
	triplet();
	doublet();
	doublet();
	doublet();
//ON =  1gap22213 131232 221 31223131 3123 213 2221
//NEW   1gap22212 231322 212 23132213 1313 213 2221
	singlet();
	triplet();
	singlet();
	doublet();
	doublet();
	triplet();
	singlet();
	triplet();
	singlet();
	triplet();
	singlet();
	doublet();
	triplet();
}

void chapter1_set2 () {
	singlet();
        usleep(1200);
//	printf("1long"); 
//1 long sleep 
	doublet();
	doublet();
	doublet();
	singlet();
	doublet();

	doublet();
	triplet();
	singlet();
	triplet();
	doublet();
	doublet();

	doublet();
	singlet();
	doublet();
//ON =  1gap22213 131232 221 31223131 3123 213 2221
//NEW   1gap22212 231322 212 23132213 1313 213 2221

	doublet();
	triplet();
	singlet();
	triplet();
	doublet();
	doublet();
	singlet();
	triplet();

	singlet();
	triplet();
	singlet();
	triplet();
}
