#include <bcm2835.h>

// Blinks on RPi Plug P1 pin 12 (which is GPIO pin 18);
#define PIN RPI_GPIO_P1_12


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void bgas_main();
void bgas_on();
void bgas_off();
void pulse_send();

int main(int argc, char *argv[])
{ 
 
  if (!bcm2835_init())
  {
        printf("Failed to init bcm2835\n");
  	return 1;
  }
  // Set the pin to be an output
  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);

  int rep;
  char *on_or_off = NULL;
 
  on_or_off = argv[1];  

  if (!(on_or_off))
     {
    	printf("Usage %s on|off \n",argv[0]);
        exit(2);	
     }

        usleep(500);
	bcm2835_gpio_write(PIN, LOW);
        usleep(8000);
  for (rep=0; rep<10; rep++);
  //recall INVERSION out a 1 to ge a zero!
  {
     {
	bcm2835_gpio_write(PIN, LOW);
        usleep(8000);
  //above is gap between burst

        bgas_main(); 

//########################################################
	if (strcmp(on_or_off,"on")==0){	
          bgas_on();
/*        # It is on */

	} else if (strcmp(on_or_off,"off") == 0) {
          bgas_off();
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

//Received 376 bytes  Taken from https://github.com/jimstudt/ook-decoder
//0000207.445396s ###  30 pulses (this is an off);
//num high   low      freq
//  1  464uS    464uS  -10.776kHz
//  2  564uS    456uS  -13.741kHz
//  3  584uS    444uS  -17.123kHz
//  4  552uS    464uS  -13.134kHz
//  5  480uS    464uS  -12.500kHz
//  6 890uS    968uS  -13.194kHz
//  7  480uS    972uS  -10.268kHz
//  8 1076uS    380uS  -11.385kHz
//  9  564uS    964uS  -14.184kHz
// 10  564uS    484uS  -10.638kHz
// 11  536uS    468uS  -11.194kHz
// 12  552uS    464uS  -11.775kHz
// 13  568uS    456uS  -13.204kHz
// 14  480uS    468uS  -12.054kHz
// 15  552uS    468uS  -11.775kHz
// 16 1076uS    968uS  -13.708kHz
// 17  480uS    464uS   -6.696kHz
// 18  556uS    464uS  -15.288kHz
// 19  564uS    380uS  -17.287kHz
// Differs from here Specific for Off
// 20  480uS    464uS  -10.714kHz
// 21  568uS    456uS  -10.563kHz
// 22  480uS    380uS  -13.839kHz
// 23  480uS    464uS  -13.839kHz
// 24  480uS    380uS  -11.607kHz
// 25 1084uS    964uS  -12.223kHz
// 26  556uS    464uS   -7.644kHz
// 27  564uS    472uS  -13.741kHz
// 28 1060uS    464uS  -12.736kHz
// 29  576uS    448uS  -15.191kHz
// 30  568uS   8004uS  -11.444kHz


void bgas_main () {
pulse_send(380,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(890,800);
pulse_send(480,800);
pulse_send(890,380);
pulse_send(480,800);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(890,800);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
}

void bgas_on(){
pulse_send(890,800);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(890,800);
pulse_send(480,380);
pulse_send(890,380);
pulse_send(480,380);
pulse_send(480,6300);
pulse_send(28,8004);
}

void bgas_off(){
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(890,800);
pulse_send(480,380);
pulse_send(480,380);
pulse_send(890,380);
pulse_send(480,380);
pulse_send(480,8000);
}

/*668uS 168uS*/
/* 550 130 */


/*256uS 580uS*/
/*140 540*/

void pulse_send (int pulseon, int pulseoff)
{
	bcm2835_gpio_write(PIN, HIGH);
        usleep(pulseon);
/*	printf("Pulseon %i \t" ,pulseon); */
	bcm2835_gpio_write(PIN, LOW);
        usleep(pulseoff);
/*	printf("Pulseoff %i \n",pulseoff); */
}

