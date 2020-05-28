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
#include <mutex>

#include "instruction.h"

#define SPI_CLOCK_1MHz 1000000

// Clock giving stable read and writes to arduino
#define SPI_CLOCK_STABLE_RW (SPI_CLOCK_1MHz * 6.6)

#define POLL_TIME std::chrono::microseconds(10) // Can be 0, halves timings for small transfers

// Signals sent by arduino
#define SIGNAL_R2RCV 0x00
#define SIGNAL_BUSY 0xff

// Signals sent do adruino
#define SIGNAL_POLL 0xff

#define ANS_MAX_SIZE 100

/* Send and recieve data via SPI
   Seperate object is needed for each spidev
 */
class spi_sock
{
public:
    uint8_t mode;
    uint8_t bits = 8; // Bits per word
    uint32_t speed;
    uint16_t delay;
    int fd;
    const char *device = "/dev/spidev0.0"; // all are in form "/dev/spidevX.Y" - all have same length

    /* Transfer LEN bytes of MSG via spi, put response into RESPONSE,
    assume RESPONSE is big enough */
    void transfer_raw(const uint8_t *msg, int len, uint8_t *response);

    /* Transfer LEN bytes of MSG via spi, put response into RESPONSE
    assume RESPONSE is big enough and initialized to zeros */
    void transfer_raw_zeroed(const uint8_t *msg,unsigned int len, uint8_t *response);
    
    /* Check if arduino is waiting for instructions MAX_TRIES times
        stop checking when arduino ready for instructions 
        return last value sent by arduino */
    uint8_t wait_until_R2RCV();

    /* Wait between transfers */
    void wait_btw_transfers();

    /* Transfer array of bytes one by one - works up to 8MHz */
    void transfer_bytes(const uint8_t *msg, uint32_t len, uint8_t *resp);

    /* Send bin from NEXT, recieve result into PREVIOUS */
    void exchange(instruction *previous, instruction *next);

    static std::mutex m;

//public:
    spi_sock(uint32_t speed = SPI_CLOCK_STABLE_RW, uint8_t mode = SPI_MODE_0, uint16_t delay = 0);
    
    /* Send instruction INS */
    void send_instruction(instruction &ins);
    ~spi_sock();
};




#endif