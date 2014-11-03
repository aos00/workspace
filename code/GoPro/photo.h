/*
*	Photo header File.
*	@Author: ANDERSON OLIVEIRA SOUSA.
*	@Date: October, 2014.
*/

class Photo
{
	private:
		char *filename;
		short id;
		int  latitude;
		int longitude;
		short altitude;
		FILE data;
		
	public:
		Photo(short,int,int, *data_stream); //Constructor parameters: ID, latitude and longitude, file data.
}

