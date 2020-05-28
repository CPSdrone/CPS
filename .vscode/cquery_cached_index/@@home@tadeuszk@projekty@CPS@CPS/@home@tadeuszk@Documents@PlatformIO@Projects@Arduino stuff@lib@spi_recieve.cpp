#include<SPI.h>
volatile int i = 0;
byte myArray[2];

void setup()
{
  Serial.begin(9600);
  pinMode(SS, INPUT_PULLUP);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, INPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();  //allows SPI interrupt
}

void loop(void)
{
  if (i == 2)
  {
    int x = (int)myArray[0]<<8|(int)myArray[1];
    Serial.print("Received 16-bit data item from Master: ");
    Serial.println(x, HEX);
    i=0;
    Serial.println("=============================================");
  }
}

ISR (SPI_STC_vect)   //Inerrrput routine function
{
  myArray[i] = SPDR;
  i++;
  
}