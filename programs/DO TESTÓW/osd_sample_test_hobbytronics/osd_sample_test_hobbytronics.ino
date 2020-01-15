/*

  MAX7456 OSD Display Test Program for Arduino
  
  Hobbytronics.co.uk
  
  Nov-2013
  
  Setup is for PAL but NTSC settings are shown in the program.
  Displays text on the screen in to following format (30 chars x 16 lines)
  The program is completely self contained and requires no library files
  
  '------------------------------`
  |                              |
  | Hobbytronics.co.uk       OSD |
  |                         Test |
  |                              |  
  |                              |
  |                              |
  |                              |
  |                              | 
  |                              |
  |                              |
  |                              |
  |                              | 
  |                              |
  |                              |
  | ####                00:00:00 |  <- Clock counts up from boot 
  |                              |  
  '------------------------------`
  
*/

// SPI pins
#define DATAOUT                   11 //MOSI
#define DATAIN                    12 //MISO 
#define SPICLOCK                  13 //sck
#define MAX7456SELECT             10 //ss

//MAX7456 opcodes
#define MAX_DMM_REG             0x04
#define MAX_DMAH_REG            0x05
#define MAX_DMAL_REG            0x06
#define MAX_DMDI_REG            0x07
#define MAX_VM0_REG             0x00
#define MAX_VM1_REG             0x01
#define MAX_CMAH_REG            0x09
#define MAX_CMM_REG             0x08

//MAX7456 commands
#define MAX_CLEAR_DISPLAY       0x04
#define MAX_CLEAR_DISPLAY_VERT  0x06
#define MAX_END_STRING          0xFF

// PAL
// all VM0_reg commands need bit 6 set
#define MAX_ENABLE_DISPLAY      0x48
#define MAX_ENABLE_DISPLAY_VERT 0x4C
#define MAX_RESET               0x42
#define MAX_DISABLE_DISPLAY     0x40

#define MAX_SCREEN_SIZE          480
#define MAX_SCREEN_WIDTH          30
#define MAX_SCREEN_ROWS           16

// NTSC
//#define MAX_ENABLE_DISPLAY      0x08
//#define MAX_ENABLE_DISPLAY_VERT 0x0C
//#define MAX_RESET               0x02
//#define MAX_DISABLE_DISPLAY     0x00
//#define MAX_SCREEN_SIZE          390
//#define MAX_SCREEN_WIDTH          30
//#define MAX_SCREEN_ROWS           13

volatile byte osd_buffer[MAX_SCREEN_SIZE+1];
volatile byte osd_writeOK;

// Variables for counting time and displaying bar
#define BAR_LENGTH                15
unsigned long counter=0;
unsigned long currentTime;
unsigned long cloopTime;
unsigned long bloopTime;
unsigned char barsize=0;

//////////////////////////////////////////////////////////////
void setup()
{
  byte spi_junk;
  int x;

  pinMode(MAX7456SELECT,OUTPUT);
  digitalWrite(MAX7456SELECT,HIGH); //disable device

  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK,OUTPUT);

  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/4 rate (4 meg)
  SPCR = (1<<SPE)|(1<<MSTR);
  spi_junk=SPSR;
  spi_junk=SPDR;

  // force soft reset on Max7456
  digitalWrite(MAX7456SELECT,LOW);
  spi_send_byte(MAX_VM0_REG, MAX_RESET);   
  digitalWrite(MAX7456SELECT,HIGH);
  
  delay(200);
  
  // osd_create_block_char();  // Run this once only as data is stored in eeprom
  
  // make sure the Max7456 is enabled
  digitalWrite(MAX7456SELECT,LOW);
  spi_send_byte(MAX_VM0_REG, MAX_ENABLE_DISPLAY);
  digitalWrite(MAX7456SELECT,HIGH);

  // clear the data array
  for (x = 0; x <= MAX_SCREEN_SIZE; x++)
  {
    osd_buffer[x] = 0x00;
  }

  osd_writeOK = false;
  
  // Setup default data to display
  // 30 chars x 16 lines for PAL
  // Line 1 = bytes 1-30, Line2 = bytes 31-60 etc up to 480
  osd_print(2,2,"Hobbytronics.co.uk");
  osd_print(2,25,"OSD");  
  osd_print(3,24,"Test");
  
  currentTime = millis();
  cloopTime = currentTime;  
  bloopTime = currentTime;  
  osd_writeOK = true;
  osd_clear_screen();
}

//////////////////////////////////////////////////////////////
void loop()
{
  int i;
  unsigned char hours, mins, secs;
  
  currentTime = millis();
    
  if(currentTime >= (cloopTime + 1000))
  {  
      // This loop runs once a second and display the time 
      // in the bottom right corner
      // Increment counter every second and display time since bootup
      cloopTime = currentTime;  // Updates cloopTime       
      hours = counter / 3600;
      mins = (counter % 3600) / 60;
      secs = counter % 60;
      char ascii_str[10];
      sprintf(ascii_str,"%0.2d:%0.2d:%0.2d",hours,mins,secs);
      for(i=0;i<8;i++)
      {
         osd_buffer[440+i] = convert_ascii(ascii_str[i]);
      }   
      counter++;    
      // Update line 15 only
      osd_write_line(15); 
  }
  
  if(currentTime >= (bloopTime + 200))
  {  
      // This loop runs every 200mS and simply uses the block character
      // we created to display a bar across the screen
      // Increment bar display
      // Bar displayed at line 15 col 2 = (14*30)+2
      bloopTime = currentTime;  // Updates cloopTime      
      
      for(i=0;i<BAR_LENGTH;i++)
      {
         if(i<barsize) osd_buffer[(14*30)+2+i] = 0xC0;
         else osd_buffer[(14*30)+2+i] = convert_ascii(' ');;
      }    
      barsize++;
      if(barsize>=BAR_LENGTH) barsize=0;
      // Update line 15
      osd_write_line(15);
  }  
    
  if (osd_writeOK)
  {
    // if osd_writeOK is set then rewrite full screen
    osd_writeOK = false;
    osd_write_screen();
  }
      
}

