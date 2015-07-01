
// Blinks on RPi Plug P1 pin 12 (which is GPIO pin 18)
#define PIN RPI_GPIO_P1_12


#include <bcm2835.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void pulse_send (int pulseon, int pulseoff)
{
	bcm2835_gpio_write(PIN, HIGH);
        usleep(pulseon);
/*	printf("Pulseon %i \t" ,pulseon); */
	bcm2835_gpio_write(PIN, LOW);
        usleep(pulseoff);
/*	printf("Pulseoff %i \n",pulseoff); */
}

