/*
PPMReader library based on https://github.com/Hasi123/read-any-ppm
Works with Servo library

Paweł (DzikuVx) Spychalski 2017
https://quadmeup.com

License: GNU GPL v3
*/

#ifndef PPMReader_h
#define PPMReader_h

#include "Arduino.h"

#define PPMREADER_PMM_CHANNEL_COUNT 16

class PPMReader
{
  public:
    PPMReader(int pin, int interrupt, bool useTimer);
    int get(uint8_t channel);
    static void handler();
    volatile static int ppm[PPMREADER_PMM_CHANNEL_COUNT];
    void start(void);
    void stop(void);
    bool isReceiving(void);
  private:
    int _pin;
    int _interrupt;
};

#endif