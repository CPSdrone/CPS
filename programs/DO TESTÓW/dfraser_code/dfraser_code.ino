
#include <EEPROM.h> //Needed to access eeprom read/write functions

#define DATAOUT 11//MOSI
#define DATAIN  12//MISO
#define SPICLOCK  13//sck
#define MAX7456SELECT 10//ss
#define VSYNC 2// INT0

//MAX7456 opcodes
#define DMM_reg   0x04
#define DMAH_reg  0x05
#define DMAL_reg  0x06
#define DMDI_reg  0x07
#define VM0_reg   0x00
#define VM1_reg   0x01

//MAX7456 commands
#define CLEAR_display 0x04
#define CLEAR_display_vert 0x06
#define END_string 0xff
// with NTSC
#define ENABLE_display 0x08
#define ENABLE_display_vert 0x0c
#define MAX7456_reset 0x02
#define DISABLE_display 0x00

// with PAL
// all VM0_reg commands need bit 6 set
//#define ENABLE_display 0x48
//#define ENABLE_display_vert 0x4c
//#define MAX7456_reset 0x42
//#define DISABLE_display 0x40

#define WHITE_level_80 0x03
#define WHITE_level_90 0x02
#define WHITE_level_100 0x01
#define WHITE_level_120 0x00

// with NTSC
#define MAX_screen_size 390
#define MAX_screen_rows 13

// with PAL
//#define MAX_screen_size 480
//#define MAX_screen_rows 16

#define EEPROM_address_hi 510
#define EEPROM_address_low 511
#define EEPROM_sig_hi 'e'
#define EEPROM_sig_low 's'

volatile byte screen_buffer[MAX_screen_size];

volatile byte writeOK;
volatile byte valid_string;
volatile byte save_screen;
volatile int  incomingByte;
volatile int  count;

//
//##############################
//# 30 characters per line     #
//# 13 lines per screen        #
//# start'[' end ']' 390 chars #
//# add '~' if front of '[' to #
//# force write to eeprom      #
//# '~[]' to zero out eeprom   #
//##############################
//
//[
//
//
//       THIS IS A TEST
//
//    Arduino / MAX7456-OSD
//
//  ABCDEFGHIJKLMNOPQRSTUVWXYZ
//         0123456789
//]
//

//////////////////////////////////////////////////////////////
void setup()
{
 byte spi_junk, eeprom_junk;
 int x;
 Serial.begin(9600);
 Serial.flush();

 pinMode(MAX7456SELECT,OUTPUT);
 digitalWrite(MAX7456SELECT,HIGH); //disable device

 pinMode(DATAOUT, OUTPUT);
 pinMode(DATAIN, INPUT);
 pinMode(SPICLOCK,OUTPUT);
 pinMode(VSYNC, INPUT);

 // SPCR = 01010000
 //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
 //sample on leading edge of clk,system clock/4 rate (4 meg)
 SPCR = (1<<SPE)|(1<<MSTR);
 spi_junk=SPSR;
 spi_junk=SPDR;
 delay(250);

 // force soft reset on Max7456
 digitalWrite(MAX7456SELECT,LOW);
 spi_transfer(VM0_reg);
 spi_transfer(MAX7456_reset);
 digitalWrite(MAX7456SELECT,HIGH);
 delay(500);

 // set all rows to same charactor white level, 90%
 digitalWrite(MAX7456SELECT,LOW);
 for (x = 0; x < MAX_screen_rows; x++)
 {
   spi_transfer(x + 0x10);
   spi_transfer(WHITE_level_90);
 }

 // make sure the Max7456 is enabled
 spi_transfer(VM0_reg);
 spi_transfer(ENABLE_display);
 digitalWrite(MAX7456SELECT,HIGH);


 // clear the array
 for (x = 0; x < MAX_screen_size; x++)
 {
   screen_buffer[x] = 0x00;
 }

 writeOK = false;
 valid_string = false;
 save_screen = false;
 incomingByte = 0;
 count = 0;

// check to see if we have a default screen stored in eeprom
 eeprom_junk = EEPROM.read(EEPROM_address_hi);
 if (eeprom_junk == EEPROM_sig_hi)
 {
   eeprom_junk = EEPROM.read(EEPROM_address_low);
   if (eeprom_junk == EEPROM_sig_low)
   {
     for (x = 0; x < MAX_screen_size; x++)
     {
       screen_buffer[x] = EEPROM.read(x);
     }
     writeOK = true;
     count = MAX_screen_size;
   }
 }

 Serial.println("Ready for text file download");
 Serial.println("");
 delay(100);  
}








