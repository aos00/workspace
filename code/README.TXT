COMO COMPILAR OS ARQUIVOS:
sudo g++ -Wall -o "%e"  -L/usr/lib/arm-linux-gnueabihf -lcurl "main.cpp" "GoPro/gopro.cpp" "GoPro/photo.cpp" "GPSDevice/gpsdevice.cpp" -l gps
