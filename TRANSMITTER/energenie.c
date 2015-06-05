#include <bcm2835.h>

// Blinks on RPi Plug P1 pin 12 (which is GPIO pin 18)
#define PIN RPI_GPIO_P1_12


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void energenie_set1();
void energenie_set2();
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
  if (!bcm2835_init())
  	return 1;

  // Set the pin to be an output
  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

  int rep;
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

        usleep(500);
	bcm2835_gpio_write(PIN, LOW);
        usleep(8000);
  for (rep=0; rep<10; rep++)
  //recall INVERSION out a 1 to ge a zero!
  {
     {
	bcm2835_gpio_write(PIN, LOW);
        usleep(8000);
  //above is gap between burst

    	if (set == 0){
           energenie_set1();
	} else if (set == 1) {
           energenie_set1();
	} else if (set == 2) {
           energenie_set2();
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
//   printf ("loop done\n");
}
	bcm2835_gpio_write(PIN, LOW);
        bcm2835_close();
        return 0;

} // main


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

void energenie_set2 (){
output_1();
output_0();
output_1();
output_1();
output_1();
output_1();
output_1();
output_1();
output_0();
output_0();
output_0();
output_1();
output_0();
output_0();
output_0();
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
/*668uS 168uS*/
       int g;
        g=18;
	bcm2835_gpio_write(PIN, HIGH);
        usleep(550);
	bcm2835_gpio_write(PIN, LOW);
        usleep(130);
//        printf("1");
}

void output_0()
{
/*256uS 580uS*/

	bcm2835_gpio_write(PIN, HIGH);
        usleep(140);
	bcm2835_gpio_write(PIN, LOW);
        usleep(540);
//        printf("0");
}
