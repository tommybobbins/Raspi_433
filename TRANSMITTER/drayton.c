#include <bcm2835.h>

// Blinks on RPi Plug P1 pin 12 (which is GPIO pin 18);
#define PIN RPI_GPIO_P1_12


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void drayton_main();
void drayton_on();
void drayton_off();
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

  int rep=0;
  char *on_or_off = NULL;
 
  on_or_off = argv[1];  

  if (!(on_or_off))
     {
    	printf("Usage %s on|off \n",argv[0]);
        exit(2);	
     }

        usleep(500);
	bcm2835_gpio_write(PIN, LOW);
        usleep(50000);
  for (rep=0; rep<10; rep++)
  {
//        printf("Loop\n");
  //recall INVERSION out a 1 to ge a zero!
     {
	bcm2835_gpio_write(PIN, LOW);
        usleep(5000);
  //above is gap between burst
//        printf ("drayton main\n");
        drayton_main(); 

//########################################################
	if (strcmp(on_or_off,"on")==0){	
          drayton_on();
        /*        # It is on */

	} else if (strcmp(on_or_off,"off") == 0) {
          drayton_off();
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

//ON
//1 1 1 1 1 1 2  1 1 2 1 1  1 2 1  2  1 1 1 2  1 1 1 1 2 1  1 1 2
//OFF
//1 1 1 1 1 1 2  1 1 2 1 1  1 2 1  2  1 1 1 1 1 1 1 1 2  2  1 1 2
//1 is 36 samples long =  0.000816326530612  816uS
//2 is 55 samples long =  0.00124716553288  1247uS
//gap  10 samples long =  0.000226757369615  200uS (round down 200)
//lgap 33 samples long =  0.000748299319728  748uS
//488 Samples          =  0.0110657596372  11066uS 
//44100 Hz = 44100 samples per second


void drayton_main () {
//pulse_send(11066,200);

pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(880,1400);


pulse_send(472,453);
pulse_send(472,453);
pulse_send(880,453);
pulse_send(472,453);
pulse_send(472,874);

pulse_send(472,453);
pulse_send(880,468);
pulse_send(472,874);

pulse_send(880,874);

pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);

}

void drayton_on(){
//ON
//488 Samples 1 1 1 2  1 1 2 1 1  1 2 1  2  1 1 1 2  1 1 1 1 2 1  1 1 2
pulse_send(880,874);

pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(880,453);
pulse_send(472,874);

pulse_send(472,453);
pulse_send(472,453);
pulse_send(880,8004);

}

void drayton_off(){
//ON
//488 Samples 1 1 1 2  1 1 2 1 1  1 2 1  2  1 1 1 2  1 1 1 1 2 1  1 1 2
//OFF
//488 Samples 1 1 1 2  1 1 2 1 1  1 2 1  2  1 1 1 1 1 1 1 1 2  2  1 1 2
pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(472,453);
pulse_send(800,874);

pulse_send(800,874);

pulse_send(472,453);
pulse_send(472,453);
pulse_send(800,8004);
}





/* 
We want it to look like this

i0001010.393312s ###  29 pulses
num high   low      freq
  1  584uS    460uS   40.240kHz
  2  576uS    460uS   36.024kHz
  3  600uS    464uS   41.250kHz
  4  592uS    460uS   41.385kHz
  5  576uS    460uS   39.497kHz
  6  576uS    460uS   39.931kHz
  7 1012uS   1424uS   39.279kHz
  8  568uS    456uS   43.134kHz
  9  572uS    456uS   39.773kHz
 10 1012uS    456uS   39.032kHz
 11  564uS    456uS   38.564kHz
 12  564uS    896uS   38.564kHz
 13  572uS    452uS   39.336kHz
 14 1104uS    464uS   39.855kHz
 15  568uS    912uS   36.532kHz
 16 1016uS    904uS   40.354kHz
 17  572uS    464uS   37.150kHz
 18  572uS    460uS   39.336kHz
 19  572uS    464uS   37.587kHz
 20 1016uS    896uS   37.894kHz
 21  576uS    472uS   39.062kHz
 22  568uS    464uS   38.732kHz
 23  576uS    468uS   39.497kHz
 24  572uS    452uS   38.899kHz
 25 1008uS    452uS   38.442kHz
 26  568uS    892uS   40.053kHz
 27  572uS    452uS   39.773kHz
 28  568uS    464uS   39.173kHz
 29 1008uS   8004uS   39.187kHz



Here is what we get

0000920.851408s ###  29 pulses
num high   low      freq
  1  620uS    468uS  -35.887kHz
  2  600uS    484uS  -39.167kHz
  3  604uS    480uS  -37.252kHz
  4  600uS    480uS  -39.167kHz
  5  604uS    480uS  -36.838kHz
  6  612uS    472uS  -36.356kHz
  7 1016uS    904uS  -40.354kHz
  8  604uS    480uS  -38.079kHz
  9  608uS    476uS  -39.062kHz
 10 1020uS    472uS  -38.971kHz
 11  600uS    480uS  -38.333kHz
 12  604uS    900uS  -37.666kHz
 13  600uS    488uS  -38.333kHz
 14 1016uS    492uS  -38.386kHz
 15  608uS    896uS  -37.418kHz
 16 1016uS    896uS  -39.616kHz
 17  608uS    480uS  -39.474kHz
 18  608uS    472uS  -37.418kHz
 19  604uS    484uS  -38.079kHz
 20 1004uS    904uS  -38.347kHz
 21  612uS    472uS  -37.582kHz
 22  604uS    480uS  -36.838kHz
 23  616uS    476uS  -36.932kHz
 24  600uS    484uS  -37.500kHz
 25 1016uS    476uS  -38.386kHz
 26  604uS    904uS  -39.321kHz
 27  620uS    464uS  -37.903kHz
 28  600uS    484uS  -40.000kHz
 29 1012uS   8004uS  -38.043kHz



*/