//////////////////////////////////////////////////////////////
// Print a string to position row/col
// Places the text into the correct location in the display buffer
void osd_print(unsigned int osd_line, unsigned int osd_col, const String &s){
    for (int i = 0; i < s.length(); i++) {
      osd_buffer[((osd_line-1)*MAX_SCREEN_WIDTH)+osd_col+i] = convert_ascii((byte) s[i]);
    }  
} 

//////////////////////////////////////////////////////////////
byte spi_send_byte(byte address, volatile byte data)
{
  SPDR = address;                      // Start the transmission
  while (!(SPSR & (1<<SPIF))) {};   // Wait the end of the transmission  
  SPDR = data;                      // Start the transmission
  while (!(SPSR & (1<<SPIF))) {};   // Wait the end of the transmission
  return SPDR;                      // return the received byte
}

//////////////////////////////////////////////////////////////
byte convert_ascii (int character)
{
// for some reason the MAX7456 does not follow ascii letter
// placement, so you have to have this odd lookup table

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
void osd_create_block_char()
{
  // Create a solid White block char at character location 0xC0
  // This function only need to be called once as the character
  // is stored in the MAX7456 eeprom
  // Check out https://sites.google.com/site/qeewiki/projects/q-s-max7456-img-gen/QMIG_v1.10.rar?attredirects=0
  // for a free character creation tool for the MAX7456
  unsigned char i;
  digitalWrite(MAX7456SELECT,LOW);
  //  disable display  
  spi_send_byte(MAX_VM0_REG, MAX_DISABLE_DISPLAY);
  // Create Character at 0xC0 - See MAX7456 datasheet for character locations
  spi_send_byte(MAX_CMAH_REG, 0xC0);   // Write to address 0xC0
  
  // We need to send 54 bytes of data to store the character. Our 'block' character
  // is all the same, so we can loop here
  for(i=0;i<54;i++)
  {
    spi_send_byte(0x0A, i);
    spi_send_byte(0x0B, 0b10101010);
  }  

  spi_send_byte(MAX_CMM_REG,0xA0);

  digitalWrite(MAX7456SELECT,HIGH);
  delay(200);
}

//////////////////////////////////////////////////////////////
void osd_clear_screen()
{
  // clear the screen
  digitalWrite(MAX7456SELECT,LOW);
  spi_send_byte(MAX_DMM_REG,MAX_CLEAR_DISPLAY);
  digitalWrite(MAX7456SELECT,HIGH);
}

//////////////////////////////////////////////////////////////
void osd_write_line(unsigned int linenum)
{
  // Instead of redrawing the whole screen, we may only need to 
  // change one or more lines. 
  unsigned int x, local_count, start_address;
  byte char_address_hi, char_address_lo;
  byte osd_char;
  
  local_count = MAX_SCREEN_WIDTH;
  start_address = (linenum-1) * MAX_SCREEN_WIDTH;
  
  digitalWrite(MAX7456SELECT,LOW);

  spi_send_byte(MAX_DMM_REG,0x01);   //16 bit trans w/o background
  spi_send_byte(MAX_DMAH_REG,(unsigned char)(start_address>>8));  // set start address high
  spi_send_byte(MAX_DMAL_REG,(unsigned char)(start_address));  // set start address low

  x = 0;
  while(local_count) // write out full screen
  {
    osd_char = osd_buffer[((linenum-1)*MAX_SCREEN_WIDTH)+x];
    spi_send_byte(MAX_DMDI_REG,osd_char);
    x++;
    local_count--;
  }

  spi_send_byte(MAX_DMDI_REG,MAX_END_STRING);

  digitalWrite(MAX7456SELECT,HIGH);
}

//////////////////////////////////////////////////////////////
void osd_write_screen()
{
  // Change the data for the whole screen
  int x, local_count;
  byte char_address_hi, char_address_lo;
  byte osd_char;
  
  local_count = MAX_SCREEN_SIZE;
  
  digitalWrite(MAX7456SELECT,LOW);

  spi_send_byte(MAX_DMM_REG,0x01);   //16 bit trans w/o background
  spi_send_byte(MAX_DMAH_REG,0x00);  // set start address high
  spi_send_byte(MAX_DMAL_REG,0x00);  // set start address low

  x = 0;
  while(local_count) // write out full screen
  {
    osd_char = osd_buffer[x];
    spi_send_byte(MAX_DMDI_REG,osd_char);
    x++;
    local_count--;
  }

  spi_send_byte(MAX_DMDI_REG,MAX_END_STRING);

  digitalWrite(MAX7456SELECT,HIGH);
}
