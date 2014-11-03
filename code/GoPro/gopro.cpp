/*
*	GoPro Class Source File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
*	@Date: October, 2014.
*/


#include "gopro.h"

const char * TAKE_PIC_URL = "10.5.5.9/bacpac/SH?t=goprohero&p=%01";
const char * GET_IMG_URL = "10.5.5.9:8080/DCIM/100GOPRO/";
const char * FILENAME = "IMAGES/GPR";

using namespace std;

GoPro::GoPro(short id)
{
	PHOTO_ID = id;
}
	

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

size_t GoPro::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void GoPro::getImage(short id)
{		
	char char_id = (char) id;
	
	cout << "id: " << id << endl;
	
	char url[strlen(GET_IMG_URL)];
	strcpy(url, GET_IMG_URL);
	cout << "url: " << url << endl;
	
	strcat(url, &char_id);
	cout << "url: " << url << endl;
	
	char filename[strlen(FILENAME)];
	strcpy(filename, FILENAME);	
	cout << "filename: " << filename << endl;
	
	strcat(filename, &char_id);
	cout << "filename: " << filename << endl;
	
	curl = curl_easy_init();
	assert(curl != NULL);
	
	fp = fopen(filename, "wb");
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &GoPro::write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	res = curl_easy_perform(curl);
	/* always cleanup */
	curl_easy_cleanup(curl);
	fclose(fp);

}