//////////////////////////////////////////////////////////////
void loop()
{
 int x, junk;
 
 if (Serial.available() > 0)
 {
   // read the incoming byte:
   incomingByte = Serial.read();

   if (incomingByte == 126) // this is the '~' for saving the screen
     save_screen = true;
     
   switch(incomingByte)
   {
     case 0x5b: // [ start of text string
       //Serial.println("start of text");
       count = 0;
       valid_string = true;
     break;
     case 0x0a: // line feed, ignore
       //Serial.println("ln");  
     break;
     case 0x0d: // carridge return, skip to start of next line
       //Serial.println("cr");  
       if (count < 30)
         count = 30;
       else
       {
         junk = (count % 30);
         count = (count + (30 - junk));
       }
     break;
     case 0x5d: // ] end of text
       //Serial.println("end of text");
       Serial.flush();
       valid_string = false;
       writeOK = true;
     break;
     default:
       if (valid_string)
       {
         //Serial.println("buffer chars");
         if (count == MAX_screen_size) // something is wrong, too many characters
           count = count -1;  // drop it back one to position 389
         screen_buffer[count] = convert_ascii(incomingByte);
         count++;
       }
     break;
   }
 }

 if (writeOK)
 {
   writeOK = false;
   write_new_screen();

   // have we received a '~' to save screen to eeprom
   if (save_screen)
   {
     save_screen = false;
     
     // write contents of screen buffer to eeprom
     if (count > 0)
     {
       for (x = 0; x < count; x++)
       {
         EEPROM.write(x, screen_buffer[x]);
       }
       EEPROM.write(EEPROM_address_hi, EEPROM_sig_hi);
       EEPROM.write(EEPROM_address_low, EEPROM_sig_low);
     }
     else // to erase all eeprom 'send ~[]'
     {
       for (x = 0; x < 512; x++)
       {
         EEPROM.write(x, 0);
       }
     }
   }

   // clear out screen_buffer
   for (x = 0; x < MAX_screen_size; x++)
   {
     screen_buffer[x] = 0x00;
   }
   count = 0;
 }  
}

//////////////////////////////////////////////////////////////
byte spi_transfer(volatile byte data)
{
 SPDR = data;                    // Start the transmission
 while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
 {
 };
 return SPDR;                    // return the received byte
}

//////////////////////////////////////////////////////////////
byte convert_ascii (int character)
{
// for some reason the MAX7456 does not follow ascii letter
// placement, so you have to have this odd lookup table
// todo... create an eeprom table that matches ascii
// and burn to the MAX7456

 byte lookup_char;

 if (character == 32)
   lookup_char = 0x00; // blank space
 else if (character == 48)
   lookup_char = 0x0a; // 0
 else if ((character > 48) && (character < 58))
   lookup_char = (character - 48); // 1-9
 else if ((character > 64) && (character < 90))
   lookup_char = (character - 54); // A-Z
 else if ((character > 96) && (character < 123))
   lookup_char = (character - 60); // a-z
 else if (character == 34)
   lookup_char = 0x48; // "
 else if (character == 39)
   lookup_char = 0x46; // '
 else if (character == 40)
   lookup_char = 0x3f; // (
 else if (character == 41)
   lookup_char = 0x40; // )
 else if (character == 44)
   lookup_char = 0x45; // ,
 else if (character == 45)
   lookup_char = 0x49; // -
 else if (character == 46)
   lookup_char = 0x41; // .
 else if (character == 47)
   lookup_char = 0x47; // /
 else if (character == 58)
   lookup_char = 0x44; // :
 else if (character == 59)
   lookup_char = 0x43; // ;
 else if (character == 60)
   lookup_char = 0x4a; // <
 else if (character == 62)
   lookup_char = 0x4b; // >
 else if (character == 63)
   lookup_char = 0x42; // ?
 else
   lookup_char = 0x00; // out of range, blank space

return (lookup_char);

}

//////////////////////////////////////////////////////////////
void write_new_screen()
{
 int x, local_count;
 byte char_address_hi, char_address_lo;
 byte screen_char;
 
 local_count = count;
 
 char_address_hi = 0;
 char_address_lo = 0;
//Serial.println("write_new_screen");  

 // clear the screen
 digitalWrite(MAX7456SELECT,LOW);
 spi_transfer(DMM_reg);
 spi_transfer(CLEAR_display);
 digitalWrite(MAX7456SELECT,HIGH);

 // disable display
 digitalWrite(MAX7456SELECT,LOW);
 spi_transfer(VM0_reg);
 spi_transfer(DISABLE_display);

 spi_transfer(DMM_reg); //dmm
 //spi_transfer(0x21); //16 bit trans background
 spi_transfer(0x01); //16 bit trans w/o background

 spi_transfer(DMAH_reg); // set start address high
 spi_transfer(char_address_hi);

 spi_transfer(DMAL_reg); // set start address low
 spi_transfer(char_address_lo);

 x = 0;
 while(local_count) // write out full screen
 {
   screen_char = screen_buffer[x];
   spi_transfer(DMDI_reg);
   spi_transfer(screen_char);
   x++;
   local_count--;
 }

 spi_transfer(DMDI_reg);
 spi_transfer(END_string);

 spi_transfer(VM0_reg); // turn on screen next vertical
 spi_transfer(ENABLE_display_vert);
 digitalWrite(MAX7456SELECT,HIGH);
}
