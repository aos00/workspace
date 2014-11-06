#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
	printf("Starting GPS...\nChecking if processor is available...\n");
	int status = system(NULL);
	printf("Status: %i",status);

	if(status){
		printf("Processor available! Executing commmand to gpsd start\n");
		if(system("sudo gpsd /dev/ttyAMA0 -F /var/run/gpsd.sock") == 0)
			printf("GPSD running...");
		else
			perror("Error starting GPSD!\n");

	}else{ 
		perror("Processor not available!\n");
	}

}
