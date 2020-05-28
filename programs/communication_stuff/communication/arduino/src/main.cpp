#include <SPI.h>

//#define DEBUG

#include "../lib/bounded.hpp"
#include "../lib/slave_instructions.hpp"

/* Buffer sizes (must be power of 2) */
#define RCV_SIZE 256
#define SND_SIZE 256

// SPI functions
void st_transfer_data(void);
void st_busy(void);

bounded_buf<RCV_SIZE, uint8_t> rcv_buf;

bounded_buf<SND_SIZE, uint8_t> snd_buf;
uint8_t snd_size = 0;

uint8_t result_buf[SND_SIZE];

enum states: uint8_t{
  r2rcv,
  recieving, 
  sending, 
  busy,
};

bool in_transfer = false;

void (*f)();

void setup()
{
  /* Initialize SPI */
  pinMode(MISO, OUTPUT); // have to send on master in, *slave out*
  SPCR |= _BV(SPE);      // turn on SPI in slave mode
  SPI.attachInterrupt(); // turn on interrupts

  Serial.begin(9600);
  

  // Sanitize buffers
  //memset(snd_buf, 'A', SND_SIZE);
  
  memset(snd_buf.buf, FILL_BYTE, SND_SIZE);
  memset(rcv_buf.buf, FILL_BYTE, RCV_SIZE);
  
  f = &st_transfer_data;

  // Finished setup
  Serial.println("Ready!");
}

ISR(SPI_STC_vect) { 
  rcv_buf.writeByte(SPDR);
  SPDR = snd_buf.readByte();
}

uint8_t counter = 0;
slave_instructions slave_ins;

void execute_istr(uint8_t *buf, uint8_t pos, uint8_t len){
  uint8_t instruction[len];
  rcv_buf.readArray(instruction, len);

  #ifdef DEBUG
  Serial.print("ins id : ");
  Serial.print(instruction[0]);
  Serial.print(", len : ");
  Serial.println(len);
  #endif

  uint8_t (*function)(uint8_t *ins, uint8_t *res);

  function = slave_ins.instr[instruction[0]];
  snd_size = function(instruction +1, result_buf +1);
  result_buf[0] = snd_size;
  
  uint8_t write_pos = snd_buf.rpos +2;
  snd_buf.writeArray(result_buf, snd_size+1, write_pos);

  /*
  char instruction[len] = "";
  rcv_buf.readArray((uint8_t*)instruction, len);
  //Serial.println(instruction);

  // Write result
  counter++;
  snd_size = 1+ sprintf((char*)result_buf+1, "Arduino saying hello for time %d", counter);
  result_buf[0] = snd_size;
  uint8_t write_pos = (snd_buf.rpos.c +2) & (SND_SIZE-1); // Write to space in front of reader
  snd_buf.writeArray(result_buf, snd_size+1, write_pos);
  */
}

void loop()
{
  if(rcv_buf.isRecieved())// Check if full instruction recieved
  {
    //Serial.println("Recieved a message");
    uint8_t len = rcv_buf.readByte();
    execute_istr(rcv_buf.buf, rcv_buf.rpos, len);
  }
  else{ // Maybe checking in wrong place
    if(rcv_buf[rcv_buf.rpos] == FILL_BYTE){

      uint8_t max_read_pos = rcv_buf.wpos;  // Dont outrun the writer
      if(rcv_buf.rpos +1 != max_read_pos)
        rcv_buf.rpos++;
    }
  }
  

}