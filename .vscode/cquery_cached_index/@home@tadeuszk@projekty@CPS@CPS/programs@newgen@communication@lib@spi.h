#ifndef SPI_H
#define SPI_H

// #include <wiringPiSPI.h> Deprecated
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <thread>
#include <chrono>

#include "mt-queue.hpp"
#include "instruction.h"

#define SPI_CLOCK_1MHz 1000000

// Clock giving stable read and writes to arduino
#define SPI_CLOCK_STABLE_RW (SPI_CLOCK_1MHz/5.8) 

#define POLL_TIME std::chrono::microseconds(10)

#define RCV_LEN 256 // Must be set same as arduino
#define SND_LEN 256


/* Send and recieve data via SPI
   Seperate object is needed for each spidev
 */
class spi_sock
{
private:
    uint8_t mode;
    uint8_t bits = 8; // Bits per word
    uint32_t speed;
    uint16_t delay;
    int fd;
    const char *device = "/dev/spidev0.0"; // all are in form "/dev/spidevX.Y" - all have same length

    /* Transfer LEN bytes of MSG via spi, put response into RESPONSE,
    assume RESPONSE is big enough */
    void transfer_raw(const char *msg, int len, char *response);

    /* Transfer LEN bytes of MSG via spi, put response into RESPONSE
    assume RESPONSE is big enough and initialized to zeros */
    void transfer_raw_zeroed(const char *msg, unsigned int len, char *response);

    /* Wait between transfers */
    void wait_btw_transfers();

    /* Send bin from NEXT, recieve result into PREVIOUS */
    void exchange(instruction *previous, instruction *next);
    bool run_thread = true;
    /* Exchange data on seperate thread while run_thread is true */
    void executor(MT_Queue<instruction*> &q, bool &run_thread);
    MT_Queue<instruction*> queue;
    std::thread th1;

public:
    spi_sock(uint32_t speed = SPI_CLOCK_STABLE_RW, uint8_t mode = SPI_MODE_0, uint16_t delay = 0);
    
    /* Send instruction INS of length LEN, put response into RESP
    assume RESP is big enough, return RESP size */
    void send_instruction(instruction *ins, uint8_t *resp);

    /* Send instruction INS of length LEN */
    void send_instruction(instruction *ins);
    ~spi_sock();
};

void send_data(char* buf, char length, int spi_channel);




#endif