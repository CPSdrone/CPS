#include <SPI.h>
#include <pins_arduino.h>

/* Buffer sizes */
#define RCV_SIZE 256
#define SND_SIZE 256

// SPI functions
void st_transfer_data(void);
void st_busy(void);

char rcv_buf[RCV_SIZE];
uint16_t r_epos = 0; // External / Internal pos
uint16_t r_ipos = 0;

char snd_buf[SND_SIZE];
uint16_t s_epos = 0;
uint16_t s_ipos = 0;

void (*f)();

void setup()
{
  /* Initialize SPI */
  pinMode(MISO, OUTPUT); // have to send on master in, *slave out*
  SPCR |= _BV(SPE);      // turn on SPI in slave mode
  SPCR &= 0xff - 3; // Set spi speed to 4MHz
  SPI.attachInterrupt(); // turn on interrupts

  Serial.begin(9600);
  Serial.println("--");

  f = &st_transfer_data;

  // Sanitize buffers
  memset(snd_buf, 0xff, SND_SIZE); 
  memset(rcv_buf, 0xff, RCV_SIZE);
}

void st_transfer_data(void)
{
  rcv_buf[r_epos++] = SPDR;
  SPDR = snd_buf[s_epos++];
  /*
  // Limit variables to max array sizes
  r_epos &= RCV_SIZE - 1;
  s_epos &= SND_SIZE - 1;
  */
}

void st_busy(void)
{
  SPDR = 0xff;
}

ISR(SPI_STC_vect) { f(); }

void execute_istr(char *instruction, char *result){
  // Do stuff
}

void loop()
{
  f = &st_busy;
  execute_istr(rcv_buf, snd_buf);
  f = &st_transfer_data;
  analogRead(1);
  digitalRead(1)

}

inline void perror(const char *s)
{
  Serial.print("Error : ");
  Serial.println(s);
  delay(5 * 1000); //5 seconds
}