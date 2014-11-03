#include <wiringPi.h>
#include <assert.h>

#define GOPRO_ID1	7 //WPi 7, BCM 4, Physical 7
#define GOPRO_ID2	0 //WPi 0, BCM 17, Physical 11
#define GOPRO_ID3	5 //WPi 5, BCM 24, Physycal 18
#define GOPRO_ID4	6 //WPi 6, BCM 25, Physycal 20
#define GOPRO_TRIGGER	1 //WPi 1, BCM 18, Physical 12
#define GOPRO_PWRMODE	4 //WPi 4, BCM 23, Physical 16
#define BUZZER		22 //BUZZER
/*
*Metodo que define o estado inicial da camera
*/
void setEstadoInicial(){
	digitalWrite(GOPRO_TRIGGER, HIGH);
	digitalWrite(GOPRO_ID2, HIGH);
	digitalWrite(GOPRO_ID3, LOW);
}

void prepararDisparo(){
	digitalWrite(GOPRO_ID2, LOW);
	digitalWrite(GOPRO_ID3, HIGH);
}

void dispararFoto(){
	digitalWrite(GOPRO_TRIGGER, LOW);
	delay(1);
	digitalWrite(GOPRO_TRIGGER, HIGH);
}

void estadoInicial(){
	digitalWrite(GOPRO_ID2, HIGH);
	digitalWrite(GOPRO_ID3, LOW);
}

void teste(){
	while(1){
		digitalWrite(GOPRO_TRIGGER, HIGH); delay(500);
		digitalWrite(GOPRO_TRIGGER, LOW);
		digitalWrite(GOPRO_ID2, HIGH); delay(500);
		digitalWrite(GOPRO_ID2, LOW);
		digitalWrite(GOPRO_ID3, HIGH); delay(500);
		digitalWrite(GOPRO_ID3, LOW);
	}
}

int main (void){

	assert(wiringPiSetup() != -1);

	pinMode(GOPRO_TRIGGER, OUTPUT);
	pinMode(GOPRO_ID2, OUTPUT);
	pinMode(GOPRO_ID3, OUTPUT);

	//teste();

	setEstadoInicial();

	while(1){
//		prepararDisparo();
//		delay(1);
		dispararFoto();
//		delay(1);
//		estadoInicial();
		delay(5000);
	}
	return 0;
}


