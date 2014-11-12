CC=g++

CCFLAGS= -Wall -I./include/
CFLAGS= -Wall -lgps -lcurl -lGeographic `pkg-config --libs opencv`
#CFLAGS= -Wall -I./include/

executavel: main.o gpsdevice.o gopro.o photo.o photohandler.o targetarea.o
	$(CC) $(CFLAGS) main.o gpsdevice.o gopro.o photo.o photohandler.o targetarea.o -o executavel

main.o: ./source/main.cpp
	$(CC) $(CCFLAGS) -c ./source/main.cpp

gpsdevice.o: ./source/GPSDevice/gpsdevice.cpp ./include/gpsdevice.hpp ./include/common.hpp
	$(CC) $(CCFLAGS) -c ./source/GPSDevice/gpsdevice.cpp

gopro.o: ./source/GoPro/gopro.cpp ./include/gopro.hpp ./include/common.hpp
	$(CC) $(CCFLAGS) -c ./source/GoPro/gopro.cpp

photo.o: ./source/GoPro/photo.cpp ./include/photo.hpp ./include/common.hpp
	$(CC) $(CCFLAGS) -c ./source/GoPro/photo.cpp

photohandler.o: ./source/PhotoHandler/photohandler.cpp ./include/photohandler.hpp
	$(CC) $(CCFLAGS) -c ./source/PhotoHandler/photohandler.cpp

targetarea.o: ./source/TargetArea/targetarea.cpp ./include/targetarea.hpp ./include/common.hpp
	$(CC) $(CCFLAGS) -c ./source/TargetArea/targetarea.cpp

clean:
	rm -rf *.o executavel
