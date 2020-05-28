#include <SPI.h>
#include <Arduino.h>

void setup() {
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPI.attachInterrupt();
}

// SPI interrupt routine
void ISR (SPI_STC_vect)
{
  byte c = SPDR;

  SPDR = c+10;
}  // end of interrupt service routine (ISR) for SPI

void loop () { }