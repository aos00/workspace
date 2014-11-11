CC=g++

CFLAGS=-c -Wall -I./include/

executavel: main.o gpsdevice.o gopro.o photo.o photohandler.o targetarea.o
	$(CC) main.o gpsdevice.o gopro.o photo.o photohandler.o targetarea.o -o executavel

main.o: ./source/main.cpp
	$(CC) $(CFLAGS) -c ./source/main.cpp

gpsdevice.o: ./source/GPSDevice/gpsdevice.cpp ./include/gpsdevice.hpp
	$(CC) $(CFLAGS) -c ./source/GPSDevice/gpsdevice.cpp -lGeographic

gopro.o: ./source/GoPro/gopro.cpp ./include/gopro.hpp
	$(CC) $(CFLAGS) -c ./source/GoPro/gopro.cpp -lcurl

photo.o: ./source/Photo/photo.cpp ./include/photo.hpp
	$(CC) $(CFLAGS) -c ./source/Photo/photo.cpp

photohandler.o: ./source/PhotoHandler/photohandler.cpp ./include/photohandler.hpp
	$(CC) $(CFLAGS) `pkg-config --libs opencv` -c ./source/PhotoHandler/photohandler.cpp

targetarea.o: ./source/TargetArea/targetarea.cpp ./include/targetarea.hpp
	$(CC) $(CFLAGS) -c ./source/TargetArea/targetarea.cpp

clean:
	rm -rf *o executavel
