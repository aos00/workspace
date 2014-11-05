COMO COMPILAR OS ARQUIVOS NO GEANY
sudo g++ -Wall -o "%e"  -L/usr/lib/arm-linux-gnueabihf -lcurl "main.cpp" "GoPro/gopro.cpp" "GoPro/photo.cpp" "GPSDevice/gpsdevice.cpp" -l gps


COMPILAR NA LINHA DE COMANDO:
g++ -I /home/pi/raspi_local_repo/code/include/ main.cpp GoPro/gopro.cpp GoPro/photo.cpp GPSDevice/gpsdevice.cpp -o saida.out -l curl -l gps

