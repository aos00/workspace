/*
*	GoPro Class Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
*	@Date: October, 2014.
*/

#include <assert.h>
#include "gopro.h"

const char * TAKE_PIC_URL = "10.5.5.9/bacpac/SH?t=goprohero&p=%01";

void GoPro::takePicture()
{
	curl = curl_easy_init();
	assert(curl != NULL);
   	curl_easy_setopt(curl, CURLOPT_URL, TAKE_PIC_URL);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	res = curl_easy_perform(curl);

	if(res != CURLE_OK)
	      fprintf(stderr, "ERRO - takePicture(): curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	curl_easy_cleanup(curl);
}

void GoPro::getImage(short id)
{
}


