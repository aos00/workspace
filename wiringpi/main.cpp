#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <pthread.h>

using namespace std;

// Use GPIO Pin 17, which is Pin 0 for wiringPi library

#define BUTTON_PIN 0
#define DEBOUNCING_TIME 1

// the event counter 
volatile int eventCounter = 0;
volatile int id = 0;
unsigned int time1 = 0;
unsigned int time2 = 0;;

// -------------------------------------------------------------------------
// myInterrupt:  called every time an event occurs

stru

void *thread_fn(int *id){
	int td_id = (int) *id;
	printf("Thread %i HERE!\n",td_id);
	eventCounter++;
	pthread_exit(NULL);
}


void myInterrupt(void) {
	time2 = millis();
	if((time2 - time1) < DEBOUNCING_TIME)
		return;
	time1 = time2;
	pthread_t thread;
	id++;
	int rc = pthread_create(&thread, NULL, thread_fn,(void *) &id);
//	int x = piThreadCreate(myThread);
	if(rc)
		perror("it didnt start!\n");
}



// -------------------------------------------------------------------------
// main
int main(void) {
  // sets up the wiringPi library
  if (wiringPiSetup () < 0) {
      fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
      return 1;
  }

  // set Pin 17/0 generate an interrupt on high-to-low transitions
  // and attach myInterrupt() to the interrupt
  if ( wiringPiISR (BUTTON_PIN, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
      fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }

  // display counter value every second.
  while ( 1 ) {
    printf( "%d\n", eventCounter );
    eventCounter = 0;
	id = 0;
    delay( 3000 ); // wait 1 second
  }

  return 0;
}
