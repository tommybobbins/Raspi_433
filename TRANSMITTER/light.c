#include <bcm2835.h>

// Blinks on RPi Plug P1 pin 12 (which is GPIO pin 18);
#define PIN RPI_GPIO_P1_12


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void light_main();
void light_on();
void light_off();
void pulse_send();

int main(int argc, char *argv[])
{ 
 
  char *on_or_off = NULL;
  on_or_off = argv[1];  
  int rep;

  if (!bcm2835_init())
  {
        printf("Failed to init bcm2835\n");
  	return 1;
  }
  // Set the pin to be an output
  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);


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
	bcm2835_gpio_write(PIN, LOW);
        usleep(8000);
  //above is gap between burst

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
	bcm2835_gpio_write(PIN, LOW);
} // End of Loop
        bcm2835_close();
} // End of Main
