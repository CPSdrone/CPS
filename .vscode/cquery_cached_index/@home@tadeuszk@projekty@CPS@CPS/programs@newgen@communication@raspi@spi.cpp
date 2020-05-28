#include <iostream>
#include <wiringPiSPI.h>
#include <string.h>
#include <unistd.h>
#include "../lib/connections.h"


#define SPI_CHANNEL 0 
#define SPI_CLOCK_SPEED 1000000 // 1MHz

int main(int argc, char **argv)
{
    snd_sock s;

/*
    int fd = wiringPiSPISetupMode(SPI_CHANNEL, SPI_CLOCK_SPEED, 0);
    if (fd == -1) {
        std::cout << "Failed to init SPI communication.\n";
        return -1;
    }
    std::cout << "SPI communication successfully setup.\n";

    char data[] = "mystring";
    char d_size = strlen(data);

    int arduino = wiringPiSPIGetFd(SPI_CHANNEL);
    if(arduino == -1)
        return -1;


    write(arduino, &d_size, 1);
    for (size_t i = 0; i < d_size; i++)
    {
        write(arduino, data, 1);
    }
    */
    /*   
    wiringPiSPIDataRW(SPI_CHANNEL,(unsigned char*) &d_size, 1);

    for (size_t i = 0; i < d_size; i++)
    {
        wiringPiSPIDataRW(SPI_CHANNEL,(unsigned char*) &data[i], 1);
    }
    */
    
    //write(fd, )

    
    //std::cout << "Data returned: " << +buf[1] << "\n";
    return 0;
}