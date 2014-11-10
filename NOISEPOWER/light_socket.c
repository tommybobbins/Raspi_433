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
//  Modified tng@chegwin.org 26-Sep-2013
//  Boiler on/off for Drayton/British Gas boilers
//  Each boiler is individual and needs correctly sniffing


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
int sampletime;
int standard_unit;

#define STANDARD_UNIT 22 //Units are 23/46 sampletimes long
#define SAMPLE_TIME 20 //22.67 microseconds
#define UNIT_INTERVAL (STANDARD_UNIT*SAMPLE_TIME)

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

void setup_io();
void pulse_send (int, int);

int main(int argc, char *argv[])
{ 
  int g,rep;
  char *on_or_off = NULL;
 
  // Set up gpi pointer for direct register access
  setup_io();

  on_or_off = argv[1];  

if (!(on_or_off))
   {
	printf("Usage %s on|off \n",argv[0]);
        exit(2);	
   }


//test suitable

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
      bcm2835_delay(100);

  for (rep=0; rep<10; rep++)
  //recall INVERSION out a 1 to ge a zero!
  {
//	printf("Hello");

//  1  868uS    304uS  -24.482kHz
//  2  868uS    750uS  -25.634kHz
//  3  364uS    304uS  -28.159kHz
//  4  868uS    296uS  -25.058kHz
//  5  872uS    796uS  -24.369kHz
//  6  368uS    804uS  -27.853kHz
//  7  368uS    296uS  -26.495kHz
//  8  876uS    796uS  -25.970kHz
//  9  372uS    296uS  -25.538kHz
// 10  872uS    296uS  -23.796kHz
// 11  868uS    275uS  -24.194kHz
// 12  872uS    296uS  -26.089kHz
// 13  872uS    796uS  -23.222kHz
// 14  376uS    292uS  -27.926kHz
// 15  868uS    296uS  -24.770kHz
// 16  876uS    275uS  -23.116kHz
// 17  868uS    296uS  -24.770kHz
// 18  868uS    304uS  -25.634kHz
// 19  868uS    296uS  -24.770kHz
// 20  872uS    296uS  -26.089kHz
// 21  872uS    275uS  -26.089kHz
        pulse_send(750,275);
        pulse_send(750,750);
        pulse_send(220,275);
        pulse_send(750,275);
        pulse_send(750,750);
        pulse_send(220,750);
        pulse_send(220,275);
        pulse_send(750,750);
        pulse_send(220,275);
        pulse_send(750,275);
        pulse_send(750,275);
        pulse_send(750,275);
        pulse_send(750,750);
        pulse_send(220,275);
        pulse_send(750,275);
        pulse_send(750,275);
        pulse_send(750,275);
        pulse_send(750,275);
        pulse_send(750,275);
        pulse_send(750,275);
        pulse_send(750,275);

// 22  868uS    804uS  -24.770kHz
// 23  364uS    275uS  -22.665kHz
// 24  872uS    296uS  -22.936kHz
// 25  872uS   5804uS  -24.369kHz


        // End of common code
  //above is gap between burst
	if (strcmp(on_or_off,"off")==0){	
             pulse_send(750,750);
             pulse_send(220,275);
             pulse_send(750,275);
             pulse_send(750,5000);
//             printf("Finished sending off\n");
	} else if (strcmp(on_or_off,"on")==0) {
             pulse_send(750,750);
             pulse_send(220,275);
             pulse_send(750,275);
             pulse_send(750,5000);
//            printf("Finished sending on\n");
	} else if (strcmp(on_or_off,"1")==0) {
//2 2  1 2 2  1  1 2  1 2 2 2 2  1 2 2 2 2 2 2 2  1  1 2 2
             usleep(450);
             pulse_send(220,750);
             pulse_send(220,275);
             pulse_send(750,275);
             pulse_send(750,5000);
//            printf("Finished sending on\n");
	} else if (strcmp(on_or_off,"2")==0) {
//2 2  1 2 2  1  1 2  1 2 2 2 2  1 2 2 2 2 2 2 2  1  2 2 2
             usleep(450);
             pulse_send(220,750);
             pulse_send(750,275);
             pulse_send(750,275);
             pulse_send(750,5000);
//            printf("Finished sending on\n");
	} else if (strcmp(on_or_off,"3")==0) {
//2 2  1 2 2  1  1 2  1 2 2 2 2  1 2 2 2 2 2 2 2  2  1 1 2
             usleep(450);
             pulse_send(750,750);
             pulse_send(220,275);
             pulse_send(220,275);
             pulse_send(750,5000);
//            printf("Finished sending on\n");
	} else if (strcmp(on_or_off,"left")==0) {
//2 2  1 2 2  1  1 2  1 2 2 2 2  1 2 2 2 2 2 2 2  1 2  1 2
             usleep(450);
             pulse_send(220,275);
             pulse_send(750,750);
             pulse_send(220,275);
             pulse_send(750,5000);
	} else if (strcmp(on_or_off,"right")==0) {
//2 2  1 2 2  1  1 2  1 2 2 2 2  1 2 2 2 2 2 2 2  2  2 1 1
             usleep(450);
             pulse_send(750,750);
             pulse_send(750,275);
             pulse_send(220,275);
             pulse_send(220,5000);
        } else {
	
        }
//        printf ("Sleeping\n");	
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


void pulse_send (int pulseon, int pulseoff)
{
	int g;
        g=18;
        GPIO_CLR = 1<<g;	
        usleep(pulseon);
//	printf("Pulseon %i \t" ,pulseon);
        GPIO_SET = 1<<g;	
        usleep(pulseoff);
//	printf("Pulseoff %i \n",pulseoff);
//	printf("1");
}
