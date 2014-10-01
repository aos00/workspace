#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include <gps.h>
#include <errno.h>
#include <math.h>

static char *server = "localhost";
static char *port = "2947";
static char scr[128];

struct gps_data_t GPS_DATA;

int main(){

	if(gps_open(server,port,&GPS_DATA) != 0)
	{
		(void) fprintf(stderr, "gps_open: gpsd nao esta rodando ou houve um network error! %d, %s\n", errno, gps_errstr(errno));
		exit(EXIT_FAILURE);
	}

	(void) gps_stream(&GPS_DATA, WATCH_ENABLE | WATCH_JSON, NULL);
	while(1){
		if(gps_waiting(&GPS_DATA, 500)){ //gps_waiting checa se exite novos dados vindos do deamon. O segundo argumento é o tempo máximo de espera em microsegundos
			errno = 0;
			if(gps_read(&GPS_DATA) == -1){
				(void) fprintf(stderr, "gps_read: erro no socket: %d, %s\n", errno, gps_errstr(errno));
			}else{
				printf("\nStatus: %i", GPS_DATA.status);
				printf("\nLatitude: %f , Longitude: %f", fabs(GPS_DATA.fix.longitude), fabs(GPS_DATA.fix.latitude));
				printf("\nData: %s", unix_to_iso8601(GPS_DATA.fix.time, scr, sizeof(scr)));

			}
		}
	}

return 0;
}
