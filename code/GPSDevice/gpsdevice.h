/*
*	GPS Device Header File.
*	@Author: ANDERSON OLIVEIRA SOUSA
* 	@UNIVERSIDADE FEDERAL DE GOIAS - ENGENHARIA DE COMPUTACAO
*	@Date: October, 2014.
*/


class GPSDevice
{
	private:
		int latitude;
		int longitude;
		short altitude;
		
	public:
		void open_conn();
		void read_data();
		void wait_powering_on();
		void update();
		int getLatitude();
		int getLongitude();
		short getAltitude();
	
}
